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

#include "olivine/render/api/index_buffer.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/core/assert.hpp"

// ========================================================================== //
// IndexBuffer Implementation
// ========================================================================== //

namespace olivine {

IndexBuffer::IndexBuffer(const CreateInfo& createInfo)
  : mBuffer(createInfo.count * GetFormatSize(createInfo.format),
            Buffer::Usage::kIndexBuffer,
            createInfo.heapKind,
            createInfo.alignment)
{
  D3D12_INDEX_BUFFER_VIEW view;
  view.BufferLocation = GetResource()->GetGPUVirtualAddress();
  view.SizeInBytes = static_cast<UINT>(mBuffer.GetSize());
  view.Format = ToFormat(createInfo.format);
  mView.handle = view;
}

// -------------------------------------------------------------------------- //

void
IndexBuffer::Write(u16* indices, u64 count)
{
  mBuffer.Write(reinterpret_cast<u8*>(indices), count * sizeof(u16));
}

// -------------------------------------------------------------------------- //

void
IndexBuffer::Write(u32* indices, u64 count)
{
  mBuffer.Write(reinterpret_cast<u8*>(indices), count * sizeof(u32));
}

// -------------------------------------------------------------------------- //

DXGI_FORMAT
IndexBuffer::ToFormat(Format format)
{
  switch (format) {
    case Format::kU16: {
      return DXGI_FORMAT_R16_UINT;
    }
    case Format::kU32: {
      return DXGI_FORMAT_R32_UINT;
    }
    default: {
      Panic("Invalid index buffer format");
    }
  }
}

// -------------------------------------------------------------------------- //

u32
IndexBuffer::GetFormatSize(Format format)
{
  switch (format) {
    case Format::kU16: {
      return sizeof(u16);
    }
    case Format::kU32: {
      return sizeof(u32);
    }
    default: {
      Panic("Invalid index buffer format");
    }
  }
}

}
