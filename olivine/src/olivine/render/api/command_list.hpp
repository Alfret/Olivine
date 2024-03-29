// MIT License
//
// Copyright (c) 2019 Filip Bj�rklund
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

#pragma once

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/render/api/d3d12_util.hpp"
#include "olivine/render/api/queue.hpp"

// ========================================================================== //
// CommandList Declaration
// ========================================================================== //

namespace olivine {

OL_FORWARD_DECLARE(Texture);
OL_FORWARD_DECLARE(Buffer);
OL_FORWARD_DECLARE(Descriptor);
OL_FORWARD_DECLARE(Color);
OL_FORWARD_DECLARE(Viewport);
OL_FORWARD_DECLARE(Rectangle);
OL_FORWARD_DECLARE(RootSignature);
OL_FORWARD_DECLARE(PipelineState);
OL_FORWARD_DECLARE(VertexBuffer);
OL_FORWARD_DECLARE(IndexBuffer);
OL_FORWARD_DECLARE(DescriptorHeap);
OL_FORWARD_DECLARE(ConstantBuffer);
OL_FORWARD_DECLARE_ENUM(PrimitiveTopology : u32);

/** \class CommandList
 * \author Filip Bj�rklund
 * \date 26 september 2019 - 12:07
 * \brief
 * \details
 * Represents a command list for recording commands that can later be
 * submitted for execution on the GPU.
 */
class CommandList
{
public:
  /* Maximum number of vertex buffers that can be bound with the same call */
  static constexpr u32 kMaxBoundVertexBuffer = 16;
  /* Maxmimum number of descriptor heaps that can be bounds at one time */
  static constexpr u32 kMaxBoundDescriptorHeaps = 2;

private:
  /* List handle */
  ID3D12GraphicsCommandList4* mHandle;
  /* Command allocator */
  ID3D12CommandAllocator* mAllocator;

  /* Kind of command queue that this list can be used with */
  CommandQueue::Kind mKind;

public:
  /** Create a command list for submitting commands to a command queue of the
   * kind specified.
   * \brief Create command list.
   * \param kind Kind of command queue that this command list will be used with.
   */
  explicit CommandList(CommandQueue::Kind kind);

  /** Destruct the command list.
   * \brief Destruct command list.
   */
  ~CommandList();

  /** Reset the command list to begin recording commands to it.
   * \note The user must make sure the list is not in use when this is called.
   * \brief Reset command list.
   */
  void Reset();

  /** Close the command list after recording commands to it. This will put the
   * command list into a state where it can be submitted for execution.
   * \brief Close command list.
   */
  void Close();

  /** Record a draw call for rendering the specified number of vertices from the
   * bound vertex buffer. The first vertex to read from the stream can also be
   * specified.
   * \brief Draw.
   * \param vertexCount Number of vertices to draw.
   * \param startVertex First vertex to draw.
   */
  void Draw(u32 vertexCount, u32 startVertex = 0);

  /** Record a draw call for rendering the specified number of indexed vertices
   * from the bound vertex and index buffers. The first instance and vertex to
   * draw can be specifed.
   * \brief Draw indexed.
   * \param indexCount Number of indices to draw with.
   * \param startIndex First index to draw.
   * \param startVertex First vertex to draw.
   */
  void DrawIndexed(u32 indexCount, u32 startIndex = 0, u32 startVertex = 0);

  /** Transition a resource between the states 'from' and 'to'.
   * \brief Transition resource state.
   * \param texture Texture to transition.
   * \param from State to transition from.
   * \param to State to transition to.
   */
  void TransitionResource(const Texture* texture,
                          ResourceState from,
                          ResourceState to);

  /** Copy data between the 'src' and 'dst' buffer at the specified offsets. The
   * number of bytes to copy are specified by 'size'.
   * \brief Copy between buffers.
   * \param dst Destination buffer.
   * \param src Source buffer.
   * \param size Number of bytes to copy. If size is zero (0) then the smallest
   * size of that of the two buffers are used.
   * \param dstOffset Offset in destination buffer to copy to.
   * \param srcOffset Offset in source buffer to copy from.
   */
  void Copy(Buffer* dst,
            Buffer* src,
            u64 size,
            u64 dstOffset = 0,
            u64 srcOffset = 0);

  /** Copy data from the 'src' buffer into the 'dst' texture. The buffer has
   * certain requirements for the layout of data that must be respected. These
   * requirement can be retrieved from the texture.
   * \brief Copy buffer to texture.
   * \param dst Destination texture.
   * \param src Source buffer.
   * \param srcOffset Offset in source buffer.
   */
  void Copy(Texture* dst, Buffer* src, u64 srcOffset = 0);

  /** Set the render target.
   * \brief Set render target.
   * \param descriptor Render-target view descriptor.
   */
  void SetRenderTarget(Descriptor descriptor);

