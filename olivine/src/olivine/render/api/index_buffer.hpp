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
// IndexBuffer Declaration
// ========================================================================== //

namespace olivine {

/** \class IndexBuffer
 * \author Filip Björklund
 * \date 28 september 2019 - 10:36
 * \brief Index buffer.
 * \details
 * Represents an index buffer.
 */
class IndexBuffer
{
public:
  /* Buffer view  */
  struct View
  {
    /* View handle */
    D3D12_INDEX_BUFFER_VIEW handle;
  };

  /* Formats for the integer data */
  enum class Format
  {
    /* 16-bit unsigned integer */
    kU16,
    /* 32-bit unsigned integer */
    kU32
  };

  /* Creation information */
  struct CreateInfo
  {
    /* Number of indices */
    u64 count;
    /* Index formt */
    Format format;
    /* Buffer alignment */
    u32 alignment = Buffer::kDefaultAlign;
    /* Kind of memory heap to place buffer in */
    HeapKind heapKind = HeapKind::kDefault;
  };

private:
  /* Buffer */
  Buffer mBuffer;

  /* View of the buffer */
  View mView;

public:
  /** Construct a index buffer from a creation information structure.
   * \brief Construct index buffer.
   * \param createInfo Creation information.
   */
  explicit IndexBuffer(const CreateInfo& createInfo);

  /** Destruct the index buffer.
   * \brief Destruct index buffer.
   */
  ~IndexBuffer() = default;

  /** Write indices to the buffer.
   * \brief Write indices.
   * \param indices Indices to write.
   * \param count Number of indices to write.
   */
  void Write(u16* indices, u64 count);

  /** Write indices to the buffer.
   * \brief Write indices.
   * \param indices Indices to write.
   * \param count Number of indices to write.
   */
  void Write(u32* indices, u64 count);

  /** Returns the generic buffer that represents the index buffer.
   * \brief Returns buffer.
   * \return Buffer.
   */
  const Buffer& GetBuffer() const { return mBuffer; }

  /** Returns the view of the index buffer.
   * \brief Returns the view.
   * \return Index buffer view.
   */
  const View& GetView() const { return mView; }

  /** Set the name of the index buffer.
   * \brief Set name.
   * \param name Name to set.
   */
  void SetName(const String& name) { mBuffer.SetName(name); }

  /** Returns the resource that is backing the buffer.
   * \brief Returns resource.
   * \return Resource handle.
   */
  ID3D12Resource* GetResource() const { return mBuffer.GetResource(); }

private:
  /** Convert 'Format' to 'DXGI_FORMAT' **/
  static DXGI_FORMAT ToFormat(Format format);

  /** Returns the byte size of a format **/
  static u32 GetFormatSize(Format format);
};

}