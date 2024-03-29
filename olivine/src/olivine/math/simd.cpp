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

#include "olivine/math/simd.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Standard headers
#include <memory.h>

// Project headers
#include "olivine/core/common.hpp"
#include "olivine/core/assert.hpp"

// ========================================================================== //
// Float4x32 Implementation
// ========================================================================== //

namespace olivine {

Float4x32::Float4x32(f32 x, f32 y, f32 z, f32 w)
  : mData{ x, y, z, w }
{}

// -------------------------------------------------------------------------- //

Float4x32::Float4x32(f32 values[4])
  : Float4x32(values[0], values[1], values[2], values[3])
{}

// -------------------------------------------------------------------------- //

Float4x32::Float4x32(f32 value)
  : Float4x32(value, value, value, value)
{}

// -------------------------------------------------------------------------- //

Float4x32::Float4x32()
  : Float4x32(0.0f, 0.0f, 0.0f, 0.0f)
{}

// -------------------------------------------------------------------------- //

Float4x32::Float4x32(const std::initializer_list<f32>& list)
{
  u32 index = 0;
  for (const f32& element : list) {
    mData.componentArray[index] = element;
    if (++index > 3) {
      break;
    }
  }
}

// -------------------------------------------------------------------------- //

Float4x32::Float4x32(const Float4x32& other)
{
  mData.simd = other.mData.simd;
}

// -------------------------------------------------------------------------- //

Float4x32::Float4x32(Float4x32&& other) noexcept
{
  mData.simd = other.mData.simd;
}

// -------------------------------------------------------------------------- //

Float4x32&
Float4x32::operator=(const Float4x32& other)
{
  if (this != &other) {
    mData.simd = other.mData.simd;
  }
  return *this;
}

// -------------------------------------------------------------------------- //

Float4x32&
Float4x32::operator=(Float4x32&& other) noexcept
{
  if (this != &other) {
    mData.simd = other.mData.simd;
  }
  return *this;
}

// -------------------------------------------------------------------------- //

Float4x32
Float4x32::operator+(const Float4x32& other) const
{
  return Float4x32{ _mm_add_ps(mData.simd, other.mData.simd) };
}

// -------------------------------------------------------------------------- //

void
Float4x32::operator+=(const Float4x32& other)
{
  mData.simd = _mm_add_ps(mData.simd, other.mData.simd);
}

// -------------------------------------------------------------------------- //

Float4x32
Float4x32::operator-(const Float4x32& other) const
{
  return Float4x32{ _mm_sub_ps(mData.simd, other.mData.simd) };
}

// -------------------------------------------------------------------------- //

void
Float4x32::operator-=(const Float4x32& other)
{
  mData.simd = _mm_sub_ps(mData.simd, other.mData.simd);
}

// -------------------------------------------------------------------------- //

Float4x32 Float4x32::operator*(const Float4x32& other) const
{
  return Float4x32{ _mm_mul_ps(mData.simd, other.mData.simd) };
}

// -------------------------------------------------------------------------- //

void
Float4x32::operator*=(const Float4x32& other)
{
  mData.simd = _mm_mul_ps(mData.simd, other.mData.simd);
}

// -------------------------------------------------------------------------- //

Float4x32
Float4x32::operator/(const Float4x32& other) const
{
  return Float4x32{ _mm_div_ps(mData.simd, other.mData.simd) };
}

// -------------------------------------------------------------------------- //

void
Float4x32::operator/=(const Float4x32& other)
{
  mData.simd = _mm_div_ps(mData.simd, other.mData.simd);
}

// -------------------------------------------------------------------------- //

bool
Float4x32::operator==(const Float4x32& other) const
{
  const __m128 result = _mm_cmpeq_ps(mData.simd, other.mData.simd);
  return _mm_movemask_ps(result) == 0xFFFF;
}

// -------------------------------------------------------------------------- //

bool
Float4x32::operator!=(const Float4x32& other) const
{
  return !this->operator==(other);
}

// -------------------------------------------------------------------------- //

f32& Float4x32::operator[](u32 index)
{
  Assert(index < 4, "Index out of bounds");
  return mData.componentArray[index];
}

// -------------------------------------------------------------------------- //

const f32& Float4x32::operator[](u32 index) const
{
  Assert(index < 4, "Index out of bounds");
  return mData.componentArray[index];
}

// -------------------------------------------------------------------------- //

f32
Float4x32::InnerProduct(const Float4x32& other) const
{
  return mData.components.x * other.mData.components.x +
         mData.components.y * other.mData.components.y +
         mData.components.z * other.mData.components.z +
         mData.components.w * other.mData.components.w;
}

// -------------------------------------------------------------------------- //

void
Float4x32::Store(f32* memory) const
{
  _mm_store_ps(memory, mData.simd);
}

// -------------------------------------------------------------------------- //

Float4x32::Float4x32(__m128 simd)
{
  mData.simd = simd;
}

}

// ========================================================================== //
// Float2x32 Implementation
// ========================================================================== //

