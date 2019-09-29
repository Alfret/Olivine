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

#include "olivine/render/api/command_list.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/app/app.hpp"
#include "olivine/render/api/device.hpp"
#include "olivine/render/api/texture.hpp"
#include "olivine/render/api/descriptor.hpp"
#include "olivine/render/api/root_signature.hpp"
#include "olivine/render/api/pipeline_state.hpp"
#include "olivine/render/api/vertex_buffer.hpp"
#include "olivine/render/api/index_buffer.hpp"
#include "olivine/render/api/constant_buffer.hpp"
#include "olivine/render/color.hpp"

// ========================================================================== //
// CommandList Implementation
// ========================================================================== //

namespace olivine {

CommandList::CommandList(CommandQueue::Kind kind)
  : mKind(kind)
{
  Device* device = App::Instance()->GetDevice();

  // Create allocator
  HRESULT hresult = device->GetHandle()->CreateCommandAllocator(
    CommandQueue::ToCommandListType(kind),
    __uuidof(ID3D12CommandAllocator),
    (void**)&mAllocator);
  Assert(SUCCEEDED(hresult), "Failed to create command list allocator");

  // Create list
  hresult = device->GetHandle()->CreateCommandList(
    0x00,
    CommandQueue::ToCommandListType(kind),
    mAllocator,
    nullptr,
    __uuidof(ID3D12GraphicsCommandList4),
    reinterpret_cast<void**>(&mHandle));
  Assert(SUCCEEDED(hresult), "Failed to create command list");
  hresult = mHandle->Close();
  Assert(SUCCEEDED(hresult), "Failed to create command list");
}

// -------------------------------------------------------------------------- //

CommandList::~CommandList()
{
  // Release list handle
  mHandle->Release();
  mHandle = nullptr;

  // Release allocator
  mAllocator->Release();
  mAllocator = nullptr;
}

// -------------------------------------------------------------------------- //

void
CommandList::Reset()
{
  HRESULT hresult = mAllocator->Reset();
  Assert(SUCCEEDED(hresult), "Failed to reset command list allocator");
  hresult = mHandle->Reset(mAllocator, nullptr);
  Assert(SUCCEEDED(hresult), "Failed to reset command list");
}

// -------------------------------------------------------------------------- //

void
CommandList::Close()
{
  HRESULT hresult = mHandle->Close();
  Assert(SUCCEEDED(hresult), "Failed to close command list");
}

// -------------------------------------------------------------------------- //

void
CommandList::Draw(u32 vertexCount, u32 startVertex)
{
  mHandle->DrawInstanced(vertexCount, 1, startVertex, 0);
}

// -------------------------------------------------------------------------- //

void
CommandList::DrawIndexed(u32 indexCount, u32 startIndex, u32 startVertex)
{
  mHandle->DrawIndexedInstanced(indexCount, 1, startIndex, startVertex, 0);
}

// -------------------------------------------------------------------------- //

void
CommandList::TransitionResource(const Texture* texture,
                                ResourceState from,
                                ResourceState to)
{
  D3D12_RESOURCE_BARRIER barrier;
  barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
  barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
  barrier.Transition.pResource = texture->GetResource();
  barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
  barrier.Transition.StateBefore = D3D12Util::ToResourceStates(from);
  barrier.Transition.StateAfter = D3D12Util::ToResourceStates(to);
  mHandle->ResourceBarrier(1, &barrier);
}

// -------------------------------------------------------------------------- //

void
CommandList::Copy(Buffer* dst,
                  Buffer* src,
                  u64 size,
                  u64 dstOffset,
                  u64 srcOffset)
{
  // Determine size
  if (size == 0) {
    size = Min(dst->GetSize(), src->GetSize());
  }

  // Copy data
  mHandle->CopyBufferRegion(
    dst->GetResource(), dstOffset, src->GetResource(), srcOffset, size);
}

// -------------------------------------------------------------------------- //

void
CommandList::Copy(Texture* dst, Buffer* src, u64 srcOffset)
{
  Device* device = App::Instance()->GetDevice();

  // Retrieve resource desc
  ID3D12Resource* resource = dst->GetResource();
  D3D12_RESOURCE_DESC resourceDesc = resource->GetDesc();

  // Get the copyable footprint
  D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
  UINT64 rowSizeInBytes;
  UINT numRows;
  UINT64 totalBytes;
  device->GetHandle()->GetCopyableFootprints(
    &resourceDesc, 0, 1, 0, &footprint, &numRows, &rowSizeInBytes, &totalBytes);

  // Copy texture regions
  D3D12_TEXTURE_COPY_LOCATION _dst, _src;
  _dst.pResource = dst->GetResource();
  _dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
  _dst.SubresourceIndex = 0;
  _src.pResource = src->GetResource();
  _src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
  _src.PlacedFootprint.Offset = srcOffset;
  _src.PlacedFootprint = footprint;
  mHandle->CopyTextureRegion(&_dst, 0, 0, 0, &_src, nullptr);
}

// -------------------------------------------------------------------------- //

void
CommandList::SetRenderTarget(Descriptor descriptor)
{
  D3D12_CPU_DESCRIPTOR_HANDLE handle = descriptor.CPU();
  mHandle->OMSetRenderTargets(1, &handle, false, nullptr);
}

// -------------------------------------------------------------------------- //

void
CommandList::ClearRenderTarget(Descriptor descriptor, Color color)
{
  // Clear render target view
  FLOAT channels[4] = { color.GetRedF32(),
                        color.GetGreenF32(),
                        color.GetBlueF32(),
                        color.GetAlphaF32() };
  mHandle->ClearRenderTargetView(descriptor.CPU(), channels, 0, nullptr);
}

// -------------------------------------------------------------------------- //

void
CommandList::SetViewport(Viewport viewport)
{
  D3D12_VIEWPORT _viewport;
  _viewport.Width = viewport.width;
  _viewport.Height = viewport.height;
  _viewport.TopLeftX = viewport.topLeftX;
  _viewport.TopLeftY = viewport.topLeftY;
  _viewport.MinDepth = viewport.minDepth;
  _viewport.MaxDepth = viewport.maxDepth;
  mHandle->RSSetViewports(1, &_viewport);
}

// -------------------------------------------------------------------------- //

void
CommandList::SetScissorRectangle(Rectangle rectangle)
{
  D3D12_RECT rect;
  rect.top = rectangle.top;
  rect.bottom = rectangle.bottom;
  rect.left = rectangle.left;
  rect.right = rectangle.right;
  mHandle->RSSetScissorRects(1, &rect);
}

// -------------------------------------------------------------------------- //

void
CommandList::SetPrimitiveTopology(PrimitiveTopology topology)
{
  mHandle->IASetPrimitiveTopology(D3D12Util::ToPrimitiveTopology(topology));
}

// -------------------------------------------------------------------------- //

void
CommandList::SetRootSignatureGraphics(const RootSignature* rootSignature)
{
  mHandle->SetGraphicsRootSignature(rootSignature->GetHandle());
}

// -------------------------------------------------------------------------- //

void
CommandList::SetRootSignatureCompute(const RootSignature* rootSignature)
{
  mHandle->SetComputeRootSignature(rootSignature->GetHandle());
}

// -------------------------------------------------------------------------- //

void
CommandList::SetPipelineState(const PipelineState* pipelineState)
{
  mHandle->SetPipelineState(pipelineState->GetHandle());
}

// -------------------------------------------------------------------------- //

void
CommandList::SetVertexBuffer(const VertexBuffer* vertexBuffer, u32 slot)
{
  D3D12_VERTEX_BUFFER_VIEW views[] = { vertexBuffer->GetView().handle };
  mHandle->IASetVertexBuffers(UINT(slot), 1, views);
}

// -------------------------------------------------------------------------- //

void
CommandList::SetVertexBuffers(const VertexBuffer* const* vertexBuffers,
                              u32 count,
                              u32 startSlot)
{
  Assert(count <= kMaxBoundVertexBuffer, "Too many vertex buffers specified");
  D3D12_VERTEX_BUFFER_VIEW views[kMaxBoundVertexBuffer];
  for (u32 i = 0; i < count; i++) {
    views[i] = vertexBuffers[i]->GetView().handle;
  }
  mHandle->IASetVertexBuffers(UINT(startSlot), UINT(count), views);
}

// -------------------------------------------------------------------------- //

void
CommandList::SetIndexBuffer(const IndexBuffer* indexBuffer)
{
  mHandle->IASetIndexBuffer(&indexBuffer->GetView().handle);
}

// -------------------------------------------------------------------------- //

void
CommandList::SetDescriptorHeap(const DescriptorHeap* heap)
{
  ID3D12DescriptorHeap* _heap[1] = { heap->GetHandle() };
  mHandle->SetDescriptorHeaps(1, _heap);
}

// -------------------------------------------------------------------------- //

void
CommandList::SetDescriptorHeaps(const DescriptorHeap* const* heaps, u32 count)
{
  Assert(count <= kMaxBoundDescriptorHeaps,
         "At most two (2) descriptor heaps can be bound at one time");
  ID3D12DescriptorHeap* _heaps[kMaxBoundDescriptorHeaps];
  for (u32 i = 0; i < count; i++) {
    _heaps[i] = heaps[i]->GetHandle();
  }
  mHandle->SetDescriptorHeaps(count, _heaps);
}

// -------------------------------------------------------------------------- //

void
CommandList::SetRootDescriptorGraphics(u32 paramIndex,
                                       ConstantBuffer* constantBuffer)
{
  mHandle->SetGraphicsRootConstantBufferView(
    paramIndex, constantBuffer->GetResource()->GetGPUVirtualAddress());
}

// -------------------------------------------------------------------------- //

void
CommandList::SetRootDescriptorTableGraphics(u32 paramIndex,
                                            Descriptor baseDescriptor)
{
  mHandle->SetGraphicsRootDescriptorTable(paramIndex, baseDescriptor.GPU());
}

// -------------------------------------------------------------------------- //

void
CommandList::SetRootDescriptorTableCompute(u32 paramIndex,
                                           Descriptor baseDescriptor)
{
  mHandle->SetComputeRootDescriptorTable(paramIndex, baseDescriptor.GPU());
}

// -------------------------------------------------------------------------- //

void
CommandList::SetName(const String& name)
{
  D3D12Util::SetName(mHandle, name);
  D3D12Util::SetName(mAllocator, name + "Allocator");
}

}
