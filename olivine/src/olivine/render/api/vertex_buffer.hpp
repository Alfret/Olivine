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

#pragma once

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/render/api/buffer.hpp"

// ========================================================================== //
// VertexBuffer Declaration
// ========================================================================== //

namespace olivine {

/** \class VertexBuffer
 * \author Filip Björklund
 * \date 27 september 2019 - 16:25
 * \brief Vertex buffer.
 * \details
 * Represents a vertex buffer.
 */
class VertexBuffer
{
public:
  /* Vertex buffer view  */
  struct View
  {
    /* View handle */
    D3D12_VERTEX_BUFFER_VIEW handle;
  };

  /* Creation information */
  struct CreateInfo
  {
    /* Size of the vertex buffer */
    u64 size;
    /** Stride **/
    u32 stride = 0;
    /** Buffer alignment **/
    u64 alignment = Buffer::kDefaultAlign;
    /** Kind of memory heap to place buffer in **/
    HeapKind heapKind = HeapKind::kDefault;
  };

private:
  /* Buffer */
  Buffer mBuffer;

  /* View of the buffer */
  View mView;

public:
  /** Construct a vertex buffer from a creation information structure.
   * \brief Construct vertex buffer.
   * \param createInfo Creation information.
   */
  explicit VertexBuffer(const CreateInfo& createInfo);

  /** Construct a vertex buffer from the specified size, stride, heap-kind and
   * alignment.
   * \brief Construct vertex buffer.
   * \param size Size in bytes of the buffer.
   * \param stride Number of bytes between each consecutive vertex in the
   * buffer.
   * \param heapKind Kind of heap to place buffer allocation in.
   * \param alignment Alignment of the buffer memory.
   */
  VertexBuffer(u64 size,
               u32 stride,
               HeapKind heapKind = HeapKind::kDefault,
               u64 alignment = Buffer::kDefaultAlign);

  /** Destruct the vertex buffer.
   * \brief Destruct vertex buffer.
   */
  ~VertexBuffer() = default;

  /** Write data to the buffer.
   * \brief Write data.
   * \param data Data to write.
   * \param size Size of data to write in bytes.
   */
  void Write(u8* data, u64 size);

  /** Returns the generic buffer that represents the vertex buffer.
   * \brief Returns buffer.
   * \return Buffer.
   */
  Buffer& GetBuffer() { return mBuffer; }

  /** Returns the generic buffer that represents the vertex buffer.
   * \brief Returns buffer.
   * \return Buffer.
   */
  const Buffer& GetBuffer() const { return mBuffer; }

  /** Returns the view of the vertex buffer.
   * \brief Returns the view.
   * \return Vertex buffer view.
   */
  const View& GetView() const { return mView; }

  /** Set the name of the vertex buffer.
   * \brief Set name.
   * \param name Name to set.
   */
  void SetName(const String& name) { mBuffer.SetName(name); }

  /** Returns the resource that is backing the buffer.
   * \brief Returns resource.
   * \return Resource handle.
   */
  ID3D12Resource* GetResource() const { return mBuffer.GetResource(); }
};

}