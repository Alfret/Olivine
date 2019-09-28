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
#include "olivine/core/assert.hpp"

// ========================================================================== //
// Functions
// ========================================================================== //

namespace olivine {

/** Returns the minimum of two values.
 * \brief Returns min of two values.
 * \tparam T Type of values.
 * \param a First value.
 * \param b Second value.
 * \return Minimum value.
 */
template<typename T>
constexpr T
Min(T a, T b)
{
  return a < b ? a : b;
}

// -------------------------------------------------------------------------- //

/** Returns the maximum of two values.
 * \brief Returns max of two values.
 * \tparam T Type of values.
 * \param a First value.
 * \param b Second value.
 * \return Maximum value.
 */
template<typename T>
constexpr T
Max(T a, T b)
{
  return a > b ? a : b;
}

// -------------------------------------------------------------------------- //

/** Returns a value clamped between a lower and upper limit.
 * \brief Returns clamped value.
 * \param value Value to clamp.
 * \param lower Lower limit.
 * \param upper Upper limit.
 * \return Clamped value.
 */
template<typename T>
constexpr T
Clamp(T value, T lower, T upper)
{
  return value < lower ? lower : value > upper ? upper : value;
}

// -------------------------------------------------------------------------- //

/** Returns whether or not a given value is a power of two.
 * \brief Returns whether value is power of two.
 * \return True if the value is a power of two otherwise false.
 */
template<typename T>
constexpr bool
IsPowerOfTwo(T value)
{
  return value && !(value & (value - 1));
}

// -------------------------------------------------------------------------- //

/** Align a value to the specified alignment.
 * \brief Align value.
 * \param value Value to align.
 * \param alignment Alignment. Required to be a power of two.
 * \return Aligned value.
 */
inline u64
AlignUp(u64 value, u64 alignment)
{
  Assert(IsPowerOfTwo(alignment),
         "Can only align with an alignment that is a power of two");

#pragma warning(push)
#pragma warning(disable : 4146)
  return (value + (alignment - 1)) & -alignment;
#pragma warning(pop)
}

}