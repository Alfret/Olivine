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

#include "olivine/render/api/buffer.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/app/app.hpp"
#include "olivine/render/api/device.hpp"

// ========================================================================== //
// Buffer Implementation
// ========================================================================== //

namespace olivine {

Buffer::Buffer(const CreateInfo& createInfo)
  : Buffer(createInfo.size,
           createInfo.usages,
           createInfo.heapKind,
           createInfo.alignment)
{}

// -------------------------------------------------------------------------- //

Buffer::Buffer(u64 size, Usage usages, HeapKind heapKind, u32 alignment)
  : mSize(size)
{
  Device* device = App::Instance()->GetDevice();

  // Setup resource descriptor
  D3D12_RESOURCE_DESC desc{};
  desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
  desc.Alignment = alignment;
  desc.Width = mSize;
  desc.Height = 1;
  desc.DepthOrArraySize = 1;
  desc.MipLevels = 1;
  desc.Format = DXGI_FORMAT_UNKNOWN;
  desc.SampleDesc.Count = 1;
  desc.SampleDesc.Quality = 0;
  desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
  desc.Flags = D3D12_RESOURCE_FLAG_NONE;

  // Setup flags
  if (u32(usages & Usage::kUnorderedAccess)) {
    desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
  }

  // Determine initial state
  D3D12_RESOURCE_STATES initialState = D3D12_RESOURCE_STATE_COMMON;
  if (heapKind == HeapKind::kUpload) {
    initialState = D3D12_RESOURCE_STATE_GENERIC_READ;
  }
  if (heapKind == HeapKind::kReadback) {
    initialState = D3D12_RESOURCE_STATE_COPY_DEST;
  }

  // Create resource
  D3D12MA::ALLOCATION_DESC allocationDesc{};
  allocationDesc.Flags = D3D12MA::ALLOCATION_FLAG_NONE;
  allocationDesc.HeapType = D3D12Util::ToHeapType(heapKind);
  const HRESULT hresult = device->GetAllocator()->CreateResource(
    &allocationDesc,
    &desc,
    initialState,
    nullptr,
    &mAllocation,
    __uuidof(ID3D12Resource),
    reinterpret_cast<void**>(&mResource));
  Assert(SUCCEEDED(hresult), "Failed to create buffer");
}

// -------------------------------------------------------------------------- //

Buffer::~Buffer()
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

void*
Buffer::Map()
{
  void* pointer;
  HRESULT hresult = mResource->Map(0, nullptr, &pointer);
  Assert(SUCCEEDED(hresult), "Failed to map buffer");
  return pointer;
}

// -------------------------------------------------------------------------- //

void
Buffer::Unmap()
{
  mResource->Unmap(0, nullptr);
}

// -------------------------------------------------------------------------- //

void
Buffer::Write(u8* data, u64 size)
{
  void* memory = Map();
  Memory::Copy(memory, data, size);
  Unmap();
}

// -------------------------------------------------------------------------- //

void
Buffer::SetName(const String& name)
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

}
