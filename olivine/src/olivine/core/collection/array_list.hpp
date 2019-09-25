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
#include <new>
#include <initializer_list>
#include <utility>
#include <functional>

// Project headers
#include "olivine/core/types.hpp"
#include "olivine/core/assert.hpp"
#include "olivine/core/traits.hpp"
#include "olivine/core/memory.hpp"
#include "olivine/math/math.hpp"

// ========================================================================== //
// ArrayList Declaration
// ========================================================================== //

namespace olivine {

template<typename T, bool R>
class ArrayList;

// -------------------------------------------------------------------------- //

/** \copydoc olivine::IsTriviallyRelocatable<T> **/
template<typename T, bool R>
struct IsTriviallyRelocatable<ArrayList<T, R>>
{
  static constexpr bool Value = IsTriviallyRelocatable<T>::Value;
};

// -------------------------------------------------------------------------- //

/** \class ArrayList
 * \author Filip Björklund
 * \date 07 june 2019 - 21:32
 * \tparam T Type of objects in list.
 * \tparam R Whether the type is trivially relocatable.
 * \brief Array-list
 * \details
 * Represents an array-list where each object in the list is laid out linearly
 * in memory.
 */
template<typename T, bool R = IsTriviallyRelocatable<T>::Value>
class ArrayList
{
public:
  /** Type of the size and index data **/
  using SizeType = u64;
  /** Pointer type **/
  using PointerType = T*;
  /** Const pointer type **/
  using ConstPointerType = T const*;
  /** Reference type **/
  using ReferenceType = T&;

  /** Default array-list capacity **/
  static constexpr SizeType DEFAULT_CAPACITY = 10;
  /** Size of an object in the list **/
  static constexpr SizeType OBJECT_SIZE = sizeof(T);
  /** Factor of resize **/
  static constexpr SizeType RESIZE_FACTOR = 2;

public:
  /** Array-list iterator **/
  class Iterator
  {
  private:
    /** Pointer to data**/
    PointerType mPointer;

  public:
    /** Construct iterator **/
    Iterator(PointerType pointer);

    /** Next element **/
    void operator++();

    /** Previous element **/
    void operator--();

    /** Check inequality **/
    bool operator!=(const Iterator& other);

    /** Retrieve reference **/
    ReferenceType operator*();

    /** Retrieve pointer **/
    PointerType operator->();
  };

private:
  /** Data buffer**/
  T* mBuffer;
  /** Capacity of the buffer **/
  SizeType mCapacity;
  /** Number of elements in the list currently **/
  SizeType mSize;

public:
  /** Construct an array-list with the specified capacity and allocator.
   * \brief Construct array-list.
   * \param capacity Initial capacity of the array-list.
   */
  explicit ArrayList(u64 capacity = DEFAULT_CAPACITY);

  /** Construct an array-list from an initializer list.
   * \brief Construct array-list.
   * \param initializerList Initializer list to initialize the array-list from.
   */
  ArrayList(std::initializer_list<T> initializerList);

  /** Copy-constructor **/
  ArrayList(const ArrayList& other);

  /** Move-constructor **/
  ArrayList(ArrayList&& other) noexcept;

  /** Destructor **/
  ~ArrayList();

  /** Copy-assignment **/
  ArrayList& operator=(const ArrayList& other);

  /** Move-assignment **/
  ArrayList& operator=(ArrayList&& other) noexcept;

  /** Append an object to the end of the list.
   * \brief Append object.
   * \param object Object to append.
   */
  void Append(const T& object);

  /** Append an object to the end of the list.
   * \brief Append object.
   * \param object Object to append.
   */
  void Append(T&& object);

  /** Append an object to the end of the list. The object is created in-place in
   * the list from the specified arguments forwarded to its constructor. A
   * reference to the inserted object is also returned from the function.
   * \brief Append object.
   * \tparam ARGS Types of arguments to object constructor.
   * \param arguments Arguments to object constructor.
   * \return Reference to the inserted object.
   */
  template<typename... ARGS>
  T& AppendEmplace(ARGS&&... arguments);

