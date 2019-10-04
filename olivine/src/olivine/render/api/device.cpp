// MIT License
//
// Copyright (c) 2019 Filip Björklund
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "olivine/render/api/device.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/app/app.hpp"
#include "olivine/math/limits.hpp"
#include "olivine/render/api/context.hpp"
#include "olivine/render/api/descriptor_allocator.hpp"

// ========================================================================== //
// Device Implementation
// ========================================================================== //

namespace olivine {

Device::Device(const CreateInfo& createInfo)
{
  Init(createInfo);
}

// -------------------------------------------------------------------------- //

void
Device::Init(const CreateInfo& createInfo)
{
  // Choose adapter
  ArrayList<IDXGIAdapter3*> adapters = EnumerateAdapters();
  // TODO(Filip Björklund): Implement selection
  mAdapter = adapters[0];
  for (IDXGIAdapter1* adapter : adapters) {
    if (adapter != mAdapter) {
      adapter->Release();
    }
  }
  D3D12Util::SetName(mAdapter, "DeviceAdapter");

  // Create device
  HRESULT hresult = S_OK;
  try {
    hresult = D3D12CreateDevice(mAdapter,
                                D3D_FEATURE_LEVEL_12_0,
                                __uuidof(ID3D12Device4),
                                reinterpret_cast<void**>(&mHandle));
  } catch (std::exception e) {
    Assert(false, "Device creation threw an exception ({})", e.what());
  }
  Assert(SUCCEEDED(hresult), "Failed to create device");
  D3D12Util::SetName(mHandle, "Device");

  // Create allocator
  D3D12MA::ALLOCATOR_DESC allocatorDesc{};
  allocatorDesc.pDevice = mHandle;
  allocatorDesc.Flags = D3D12MA::ALLOCATOR_FLAG_NONE;
  hresult = D3D12MA::CreateAllocator(&allocatorDesc, &mAllocator);
  Assert(SUCCEEDED(hresult), "Failed to create device allocator");

  // Create descriptor allocators
  mDescriptorAllocatorCbvUavSrv =
    new DescriptorAllocator(Descriptor::Kind::kCbvSrvUav, false, 2048);
  mDescriptorAllocatorSampler =
    new DescriptorAllocator(Descriptor::Kind::kSampler, false, 128);
  mDescriptorAllocatorRtv =
    new DescriptorAllocator(Descriptor::Kind::kRtv, false, 32);
  mDescriptorAllocatorDsv =
    new DescriptorAllocator(Descriptor::Kind::kDsv, false, 16);

  // Check ray-tracing support
  D3D12_FEATURE_DATA_D3D12_OPTIONS5 options5;
  hresult = mHandle->CheckFeatureSupport(
    D3D12_FEATURE_D3D12_OPTIONS5, &options5, sizeof options5);
  mFeatures.rayTracing = false;
  if (SUCCEEDED(hresult)) {
    mFeatures.rayTracing =
      options5.RaytracingTier != D3D12_RAYTRACING_TIER_NOT_SUPPORTED;
  }

  // Check ray-tracing support
  D3D12_FEATURE_DATA_D3D12_OPTIONS6 options6;
  hresult = mHandle->CheckFeatureSupport(
    D3D12_FEATURE_D3D12_OPTIONS6, &options6, sizeof options6);
  mFeatures.vrs.tier = TierVRS::kNoSupport;
  if (SUCCEEDED(hresult)) {
    if (options6.VariableShadingRateTier ==
        D3D12_VARIABLE_SHADING_RATE_TIER_1) {
      mFeatures.vrs.tier = TierVRS::kTier1;
    } else if (options6.VariableShadingRateTier ==
               D3D12_VARIABLE_SHADING_RATE_TIER_2) {
      mFeatures.vrs.tier = TierVRS::kTier2;
    }
  }
}

// -------------------------------------------------------------------------- //

Device::~Device()
{
  // Destroy descriptor allocators
  delete mDescriptorAllocatorDsv;
  delete mDescriptorAllocatorRtv;
  delete mDescriptorAllocatorSampler;
  delete mDescriptorAllocatorCbvUavSrv;

  // Release allocator
  mAllocator->Release();
  mAllocator = nullptr;

  // Release device
  mHandle->Release();
  mHandle = nullptr;

  // Release adapter
  mAdapter->Release();
  mAdapter = nullptr;
}

// -------------------------------------------------------------------------- //

u64
Device::GetMemoryUsage()
{
  DXGI_QUERY_VIDEO_MEMORY_INFO info;
  const HRESULT hresult = mAdapter->QueryVideoMemoryInfo(
    0x00, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &info);
  if (FAILED(hresult)) {
    return Limits::kU64Max;
  }
  return info.CurrentUsage;
}

// -------------------------------------------------------------------------- //

u64
Device::GetMemoryBudget()
{
  DXGI_QUERY_VIDEO_MEMORY_INFO info;
  const HRESULT hresult = mAdapter->QueryVideoMemoryInfo(
    0x00, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &info);
  if (FAILED(hresult)) {
    return Limits::kU64Max;
  }
  return info.Budget;
}

// -------------------------------------------------------------------------- //

ArrayList<IDXGIAdapter3*>
Device::EnumerateAdapters()
{
  App* app = App::Instance();
  Context* ctx = app->GetContext();

  ArrayList<IDXGIAdapter3*> adapters;
  IDXGIAdapter1* _adapter;
  UINT index = 0;
  while (ctx->GetFactory()->EnumAdapters1(index++, &_adapter) !=
         DXGI_ERROR_NOT_FOUND) {
    D3D12Util::SetName(_adapter, "EnumAdapter1({})", index);

    // Query adapter3 interface
    IDXGIAdapter3* adapter;
    const HRESULT hresult = _adapter->QueryInterface(IID_PPV_ARGS(&adapter));
    _adapter->Release();
    if (FAILED(hresult)) {
      continue;
    }
    D3D12Util::SetName(adapter, "EnumAdapter3({})", index);

    // Make sure that it's not a software adapter and that D3D12 is supported
    DXGI_ADAPTER_DESC1 adapterDesc;
    adapter->GetDesc1(&adapterDesc);
    if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
      adapter->Release();
      continue;
    }
    if (FAILED(D3D12CreateDevice(
          adapter, D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device4), nullptr))) {
      adapter->Release();
      continue;
    }

    // Add adapter to list
    adapters.Append(adapter);
  }
  Assert(adapters.GetSize() > 0, "Failed to enumerate adapters");
  return adapters;
}

}
