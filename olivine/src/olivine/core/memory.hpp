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

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/core/types.hpp"
#include "olivine/core/macros.hpp"
#include "olivine/core/traits.hpp"
#include "olivine/math/math.hpp"

// ========================================================================== //
// MemTracker Declaration
// ========================================================================== //

namespace olivine {

/** Memory tracker **/
class MemTracker
{
private:
  /** Current usage **/
  u64 mUsage;

public:
  /** Returns the current usage **/
  u64& Usage() { return mUsage; }

  /** Returns the current usage **/
  OL_NODISCARD const u64& Usage() const { return mUsage; }

private:
  /** Construct memory tracker **/
  MemTracker() = default;

  /** Destruct and check leaks **/
  ~MemTracker();

public:
  /** Global instance **/
  static MemTracker& Instance();
};

}

// ========================================================================== //
// Memory Declaration
// ========================================================================== //

namespace olivine {

/** Memory utilities **/
class Memory
{
  OL_NAMESPACE_CLASS(Memory);

public:
  /** Minimum alignment **/
  static constexpr u64 MIN_ALIGN = alignof(void*);

public:
  /** Allocate memory **/
  static void* Allocate(u64 size, u64 alignment = MIN_ALIGN);

  /** Reallocate memory **/
  static void* Rellocate(void* pointer, u64 size, u64 alignment);

  /** Free memory **/
  static void Free(void* pointer);

  /** Returns the size that an allocation takes up in memory **/
  static u64 GetAllocationSize(void* pointer);

  /** Returns the required alignment of the type 'T' **/
  template<typename T>
  static constexpr u64 Alignof()
  {
    return Max(alignof(T), MIN_ALIGN);
  }

  /** Copy memory of the given size from the the source to the destination
   * address **/
  static void* Copy(void* destination, const void* source, u64 size);

  /** Clear the memory of the specified size to all 0 **/
  static void* Clear(void* memory, u64 size);

  /** Relocate memory from the source memory location to the destination
   * location **/
  template<typename T>
  static T* Relocate(T* destination, T* source)
  {
    if constexpr (!IsTriviallyRelocatable<T>::Value) {
      ::new (destination) T{ std::move(*source) };
      source->~T();
    } else {
      Copy(destination, source, sizeof(T));
    }
    return destination;
  }
};

}