  /** Prepend an object to the beginning of the list.
   * \brief Prepend object.
   * \param object Object to prepend.
   */
  void Prepend(const T& object);

  /** Prepend an object to the beginning of the list.
   * \brief Prepend object.
   * \param object Object to prepend.
   */
  void Prepend(T&& object);

  /** Prepend an object to the beginning of the list. The object is created
   * in-place in the list from the specified arguments forwarded to its
   * constructor. A reference to the inserted object is also returned from the
   * function.
   * \brief Prepend object.
   * \tparam ARGS Types of arguments to object constructor.
   * \param arguments Arguments to object constructor.
   * \return Reference to the inserted object.
   */
  template<typename... ARGS>
  T& PrependEmplace(ARGS&&... arguments);

  /** Remove an object from the list at the specified index.
   * \brief Remove object.
   * \param index Index of object.
   */
  void Remove(SizeType index);

  /** Remove an object from that list that is equal to the specified object.
   * \brief Remove object.
   * \param object Object to search for and remove.
   */
  void RemoveObject(const T& object);

  /** Remove an object from that list that is equal to the specified object.
   * \brief Remove object.
   * \param object Object to search for and remove.
   */
  void RemoveObject(T&& object);

  /** Resize the list to the specified size. This is useful if the user is using
   * the index operator to access objects instead of inserting them.
   * \note If the size is less than the current size then any objects outside
   * the new range (0 - size) will be destructed.
   * \brief Resize list.
   * \param size Size to resize to.
   */
  void Resize(SizeType size);

  /** Reserve capacity in the list for the specified number of objects. If the
   * capacity of the list is already greater than the new capacity then nothing
   * will happen.
   * \brief Reserve capacity.
   * \param capacity Capacity to reserve.
   */
  void Reserve(SizeType capacity);

  /** Shrink the capacity of the list to that specified. If the capacity is
   * already less than the one to shrink to nothing will happen.
   * \brief Shrink capacity.
   * \param capacity Capacity to shrink to.
   */
  void Shrink(SizeType capacity);

  /** Shrink the capacity of the list to exactly fit all the objects currently
   * in the list.
   * \brief Shrink to fit.
   */
  void ShrinkToFit();

  /** Returns whether or not the list contains the specified object.
   * \brief Returns whether object is in list.
   * \param object Object to check if in list.
   * \return True if the list contains the object otherwise false.
   */
  bool Contains(const T& object) const;

  /** Returns whether or not the list contains the specified object.
   * \brief Returns whether object is in list.
   * \param object Object to check if in list.
   * \param compareFunction Function for comparing objects in list.
   * \return True if the list contains the object otherwise false.
   */
  template<typename S>
  bool Contains(
    const S& object,
    std::function<bool(const S& lhs, const T& rhs)> compareFunction);

  /** Concatenate this and another array-list together into a combined list. The
   * specified (optional) allocator is the allocator of the resulting list.
   * \brief Returns concatenated list.
   * \param other List to concatenate with.
   * \return Concatenated list.
   */
  ArrayList<T> Concatenated(const ArrayList<T>& other) const;

  /** Returns the object at the specified index in the list.
   * \pre Index must not be out of bounds.
   * \brief Returns object at index.
   * \param index Index to return object at.
   * \return Object at index.
   */
  T& At(SizeType index);

  /** Returns the object at the specified index in the list.
   * \pre Index must not be out of bounds.
   * \brief Returns object at index.
   * \param index Index to return object at.
   * \return Object at index.
   */
  const T& At(SizeType index) const;

  /** Returns the object at the specified index in the list.
   * \pre Index must not be out of bounds.
   * \brief Returns object at index.
   * \param index Index to return object at.
   * \return Object at index.
   */
  T& operator[](SizeType index);

  /** Returns the object at the specified index in the list.
   * \pre Index must not be out of bounds.
   * \brief Returns object at index.
   * \param index Index to return object at.
   * \return Object at index.
   */
  const T& operator[](SizeType index) const;

