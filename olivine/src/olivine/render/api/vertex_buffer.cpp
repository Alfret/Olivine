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

#include "olivine/render/api/vertex_buffer.hpp"

// ========================================================================== //
// VertexBuffer Implementation
// ========================================================================== //

namespace olivine {

VertexBuffer::VertexBuffer(const CreateInfo& createInfo)
  : mBuffer(createInfo.size,
            Buffer::Usage::kVertexBuffer,
            createInfo.heapKind,
            createInfo.alignment)
{
  D3D12_VERTEX_BUFFER_VIEW view;
  view.BufferLocation = GetResource()->GetGPUVirtualAddress();
  view.SizeInBytes = static_cast<UINT>(createInfo.size);
  view.StrideInBytes = createInfo.stride;
  mView.handle = view;
}

// -------------------------------------------------------------------------- //

void
VertexBuffer::Write(u8* data, u64 size)
{
  mBuffer.Write(data, size);
}

}
