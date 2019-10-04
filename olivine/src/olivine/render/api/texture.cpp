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

#include "olivine/render/api/texture.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/app/app.hpp"
#include "olivine/render/api/device.hpp"
#include "olivine/render/api/descriptor_allocator.hpp"

// ========================================================================== //
// Texture Implementation
// ========================================================================== //

namespace olivine {

Texture::Texture(const CreateInfo& createInfo)
  : mWidth(createInfo.width)
  , mHeight(createInfo.height)
  , mDepth(createInfo.depth)
  , mDimension(createInfo.dimension)
  , mFormat(createInfo.format)
  , mUsages(createInfo.usages)
{
  // Assert preconditions
  Assert(createInfo.width != 0 && createInfo.height != 0,
         "Invalid texture creation information. Width and height cannot be "
         "zero (0)");
  Assert(createInfo.format != Format::kInvalid,
         "Invalid texture creation information. Format cannot be 'kInvalid'");

  Device* device = App::Instance()->GetDevice();

  // Setup resource descriptor
  D3D12_RESOURCE_DESC desc;
  desc.Dimension = ToResourceDim(createInfo.dimension);
  desc.Alignment = 0;
  desc.Width = createInfo.width;
  desc.Height = createInfo.height;
  desc.DepthOrArraySize =
    createInfo.dimension == Dim::k3D ? createInfo.depth : createInfo.arraySize;
  desc.MipLevels = createInfo.mipLevels;
  desc.Format = D3D12Util::ToDXGIFormat(createInfo.format);
  desc.SampleDesc.Count = 1;
  desc.SampleDesc.Quality = 0;
  desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
  desc.Flags = D3D12_RESOURCE_FLAG_NONE;

  // Setup flags
  if (u32(createInfo.usages & Usage::kColorAttachment)) {
    desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
  }
  if (u32(createInfo.usages & Usage::kDepthStencilAttachment)) {
    desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
  }
  if (!u32(createInfo.usages & Usage::kShaderResource)) {
    desc.Flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
  }

  // Create resource
  D3D12MA::ALLOCATION_DESC allocationDesc{};
  allocationDesc.Flags = D3D12MA::ALLOCATION_FLAG_NONE;
  allocationDesc.HeapType = D3D12Util::ToHeapType(createInfo.heapKind);
  const HRESULT hresult = device->GetAllocator()->CreateResource(
    &allocationDesc,
    &desc,
    D3D12_RESOURCE_STATE_COMMON,
    nullptr,
    &mAllocation,
    __uuidof(ID3D12Resource),
    reinterpret_cast<void**>(&mResource));
  Assert(SUCCEEDED(hresult), "Failed to create texture");

  // Create views
  CreateViews();
}

// -------------------------------------------------------------------------- //

Texture::Texture(ID3D12Resource* resource,
                 u32 width,
                 u32 height,
                 u32 depth,
                 Dim dimension,
                 Format format,
                 Usage usages)
  : mResource(resource)
  , mWidth(width)
  , mHeight(height)
  , mDepth(depth)
  , mDimension(dimension)
  , mFormat(format)
  , mUsages(usages)
{
  resource->AddRef();
  CreateViews();
}

// -------------------------------------------------------------------------- //

Texture::~Texture()
{
  // Release allocation
  if (mAllocation) {
    mAllocation->Release();
  }

  // Release resource
  mResource->Release();
  mResource = nullptr;
}

// -------------------------------------------------------------------------- //

Texture::BufferRequirements
Texture::GetBufferRequirements() const
{
  Device* device = App::Instance()->GetDevice();

  D3D12_RESOURCE_DESC desc = mResource->GetDesc();
  UINT64 size, rowsize;
  D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
  device->GetHandle()->GetCopyableFootprints(
    &desc, 0, 1, 0, &footprint, nullptr, &rowsize, &size);

  BufferRequirements reqs;
  reqs.size = u64(size);
  reqs.alignment = desc.Alignment;
  reqs.rowStride = footprint.Footprint.RowPitch;
  return reqs;
}

// -------------------------------------------------------------------------- //

void
Texture::SetName(const String& name)
{
  // Name resource
  D3D12Util::SetName(mResource, name);

  // Name allocation
  if (mAllocation) {
    char16* _name = (name + "Mem").GetUTF16();
    mAllocation->SetName(_name);
    delete _name;
  }
}

// -------------------------------------------------------------------------- //

void
Texture::CreateViews()
{
  Device* device = App::Instance()->GetDevice();

  mSrvHandle = DescriptorAllocator::kInvalidHandle;
  mRtvHandle = DescriptorAllocator::kInvalidHandle;

  // Create SRV
  if (bool(mUsages & Usage::kShaderResource)) {
    if (mDimension == Dim::k1D) {
    } else if (mDimension == Dim::k2D) {
    } else if (mDimension == Dim::k3D) {
    }
  }
}

// -------------------------------------------------------------------------- //

D3D12_RESOURCE_DIMENSION
Texture::ToResourceDim(Dim dimension)
{
  switch (dimension) {
    case Dim::k1D: {
      return D3D12_RESOURCE_DIMENSION_TEXTURE1D;
    }
    case Dim::k2D: {
      return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    }
    case Dim::k3D: {
      return D3D12_RESOURCE_DIMENSION_TEXTURE3D;
    }
    default: {
      return (D3D12_RESOURCE_DIMENSION)UINT_MAX;
    }
  }
}

}