  /** Returns the iterator to the beginning of the list.
   * \brief Returns beginning iterator.
   * \return Begin iterator.
   */
  Iterator Begin() const { return Iterator(mBuffer); }

  /** \copydoc ArrayList::Begin **/
  Iterator begin() const { return Begin(); }

  /** Returns the iterator to the end of the list.
   * \brief Returns ending iterator.
   * \return End iterator.
   */
  Iterator End() const { return Iterator(mBuffer + mSize); }

  /** \copydoc ArrayList::End **/
  Iterator end() const { return End(); }

  /** Returns the underlying data buffer of the list.
   * \brief Returns data.
   * \return Data pointer.
   */
  PointerType GetData() { return mBuffer; }

  /** Returns the underlying data buffer of the list.
   * \brief Returns data.
   * \return Data pointer.
   */
  ConstPointerType GetData() const { return mBuffer; }

  /** Returns the capacity of the list.
   * \brief Returns capacity.
   * \return Capacity.
   */
  SizeType GetCapacity() const { return mCapacity; }

  /** Returns the size of the list.
   * \brief Returns size.
   * \return Size.
   */
  SizeType GetSize() const { return mSize; }

public:
  /** Concatenate two array-lists together into one list. The default system
   * allocator is used for the resulting array-list.
   * \brief Concatenate lists.
   * \param lhs Left-hand side.
   * \param rhs Right-hand side.
   * \return Concatenated list.
   */
  friend ArrayList<T> operator+(const ArrayList<T>& lhs,
                                const ArrayList<T>& rhs)
  {
    return lhs.Concatenated(rhs);
  }

private:
  /** Check that the capacity is enough to add an object. If it's not then
   * resize **/
  void CheckCapacityToAdd();
};

// -------------------------------------------------------------------------- //

template<typename T, bool R>
ArrayList<T, R>::Iterator::Iterator(ArrayList::PointerType pointer)
  : mPointer(pointer)
{}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
void
ArrayList<T, R>::Iterator::operator++()
{
  ++mPointer;
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
void
ArrayList<T, R>::Iterator::operator--()
{
  --mPointer;
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
bool
ArrayList<T, R>::Iterator::operator!=(const Iterator& other)
{
  return mPointer != other.mPointer;
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
typename ArrayList<T, R>::ReferenceType ArrayList<T, R>::Iterator::operator*()
{
  return *mPointer;
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
typename ArrayList<T, R>::PointerType ArrayList<T, R>::Iterator::operator->()
{
  return mPointer;
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
ArrayList<T, R>::ArrayList(u64 capacity)
  : mBuffer(nullptr)
  , mCapacity(capacity)
  , mSize(0)
{
  mBuffer =
    static_cast<T*>(Memory::Allocate(mCapacity * OBJECT_SIZE, alignof(T)));
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
ArrayList<T, R>::ArrayList(std::initializer_list<T> initializerList)
  : mCapacity(initializerList.size())
  , mSize(0)
{
  mBuffer =
    static_cast<T*>(Memory::Allocate(mCapacity * OBJECT_SIZE, alignof(T)));
  for (const T& element : initializerList) {
    new (mBuffer + (mSize++)) T{ element };
  }
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
ArrayList<T, R>::ArrayList(const ArrayList& other)
  : mCapacity(other.mCapacity)
  , mSize(other.mSize)
{
  mBuffer =
    static_cast<T*>(Memory::Allocate(mCapacity * OBJECT_SIZE, alignof(T)));
  for (SizeType i = 0; i < mSize; ++i) {
    new (mBuffer + i) T{ other.mBuffer[i] };
  }
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
ArrayList<T, R>::ArrayList(ArrayList&& other) noexcept
  : mBuffer(other.mBuffer)
  , mCapacity(other.mCapacity)
  , mSize(other.mSize)
{
  other.mBuffer = nullptr;
  other.mCapacity = 0;
  other.mSize = 0;
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
ArrayList<T, R>::~ArrayList()
{
  for (SizeType i = 0; i < mSize; ++i) {
    mBuffer[i].~T();
  }
  Memory::Free(mBuffer);
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
ArrayList<T, R>&
ArrayList<T, R>::operator=(const ArrayList& other)
{
  if (this != &other) {
    // Destruct this list
    for (SizeType i = 0; i < mSize; ++i) {
      mBuffer[i].~T();
    }
    Memory::Free(mBuffer);

    // Copy other list
    mCapacity = other.mCapacity;
    mSize = other.mSize;
    mBuffer =
      static_cast<T*>(Memory::Allocate(mCapacity * OBJECT_SIZE, alignof(T)));
    for (SizeType i = 0; i < mSize; ++i) {
      new (mBuffer + i) T{ other.mBuffer[i] };
    }
  }
  return *this;
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
ArrayList<T, R>&
ArrayList<T, R>::operator=(ArrayList&& other) noexcept
{
  if (this != &other) {
    // Destruct this list
    for (SizeType i = 0; i < mSize; ++i) {
      mBuffer[i].~T();
    }
    Memory::Free(mBuffer);

    // Move other list
    mBuffer = other.mBuffer;
    mCapacity = other.mCapacity;
    mSize = other.mSize;
    other.mBuffer = nullptr;
    other.mCapacity = 0;
    other.mSize = 0;
  }
  return *this;
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
void
ArrayList<T, R>::Append(const T& object)
{
  CheckCapacityToAdd();
  new (mBuffer + (mSize++)) T{ object };
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
void
ArrayList<T, R>::Append(T&& object)
{
  CheckCapacityToAdd();
  new (mBuffer + (mSize++)) T{ std::move(object) };
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
template<typename... ARGS>
T&
ArrayList<T, R>::AppendEmplace(ARGS&&... arguments)
{
  CheckCapacityToAdd();
  new (mBuffer + mSize) T{ std::forward<ARGS>(arguments)... };
  return At(mSize++);
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
void
ArrayList<T, R>::Prepend(const T& object)
{
  CheckCapacityToAdd();
  for (SizeType i = (mSize > 0) ? (mSize - 1) : 0; i > 0; --i) {
    Memory::Relocate(mBuffer + i, mBuffer + i - 1);
  }
  new (mBuffer) T{ object };
  ++mSize;
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
void
ArrayList<T, R>::Prepend(T&& object)
{
  CheckCapacityToAdd();
  for (SizeType i = (mSize > 0) ? (mSize - 1) : 0; i > 0; --i) {
    Memory::Relocate(mBuffer + i, mBuffer + i - 1);
  }
  new (mBuffer) T{ std::move(object) };
  ++mSize;
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
template<typename... ARGS>
T&
ArrayList<T, R>::PrependEmplace(ARGS&&... arguments)
{
  CheckCapacityToAdd();
  for (SizeType i = (mSize > 0) ? (mSize - 1) : 0; i > 0; --i) {
    Memory::Relocate(mBuffer + i, mBuffer + i - 1);
  }
  new (mBuffer) T{ std::forward<ARGS>(arguments)... };
  ++mSize;
  return At(0);
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
void
ArrayList<T, R>::Remove(SizeType index)
{
  // Assert preconditions
  Assert(index >= 0 && index < mSize, "ArrayList Remove index out of bounds");

  // Destruct object and move other into spots
  mBuffer[index].~T();
  for (SizeType i = index; i < mSize - 1; ++i) {
    Memory::Relocate(mBuffer + i, mBuffer + i + 1);
  }
  mSize--;
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
void
ArrayList<T, R>::RemoveObject(const T& object)
{
  for (SizeType i = 0; i < mSize; ++i) {
    if (this[i] == object) {
      mBuffer[i].~T();
      for (SizeType j = i; j < mSize - 1; ++j) {
        Memory::Relocate(mBuffer + j, mBuffer + j + 1);
      }
      mSize--;
      return;
    }
  }
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
void
ArrayList<T, R>::RemoveObject(T&& object)
{
  for (SizeType i = 0; i < mSize; ++i) {
    if (this[i] == object) {
      mBuffer[i].~T();
      for (SizeType j = i; j < mSize - 1; ++j) {
        Memory::Relocate(mBuffer + j, mBuffer + j + 1);
      }
      mSize--;
      return;
    }
  }
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
void
ArrayList<T, R>::Resize(SizeType size)
{
  if (size < mCapacity) {
    // Destruct objects if new size is lesser
    for (SizeType i = size; i < mSize; ++i) {
      mBuffer[i].~T();
    }
    // Default construct objects if new size is greater
    for (SizeType i = mSize; i < size; ++i) {
      new (mBuffer + i) T{};
    }
    mSize = size;
  } else {
    // Allocate new buffer and move all objects
    T* newBuffer =
      static_cast<T*>(Memory::Allocate(size * OBJECT_SIZE, alignof(T)));
    for (SizeType i = 0; i < Min(mSize, size); ++i) {
      Memory::Relocate(newBuffer + i, mBuffer + i);
    }
    for (SizeType i = mSize; i < size; ++i) {
      new (newBuffer + i) T{};
    }
    Memory::Free(mBuffer);
    mBuffer = newBuffer;
    mSize = size;
  }
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
void
ArrayList<T, R>::Reserve(SizeType capacity)
{
  // Only do something if new capacity is greater than old
  if (capacity > mCapacity) {
    T* newBuffer =
      static_cast<T*>(Memory::Allocate(capacity * OBJECT_SIZE, alignof(T)));
    for (SizeType i = 0; i < mSize; ++i) {
      Memory::Relocate(newBuffer + i, mBuffer + i);
    }
    Memory::Free(mBuffer);
    mBuffer = newBuffer;
    mCapacity = capacity;
  }
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
void
ArrayList<T, R>::Shrink(SizeType capacity)
{
  // Only shrink if new capacity is less than old
  if (capacity < mCapacity) {
    T* newBuffer =
      static_cast<T*>(Memory::Allocate(capacity * OBJECT_SIZE, alignof(T)));
    for (SizeType i = 0; i < Min(mSize, capacity); ++i) {
      new (newBuffer + i) T{ std::move(mBuffer[i]) };
    }
    for (SizeType i = capacity; i < mSize; ++i) {
      mBuffer[i].~T();
    }
    Memory::Free(mBuffer);
    mBuffer = newBuffer;
    mCapacity = capacity;
    mSize = Min(mSize, capacity);
  }
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
void
ArrayList<T, R>::ShrinkToFit()
{
  Shrink(mSize);
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
bool
ArrayList<T, R>::Contains(const T& object) const
{
  for (SizeType i = 0; i < mSize; ++i) {
    if (At(i) == object) {
      return true;
    }
  }
  return false;
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
template<typename S>
bool
ArrayList<T, R>::Contains(
  const S& object,
  std::function<bool(const S&, const T&)> compareFunction)
{
  for (SizeType i = 0; i < mSize; ++i) {
    if (compareFunction(object, At(i))) {
      return true;
    }
  }
  return false;
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
ArrayList<T>
ArrayList<T, R>::Concatenated(const ArrayList<T>& other) const
{
  ArrayList output(mSize + other.mSize);
  for (T& object : *this) {
    output.Append(object);
  }
  for (T& object : other) {
    output.Append(object);
  }
  return output;
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
T&
ArrayList<T, R>::At(SizeType index)
{
  // Assert precondition
  Assert(index >= 0 && index < mSize, "ArrayList access index out of bounds");
  return mBuffer[index];
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
const T&
ArrayList<T, R>::At(SizeType index) const
{
  // Assert precondition
  Assert(index >= 0 && index < mSize, "ArrayList access index out of bounds");
  return mBuffer[index];
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
T& ArrayList<T, R>::operator[](SizeType index)
{
  return At(index);
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
const T& ArrayList<T, R>::operator[](SizeType index) const
{
  return At(index);
}

// -------------------------------------------------------------------------- //

template<typename T, bool R>
void
ArrayList<T, R>::CheckCapacityToAdd()
{
  if (mSize >= mCapacity) {
    Reserve(mCapacity ? mCapacity * RESIZE_FACTOR : DEFAULT_CAPACITY);
  }
}

}
