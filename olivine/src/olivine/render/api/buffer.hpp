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
#include "olivine/core/common.hpp"
#include "olivine/render/api/common.hpp"
#include "olivine/render/api/d3d12_util.hpp"

// ========================================================================== //
// Buffer Declaration
// ========================================================================== //

namespace olivine {

/** \class Buffer
 * \author Filip Björklund
 * \date 27 september 2019 - 15:57
 * \brief Buffer.
 * \details
 * Represents a buffer.
 */
class Buffer
{
public:
  /* Default buffer memory alignment */
  static constexpr u32 kDefaultAlign =
    D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;

public:
  /* Buffer usages */
  enum class Usage : u32
  {
    /* No specific usage */
    kNone = 0,
    /* Constant buffer */
    kConstantBuffer = Bit(0u),
    /* Unordered access */
    kUnorderedAccess = Bit(1u),
    /* Vertex buffer */
    kVertexBuffer = Bit(2u),
    /* Index buffer */
    kIndexBuffer = Bit(3u)
  };
  OL_ENUM_CLASS_OPERATORS(friend, Usage, u32);

  /** Creation information **/
  struct CreateInfo
  {
    /** Size in bytes**/
    u64 size = 0;
    /** Alignment in bytes. Must be a power of two **/
    u64 alignment = kDefaultAlign;
    /** Usages for buffer **/
    Usage usages = Usage::kNone;
    /** Kind of memory to place buffer in **/
    HeapKind heapKind = HeapKind::kDefault;
  };

private:
  /* Buffer resource handle */
  ID3D12Resource* mResource = nullptr;
  /* Allocation */
  D3D12MA::Allocation* mAllocation = nullptr;

  /* Size in bytes */
  u64 mSize;
  /* Alignment in bytes */
  u64 mAlignment;

public:
  /** Construct a buffer from a creation information structure.
   * \brief Construct buffer.
   * \param createInfo Creation information.
   */
  explicit Buffer(const CreateInfo& createInfo);

  /** Construct a buffer with the specified 'size', 'usages', 'heapKind' and
   * 'alignment'.
   * \brief Construct buffer.
   * \param size Size of buffer in bytes.
   * \param usages Usages for buffer.
   * \param heapKind Kind of memory heap to place buffer allocation in.
   * \param alignment Alignment of buffer memory.
   */
  Buffer(u64 size,
         Usage usages,
         HeapKind heapKind = HeapKind::kDefault,
         u64 alignment = kDefaultAlign);

  /** Destruct the buffer.
   * \brief Destruct buffer.
   */
  ~Buffer();

  /** Map the buffer to a CPU accessible memory range.
   * \note The user should call 'Unmap' when the buffer should be unmapped.
   * However persistant mapping is supported.
   * \brief Map buffer.
   * \return Pointer to mapped buffer memory.
   */
  u8* Map();

  /** Unmap buffer that was previously mapped to a CPU accessible memory range.
   * \note This expects to be called after a call to 'Map'.
   * \brief Unmap buffer.
   */
  void Unmap();

  /** Write data to the buffer from the CPU. This call will internally map the
   * buffer, write the data to it, and then unmap again.
   * \brief Write data to buffer.
   * \param data Data to write.
   * \param size Size of data to write, in bytes.
   * \param offset Offset to write at.
   */
  void Write(const u8* data, u64 size, u64 offset = 0);

  /** Returns the size of the buffer in bytes.
   * \brief Returns size.
   * \return Size in bytes.
   */
  u64 GetSize() const { return mSize; }

  /** Returns the alignment of the buffer in bytes.
   * \brief Returns alignment.
   * \return Alignment in bytes.
   */
  u64 GetAlignment() const { return mAlignment; }

  /** Set the name of the buffer.
   * \brief Set name.
   * \param name Name to set.
   */
  void SetName(const String& name);

  /** Returns the handle to the resource backing the buffer.
   * \brief Returns buffer backing resource.
   * \return Resource.
   */
  ID3D12Resource* GetResource() const { return mResource; }
};

}