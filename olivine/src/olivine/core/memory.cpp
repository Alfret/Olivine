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

#include "olivine/core/memory.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Standard headers
#include <new>

// Project headers
#include "olivine/core/assert.hpp"

// Library headers
#include <mimalloc/mimalloc.h>

// ========================================================================== //
// MemTracker Implementation
// ========================================================================== //

namespace olivine {

MemTracker::~MemTracker()
{
  // OL_ASSERT(mUsage == 0, "Leaking memory");
}

// -------------------------------------------------------------------------- //

MemTracker&
MemTracker::Instance()
{
  static MemTracker tracker;
  return tracker;
}

}

// ========================================================================== //
// Memory Implementation
// ========================================================================== //

namespace olivine {

void*
Memory::Allocate(u64 size, u64 alignment)
{
  alignment = Max(alignment, MIN_ALIGN);
  if (size == 0) {
    return nullptr;
  }
  void* pointer = mi_malloc_aligned(size, alignment);
  MemTracker::Instance().Usage() += mi_usable_size(pointer);
  return pointer;
}

// -------------------------------------------------------------------------- //

void*
Memory::Rellocate(void* pointer, u64 size, u64 alignment)
{
  if (!pointer) {
    return Allocate(size, alignment);
  }
  MemTracker::Instance().Usage() -= mi_usable_size(pointer);
  void* _pointer = mi_realloc_aligned(pointer, size, alignment);
  MemTracker::Instance().Usage() += mi_usable_size(_pointer);
  return _pointer;
}

// -------------------------------------------------------------------------- //

void
Memory::Free(void* pointer)
{
  MemTracker::Instance().Usage() -= mi_usable_size(pointer);
  mi_free(pointer);
}

// -------------------------------------------------------------------------- //

u64
Memory::GetAllocationSize(void* pointer)
{
  return mi_usable_size(pointer);
}

// -------------------------------------------------------------------------- //

void*
Memory::Copy(void* destination, const void* source, u64 size)
{
  return memcpy(destination, source, size);
}

// -------------------------------------------------------------------------- //

void*
Memory::Clear(void* memory, u64 size)
{
  return memset(memory, 0, size);
}

}

// ========================================================================== //
// Global Functions
// ========================================================================== //

void*
operator new(std::size_t size)
{
  return olivine::Memory::Allocate(size, olivine::Memory::MIN_ALIGN);
}

// -------------------------------------------------------------------------- //

void*
operator new[](size_t size)
{
  return olivine::Memory::Allocate(size, olivine::Memory::MIN_ALIGN);
}

// --------------------------------------------------------------------------
// //

void
operator delete(void* pointer) noexcept
{
  olivine::Memory::Free(pointer);
}

// -------------------------------------------------------------------------- //

void
operator delete[](void* pointer) noexcept
{
  olivine::Memory::Free(pointer);
}