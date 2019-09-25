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
#include "olivine/core/types.hpp"
#include "olivine/core/macros.hpp"

// ========================================================================== //
// Format Enumeration
// ========================================================================== //

namespace olivine {

/** \enum Format
 * \author Filip Björklund
 * \date 25 september 2019 - 16:02
 * \brief Formats.
 * \details
 * Enumeration of formats that can be used to specify the layout of data that is
 * used by the GPU.
 */
enum class Format : u32
{
  /** Invalid format **/
  kInvalid = 0,
  /** 8-bit R, unsigned and normalized **/
  kR8Unorm,
  /** 8-bit RGBA, unsigned and normalized **/
  kR8G8B8A8Unorm,
  /** 8-bit BGRA, unsigned and normalized **/
  kB8G8R8A8Unorm,

  /** 32-bit depth float **/
  kD32Float,
  /** 24-bit depth unsigned normalized and 8-bit stencil unsigned integer **/
  kD24UnormS8Uint,
};
OL_ENUM_CLASS_OPERATORS(inline, Format, u32);

}
