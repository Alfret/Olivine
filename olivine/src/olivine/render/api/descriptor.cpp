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

#include "olivine/render/api/descriptor.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/app/app.hpp"
#include "olivine/render/api/device.hpp"
#include "olivine/render/api/texture.hpp"

// ========================================================================== //
// Descriptor Implementation
// ========================================================================== //

namespace olivine {

Descriptor::Descriptor(Kind kind,
                       D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle,
                       D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle)
  : mKind(kind)
  , mCpuHandle(cpuHandle)
  , mGpuHandle(gpuHandle)
{}

// -------------------------------------------------------------------------- //

D3D12_DESCRIPTOR_HEAP_TYPE
Descriptor::ToDescriptorType(Kind kind)
{
  switch (kind) {
    case Kind::kCbvSrvUav: {
      return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    }
    case Kind::kSampler: {
      return D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
    }
    case Kind::kRtv: {
      return D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    }
    case Kind::kDsv: {
      return D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    }
    default: {
      return (D3D12_DESCRIPTOR_HEAP_TYPE)UINT_MAX;
    }
  }
}

}

// ========================================================================== //
// DescriptorHeap Implementation
// ========================================================================== //

namespace olivine {

DescriptorHeap::DescriptorHeap(Descriptor::Kind kind,
                               u32 count,
                               bool shaderVisible)
  : mKind(kind)
  , mCount(count)
  , mIsShaderVisible(shaderVisible)
{
  Device* device = App::Instance()->GetDevice();

  // Setup heap info
  D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
  heapDesc.Type = Descriptor::ToDescriptorType(mKind);
  heapDesc.NumDescriptors = mCount;
  heapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
                                 : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
  heapDesc.NodeMask = 0x00;

  // Create descriptor heap
  const HRESULT hresult = device->GetHandle()->CreateDescriptorHeap(
    &heapDesc,
    __uuidof(ID3D12DescriptorHeap),
    reinterpret_cast<void**>(&mHandle));
  Assert(SUCCEEDED(hresult), "Failed to create descriptor heap");

  // Retrieve handle size
  mHandleSize = device->GetHandle()->GetDescriptorHandleIncrementSize(
    Descriptor::ToDescriptorType(mKind));
  Assert(mHandleSize != 0, "Size of descriptor handle cannot be 0");
}

// -------------------------------------------------------------------------- //

DescriptorHeap::~DescriptorHeap()
{
  // Release heap
  mHandle->Release();
  mHandle = nullptr;
}

// -------------------------------------------------------------------------- //

Descriptor
DescriptorHeap::WriteDescriptorRTV(u32 index, Texture* texture, Format format)
{
  Device* device = App::Instance()->GetDevice();

  // If format is 'invalid' then set to texture
  if (format == Format::kInvalid) {
    format = texture->GetFormat();
  }

  // Retrieve CPU handle
  D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle =
    mHandle->GetCPUDescriptorHandleForHeapStart();
  cpuHandle.ptr += (u64(mHandleSize) * u64(index));

  // Create view
  D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
  rtvDesc.Format = D3D12Util::ToDXGIFormat(format);
  rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
  rtvDesc.Texture2D.MipSlice = 0;
  rtvDesc.Texture2D.PlaneSlice = 0;
  device->GetHandle()->CreateRenderTargetView(
    texture->GetResource(), &rtvDesc, cpuHandle);

  // Retrieve GPU handle (Might be 0)
  D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle =
    mHandle->GetGPUDescriptorHandleForHeapStart();
  gpuHandle.ptr += (u64(mHandleSize) * u64(index));

  // Return descriptor
  return Descriptor(mKind, cpuHandle, gpuHandle);
}

// -------------------------------------------------------------------------- //

Descriptor
DescriptorHeap::WriteDescriptorSRV(u32 index, Texture* texture, Format format)
{
  Device* device = App::Instance()->GetDevice();

  // If format is 'invalid' then set to texture
  if (format == Format::kInvalid) {
    format = texture->GetFormat();
  }

  // Retrieve CPU handle
  D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle =
    mHandle->GetCPUDescriptorHandleForHeapStart();
  cpuHandle.ptr += (u64(mHandleSize) * u64(index));

  // Create view
  D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
  srvDesc.Format = D3D12Util::ToDXGIFormat(format);
  srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
  srvDesc.Texture2D.MostDetailedMip = 0;
  srvDesc.Texture2D.MipLevels = 1;
  srvDesc.Texture2D.PlaneSlice = 0;
  srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

  device->GetHandle()->CreateShaderResourceView(
    texture->GetResource(), &srvDesc, cpuHandle);

  // Retrieve GPU handle (Might be 0)
  D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle =
    mHandle->GetGPUDescriptorHandleForHeapStart();
  gpuHandle.ptr += (u64(mHandleSize) * u64(index));

  // Return descriptor
  return Descriptor(mKind, cpuHandle, gpuHandle);
}

// -------------------------------------------------------------------------- //

Descriptor
DescriptorHeap::At(u32 index) const
{
  Assert(index < mCount,
         "Descriptor index out of bounds: {} not in [0, {})",
         index,
         mCount);

  // Retrieve CPU handle
  D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle =
    mHandle->GetCPUDescriptorHandleForHeapStart();
  cpuHandle.ptr += (u64(mHandleSize) * u64(index));

  // Retrieve GPU handle (Might be 0)
  D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle =
    mHandle->GetGPUDescriptorHandleForHeapStart();
  gpuHandle.ptr += (u64(mHandleSize) * u64(index));

  // Return handle
  return Descriptor(mKind, cpuHandle, gpuHandle);
}

// -------------------------------------------------------------------------- //

Descriptor DescriptorHeap::operator[](u32 index)
{
  return At(index);
}

// -------------------------------------------------------------------------- //

void
DescriptorHeap::SetName(const String& name)
{
  D3D12Util::SetName(mHandle, name);
}

}
