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

#include "olivine/render/api/descriptor_allocator.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/core/assert.hpp"
#include "olivine/math/limits.hpp"

// ========================================================================== //
// Macros
// ========================================================================== //

/* Macro for retrieveing the heap of a descriptor allocator handle */
#define OL_DESCRIPTOR_ALLOCATOR_HANDLE_HEAP(handle)                            \
  u16((handle >> 16ul) & 0xFFFFul)

// -------------------------------------------------------------------------- //

/* Macro for retrieveing the index of a descriptor allocator handle */
#define OL_DESCRIPTOR_ALLOCATOR_HANDLE_INDEX(handle) u16(handle & 0xFFFFul)

// -------------------------------------------------------------------------- //

/* Macro for creating a descriptor allocator handle from heap and index */
#define OL_DESCRIPTOR_ALLOCATOR_MAKE_HANDLE(heap, index)                       \
  u32(((heap & 0xFFFFul) << 16ul) | (index & 0xFFFFul))

// ========================================================================== //
// DescriptorAllocator Implementation
// ========================================================================== //

namespace olivine {

DescriptorAllocator::Heap::Heap(Descriptor::Kind kind,
                                u16 count,
                                bool shaderVisible)
  : mHeap(kind, u32(count), shaderVisible)
  , mCapacity(count)
{
  Assert(
    count < Limits::kU16Max,
    "Capacity of descriptor allocator heap must not equal 'Limits::kU16Max', "
    "as this special values signifies an invalid handle");

  // Setup list with all indices free
  mIndices = new u16[mCapacity];
  for (u16 i = 0; i < mCapacity; i++) {
    mIndices[i] = i;
  }
}

// -------------------------------------------------------------------------- //

DescriptorAllocator::Heap::~Heap()
{
  delete mIndices;
  mIndices = nullptr;
}

// -------------------------------------------------------------------------- //

u16
DescriptorAllocator::Heap::Allocate()
{
  Assert(HasEmptySpace(),
         "Cannot allocate from full descriptor allocator heap");
  return mIndices[mHeadIndex++];
}

// -------------------------------------------------------------------------- //

void
DescriptorAllocator::Heap::Free(u16 index)
{
  Assert(mHeadIndex > 0 && index != Limits::kU16Max,
         "Cannot free invalid descriptor from descriptor allocator heap");
  mIndices[--mHeadIndex] = index;
}

// -------------------------------------------------------------------------- //

DescriptorAllocator::DescriptorAllocator(Descriptor::Kind kind,
                                         bool shaderVisible,
                                         u16 perHeapCapacity)
  : mKind(kind)
  , mCapacity(perHeapCapacity)
  , mShaderVisible(shaderVisible)
{
  AddHeap();
}

// -------------------------------------------------------------------------- //

u32
DescriptorAllocator::Allocate()
{
  std::lock_guard lock(mMutex);
  u16 index = Heap::kInvalidIndex;
  u16 heapIndex = 0;
  for (Heap& heap : mHeaps) {
    if (heap.HasEmptySpace()) {
      index = heap.Allocate();
      break;
    }
    heapIndex++;
  }

  // If we have an invalid index then allocate new heap
  if (index == Heap::kInvalidIndex) {
    AddHeap();
    index = mHeaps.back().Allocate();
  }

  Assert(index != Heap::kInvalidIndex, "Failed to allocate descriptor");
  return OL_DESCRIPTOR_ALLOCATOR_MAKE_HANDLE(heapIndex, index);
}

// -------------------------------------------------------------------------- //

void
DescriptorAllocator::Free(u32 handle)
{
  Assert(handle != kInvalidHandle, "Cannot free invalid handle");
  const u16 index = OL_DESCRIPTOR_ALLOCATOR_HANDLE_INDEX(handle);
  u16 heapIndex = OL_DESCRIPTOR_ALLOCATOR_HANDLE_HEAP(handle);
  Assert(heapIndex < mHeapCount.load(), "Invalid descriptor allocator handle");

  std::lock_guard lock(mMutex);
  for (Heap& heap : mHeaps) {
    if (heapIndex-- == 0) {
      heap.Free(index);
    }
  }
}

// -------------------------------------------------------------------------- //

Descriptor
DescriptorAllocator::GetHandle(u32 handle) const
{
  Assert(handle != kInvalidHandle, "Cannot get descriptor for invalid handle");
  return GetHeap(handle).At(OL_DESCRIPTOR_ALLOCATOR_HANDLE_INDEX(handle));
}

// -------------------------------------------------------------------------- //

const DescriptorHeap&
DescriptorAllocator::GetHeap(u32 handle) const
{
  Assert(handle != kInvalidHandle,
         "Cannot get descriptor heap for invalid handle");
  u16 heapIndex = OL_DESCRIPTOR_ALLOCATOR_HANDLE_HEAP(handle);
  Assert(heapIndex < mHeapCount.load(), "Invalid descriptor allocator handle");

  std::lock_guard lock(mMutex);
  for (const Heap& heap : mHeaps) {
    if (heapIndex-- == 0) {
      return heap.GetHandle();
    }
  }
  Panic("Descriptor handle is not valid");
}

// -------------------------------------------------------------------------- //

void
DescriptorAllocator::AddHeap()
{
  mHeaps.emplace_back(mKind, mCapacity, mShaderVisible);
  ++mHeapCount;
}

}