namespace olivine {

Float2x32::Float2x32(f32 x, f32 y)
{
  mData.components = { x, y };
}

// -------------------------------------------------------------------------- //

Float2x32::Float2x32(f32 values[2])
  : Float2x32(values[0], values[1])
{}

// -------------------------------------------------------------------------- //

Float2x32::Float2x32(f32 value)
  : Float2x32(value, value)
{}

// -------------------------------------------------------------------------- //

Float2x32::Float2x32()
  : Float2x32(0.0f, 0.0f)
{}

// -------------------------------------------------------------------------- //

Float2x32::Float2x32(const std::initializer_list<f32>& list)
{
  u32 index = 0;
  for (const f32& element : list) {
    mData.componentArray[index] = element;
    if (++index > 1) {
      break;
    }
  }
}

// -------------------------------------------------------------------------- //

Float2x32::Float2x32(const Float2x32& other)
{
  mData.simd = other.mData.simd;
}

// -------------------------------------------------------------------------- //

Float2x32::Float2x32(Float2x32&& other) noexcept
{
  mData.simd = other.mData.simd;
}

// -------------------------------------------------------------------------- //

Float2x32&
Float2x32::operator=(const Float2x32& other)
{
  if (this != &other) {
    mData.simd = other.mData.simd;
  }
  return *this;
}

// -------------------------------------------------------------------------- //

Float2x32&
Float2x32::operator=(Float2x32&& other) noexcept
{
  if (this != &other) {
    mData.simd = other.mData.simd;
  }
  return *this;
}

// -------------------------------------------------------------------------- //

Float2x32
Float2x32::operator+(const Float2x32& other) const
{
  const __m128 left = LoadSimd128();
  const __m128 right = other.LoadSimd128();
  return Float2x32{ _mm_add_ps(left, right) };
}

// -------------------------------------------------------------------------- //

void
Float2x32::operator+=(const Float2x32& other)
{
  const __m128 left = LoadSimd128();
  const __m128 right = other.LoadSimd128();
  StoreSimd128(_mm_add_ps(left, right));
}

// -------------------------------------------------------------------------- //

Float2x32
Float2x32::operator-(const Float2x32& other) const
{
  const __m128 left = LoadSimd128();
  const __m128 right = other.LoadSimd128();
  return Float2x32{ _mm_sub_ps(left, right) };
}

// -------------------------------------------------------------------------- //

void
Float2x32::operator-=(const Float2x32& other)
{
  const __m128 left = LoadSimd128();
  const __m128 right = other.LoadSimd128();
  StoreSimd128(_mm_sub_ps(left, right));
}

// -------------------------------------------------------------------------- //

Float2x32 Float2x32::operator*(const Float2x32& other) const
{
  const __m128 left = LoadSimd128();
  const __m128 right = other.LoadSimd128();
  return Float2x32{ _mm_mul_ps(left, right) };
}

// -------------------------------------------------------------------------- //

void
Float2x32::operator*=(const Float2x32& other)
{
  const __m128 left = LoadSimd128();
  const __m128 right = other.LoadSimd128();
  StoreSimd128(_mm_mul_ps(left, right));
}

// -------------------------------------------------------------------------- //

Float2x32
Float2x32::operator/(const Float2x32& other) const
{
  const __m128 left = LoadSimd128();
  const __m128 right = other.LoadSimd128();
  return Float2x32{ _mm_div_ps(left, right) };
}

// -------------------------------------------------------------------------- //

void
Float2x32::operator/=(const Float2x32& other)
{
  const __m128 left = LoadSimd128();
  const __m128 right = other.LoadSimd128();
  StoreSimd128(_mm_div_ps(left, right));
}

// -------------------------------------------------------------------------- //

bool
Float2x32::operator==(const Float2x32& other) const
{
  return mData.components.x == other.mData.components.x &&
         mData.components.y == other.mData.components.y;
}

// -------------------------------------------------------------------------- //

bool
Float2x32::operator!=(const Float2x32& other) const
{
  return !operator==(other);
}

// -------------------------------------------------------------------------- //

f32& Float2x32::operator[](u32 index)
{
  Assert(index < 2, "Index out of bounds");
  return mData.componentArray[index];
}

// -------------------------------------------------------------------------- //

const f32& Float2x32::operator[](u32 index) const
{
  Assert(index < 2, "Index out of bounds");
  return mData.componentArray[index];
}

// -------------------------------------------------------------------------- //

f32
Float2x32::InnerProduct(const Float2x32& other) const
{
  return mData.components.x * other.mData.components.x +
         mData.components.y * other.mData.components.y;
}

// -------------------------------------------------------------------------- //

Float2x32::Float2x32(__m64 simd)
{
  mData.simd = simd;
}

// -------------------------------------------------------------------------- //

Float2x32::Float2x32(__m128 simd)
{
  StoreSimd128(simd);
}

// -------------------------------------------------------------------------- //

__m128
Float2x32::LoadSimd128() const
{
  const __m128d tmp = _mm_load_sd(reinterpret_cast<const double*>(this));
  return _mm_castpd_ps(tmp);
}

// -------------------------------------------------------------------------- //

void
Float2x32::StoreSimd128(__m128 data)
{
  _mm_storel_pi(&mData.simd, data);
}

}