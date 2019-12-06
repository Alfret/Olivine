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
#include "olivine/render/api/d3d12_util.hpp"
#include "olivine/render/api/buffer.hpp"

// ========================================================================== //
// ConstantBuffer Declaration
// ========================================================================== //

namespace olivine {

/** \class ConstantBuffer
 * \author Filip Björklund
 * \date 29 september 2019 - 14:30
 * \brief Constant buffer.
 * \details
 * Represents a constant buffer.
 */
class ConstantBuffer
{

public:
  /* Creation information */
  struct CreateInfo
  {
    /* Size of the vertex buffer */
    u64 size;
    /** Buffer alignment **/
    u64 alignment = Buffer::kDefaultAlign;
    /** Kind of memory heap to place buffer in **/
    HeapKind heapKind = HeapKind::kDefault;
  };

private:
  /* Buffer */
  Buffer mBuffer;

public:
  /** Construct a constant buffer from a creation information structure.
   * \brief Construct constant buffer.
   * \param createInfo Creation information.
   */
  explicit ConstantBuffer(const CreateInfo& createInfo);

  /** Construct a constant buffer from the specified size, heap-kind and
   * alignment.
   * \brief Construct constant buffer.
   * \param size Size in bytes.
   * \param heapKind Kind of memory heap to place buffer allocation in.
   * \param alignment Alignment of the buffer memory.
   */
  explicit ConstantBuffer(u64 size,
                          HeapKind heapKind = HeapKind::kDefault,
                          u64 alignment = Buffer::kDefaultAlign);

  /** Destruct the constant buffer.
   * \brief Destruct constant buffer.
   */
  ~ConstantBuffer() = default;

  /** Write data to the buffer.
   * \brief Write data.
   * \param data Data to write.
   * \param size Size of data to write in bytes.
   * \param offset Offset to write at
   */
  void Write(const u8* data, u64 size, u64 offset)
  {
    mBuffer.Write(data, size);
  }

  /** Write data to the buffer from an object of type T.
   * \tparam T Type of object.
   * \brief Write object data.
   * \param object Object to write.
   * \param offset Offset to write at
   */
  template<typename T>
  void Write(const T& object, u64 offset);

  /** Returns the generic buffer that represents the constant buffer.
   * \brief Returns buffer.
   * \return Buffer.
   */
  const Buffer& GetBuffer() const { return mBuffer; }

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

// ========================================================================== //
// ConstantBuffer Implementation
// ========================================================================== //

namespace olivine {

template<typename T>
void
ConstantBuffer::Write(const T& object, u64 offset)
{
  Write(reinterpret_cast<const u8*>(&object), sizeof(T), offset * sizeof(T));
}

}
