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

// Standard headers
#include <atomic>
#include <mutex>
#include <list>

// Project headers
#include "olivine/math/limits.hpp"
#include "olivine/render/api/descriptor.hpp"

// ========================================================================== //
// DescriptorAllocator Declaration
// ========================================================================== //

namespace olivine {

/** \class DescriptorAllocator
 * \author Filip Björklund
 * \date 01 oktober 2019 - 12:54
 * \brief
 * \details
 * Represents a descriptor allocator. This object contains a descriptor heap for
 * a specific kind of descriptor and also a free-list to determine what
 * descriptors are not used.
 */
class DescriptorAllocator
{
private:
  /* Heap of descriptors */
  class Heap
  {
    OL_DEFAULT_MOVE(Heap);

  public:
    /* Invalid index value */
    static constexpr u16 kInvalidIndex = Limits::kU16Max;

  private:
    /* Descriptor heap */
    DescriptorHeap mHeap;
    /* List of free indices */
    u16* mIndices = nullptr;
    /* Capacity of index list */
    u16 mCapacity;
    /* Current list head index */
    u16 mHeadIndex = 0;

  public:
    /* Construct heap */
    Heap(Descriptor::Kind kind, u16 count, bool shaderVisible);

    /* Destruct heap */
    ~Heap();

    /* Allocate index */
    u16 Allocate();

    /* Free index */
    void Free(u16 index);

    /* Returns whether the heap has free space */
    bool HasEmptySpace() const { return mHeadIndex < mCapacity; }

    /* Returns the heap handle */
    const DescriptorHeap& GetHandle() const { return mHeap; }
  };

public:
  /* Default descriptor capacity */
  static constexpr u16 kDefaultCapacity = 1024;
  /* Invalid handle value */
  static constexpr u32 kInvalidHandle = Limits::kU32Max;

private:
  /* Descriptor heaps */
  std::list<Heap> mHeaps;
  // TODO(Filip Björklund): We don't really need this, as the only usage is in
  //  asserts and due to the nature of never deleting heaps it will most likely
  //  never fail
  /* Heap count */
  std::atomic<u32> mHeapCount = 0;
  /* Mutex */
  mutable std::mutex mMutex;

  /* Descriptor kind */
  Descriptor::Kind mKind;
  /* Capacity */
  u16 mCapacity;
  /* Whether descriptors are shader visible */
  bool mShaderVisible;

public:
  /** Construct a descriptor allocator for a specific kind of descriptor.
   * \brief Construct free-list descriptor allocator.
   * \param kind Kind of descriptor to create for.
   * \param shaderVisible Whether the descriptors allocated should be
   * shader-visible.
   * \param perHeapCapacity Initial descriptor capacity for each heap.
   */
  explicit DescriptorAllocator(Descriptor::Kind kind,
                               bool shaderVisible = false,
                               u16 perHeapCapacity = kDefaultCapacity);

  /** Default destructor **/
  ~DescriptorAllocator() = default;

  /** Allocate a descriptor from the allocator heaps.
   * \brief Allocate descriptor.
   * \return Allocated descriptor index.
   */
  u32 Allocate();

  /** Free an allocated descriptor from the allocator heaps.
   *
   */
  void Free(u32 handle);

  /**
   *
   */
  Descriptor GetHandle(u32 handle) const;

  /**
   *
   */
  const DescriptorHeap& GetHeap(u32 handle) const;

private:
  /* Add one more heap to the allocator */
  void AddHeap();
};

}

// ========================================================================== //
// DescriptorStack Declaration
// ========================================================================== //

namespace olivine {}