  /** Clear a render target to the specified color.
   * \brief Clear render target.
   * \param descriptor Render-target view descriptor.
   * \param color Color to clear target to.
   */
  void ClearRenderTarget(Descriptor descriptor, Color color);

  /** Set the viewport.
   * \brief Set viewport.
   * \param viewport Viewport to set.
   */
  void SetViewport(Viewport viewport);

  /** Set the scissor rectangle.
   * \brief Set scissor rectangle.
   * \brief rectangle Scissor rectangle to set.
   */
  void SetScissorRectangle(Rectangle rectangle);

  /** Set the primitive topology for the following draw commands.
   * \brief Set primitive topology.
   * \param topology Primitive topology to set.
   */
  void SetPrimitiveTopology(PrimitiveTopology topology);

  /** Set the root signature for use with the graphics pipeline.
   * \brief Set graphics root signature.
   * \param rootSignature Root signature to set.
   */
  void SetRootSignatureGraphics(const RootSignature* rootSignature);

  /** Set the root signature for use with the compute pipeline.
   * \brief Set compute root signature.
   * \param rootSignature Root signature to set.
   */
  void SetRootSignatureCompute(const RootSignature* rootSignature);

  /** Set the pipeline state.
   * \brief Set pipeline state.
   * \param pipelineState Pipeline state to set.
   */
  void SetPipelineState(const PipelineState* pipelineState);

  /** Set the vertex buffer.
   * \brief Set vertex buffer.
   * \param vertexBuffer Vertex buffer to set.
   * \param slot Slot to bind vertex buffer at.
   */
  void SetVertexBuffer(const VertexBuffer* vertexBuffer, u32 slot = 0);

  /** Set multiple vertex buffers.
   * \brief If more than 'kMaxBoundVertexBuffer' vertex buffers must be set,
   * then multiple calls can be made with different starting slots.
   * \brief Set vertex buffers.
   * \param vertexBuffers Vertex buffers to set.
   * \param count Number of vertex buffer to set. Maximum value is
   * kMaxBoundVertexBuffer.
   * \param startSlot Starting slot to bind vertex buffers from.
   */
  void SetVertexBuffers(const VertexBuffer* const* vertexBuffers,
                        u32 count,
                        u32 startSlot = 0);

  /** Set the index buffer.
   * \brief Set index buffer.
   * \param indexBuffer Index buffer to set.
   */
  void SetIndexBuffer(const IndexBuffer* indexBuffer);

  /** Set the current descriptor heap. Only one sampler heap and non-sampler
   * heap can be bound at one time. This means that the function
   * 'SetDescriptorHeaps' must be used instead if the user must bind a sampler
   * aswell as a non-sampler heap at the same time.
   * \brief Set descriptor heap.
   */
  void SetDescriptorHeap(const DescriptorHeap* heap);

  /** Set multiple descriptor heaps. The maximum count is only two (2) as there
   * is only support for a single sampler heap and non-sampler heap bound at one
   * time.
   * \brief Set descriptor heaps.
   * \param heaps Heaps to set.
   * \param count Number of heaps to set. Maximum value is two (2).
   */
  void SetDescriptorHeaps(const DescriptorHeap* const* heaps, u32 count);

  /** Set a root CBV descriptor at the specified parameter index for the
   * graphics pipeline.
   * \brief Set root descriptor (CBV)
   * \param paramIndex Parameter index.
   * \param constantBuffer Constant buffer to set descriptor for.
   */
  void SetRootDescriptorGraphics(u32 paramIndex,
                                 ConstantBuffer* constantBuffer);

  /** Set a root descriptor table at the specified parmeter index for the
   * graphics pipeline.
   * \brief Set graphics root descriptor table.
   * \param paramIndex Parameter index.
   * \param baseDescriptor Base descriptor in the table (Handle to table start).
   */
  void SetRootDescriptorTableGraphics(u32 paramIndex,
                                      Descriptor baseDescriptor);

  /** Set a root descriptor table at the specified parmeter index for the
   * compute pipeline.
   * \brief Set compute root descriptor table.
   * \param paramIndex Parameter index.
   * \param baseDescriptor Base descriptor in the table (Handle to table start).
   */
  void SetRootDescriptorTableCompute(u32 paramIndex, Descriptor baseDescriptor);

  /** Returns the kind of the command list.
   * \brief Returns the kind.
   * \return Kind.
   */
  CommandQueue::Kind GetKind() const { return mKind; }

  /** Set the name of the command list.
   * \brief Set name.
   * \param name Name to set.
   */
  void SetName(const String& name);

  /** Returns the command list handle.
   * \brief Returns handle.
   * \return Handle.
   */
  ID3D12GraphicsCommandList* GetHandle() const { return mHandle; }
};

}