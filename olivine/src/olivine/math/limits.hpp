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
#include "olivine/core/macros.hpp"
#include "olivine/core/types.hpp"

// ========================================================================== //
// Limits Declaration
// ========================================================================== //

namespace olivine {

/** \class Limits
 * \author Filip Björklund
 * \date 28 september 2019 - 11:48
 * \brief Limits.
 * \details
 * Namespace class with limits utilities.
 */
class Limits
{
  OL_NAMESPACE_CLASS(Limits);

public:
  /* Minimum value of 'u8' */
  static constexpr u8 kU8Min = 0u;
  /* Maximum value of 'u8' */
  static constexpr u8 kU8Max = 255u;

  /* Minimum value of 'u16' */
  static constexpr u16 kU16Min = 0u;
  /* Maximum value of 'u16' */
  static constexpr u16 kU16Max = 65535;

  /* Minimum value of 'u32' */
  static constexpr u32 kU32Min = 0;
  /* Maximum value of 'u32' */
  static constexpr u32 kU32Max = 4294967295ul;

  /* Minimum value of 'u64' */
  static constexpr u64 kU64Min = 0;
  /* Maximum value of 'u64' */
  static constexpr u64 kU64Max = 18446744073709551615ull;
};

}