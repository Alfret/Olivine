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
#include "olivine/render/api/context.hpp"

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
  ArrayList<IDXGIAdapter1*> adapters = EnumerateAdapters();
  // TODO(Filip Björklund): Implement selection
  mAdapter = adapters[0];
  for (IDXGIAdapter1* adapter : adapters) {
    if (adapter != mAdapter) {
      adapter->Release();
    }
  }
  D3D12Util::SetName(mAdapter, "DeviceAdapter");

  // Create device
  HRESULT hresult = D3D12CreateDevice(mAdapter,
                                      D3D_FEATURE_LEVEL_12_0,
                                      __uuidof(ID3D12Device4),
                                      reinterpret_cast<void**>(&mHandle));
  Assert(SUCCEEDED(hresult), "Failed to create device");
  D3D12Util::SetName(mHandle, "Device");

  // Create allocator
  D3D12MA::ALLOCATOR_DESC allocatorDesc{};
  allocatorDesc.pDevice = mHandle;
  allocatorDesc.Flags = D3D12MA::ALLOCATOR_FLAG_NONE;
  hresult = D3D12MA::CreateAllocator(&allocatorDesc, &mAllocator);
  Assert(SUCCEEDED(hresult), "Failed to create device allocator");
}

// -------------------------------------------------------------------------- //

Device::~Device()
{
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

ArrayList<IDXGIAdapter1*>
Device::EnumerateAdapters()
{
  App* app = App::Instance();
  Context* ctx = app->GetContext();

  ArrayList<IDXGIAdapter1*> adapters;
  IDXGIAdapter1* adapter;
  UINT index = 0;
  while (ctx->GetFactory()->EnumAdapters1(index++, &adapter) !=
         DXGI_ERROR_NOT_FOUND) {
    adapters.Append(adapter);
  }
  return adapters;
}

}
