// MIT License
//
// Copyright (c) 2019 Filip Bj�rklund
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
#include "olivine/math/constants.hpp"

// ========================================================================== //
// Functions
// ========================================================================== //

namespace olivine {

/** Literal for creating a bytes value from kibibytes **/
constexpr unsigned long long operator"" _KiB(unsigned long long kibibytes)
{
  return kibibytes * 1024;
}

// -------------------------------------------------------------------------- //

/** Literal for creating a bytes value from mebibytes **/
constexpr unsigned long long operator"" _MiB(unsigned long long mebibytes)
{
  return mebibytes * 1024 * 1024;
}

// -------------------------------------------------------------------------- //

/** Literal for creating a bytes value from gibibytes **/
constexpr unsigned long long operator"" _GiB(unsigned long long gibibytes)
{
  return gibibytes * 1024 * 1024 * 1024;
}

// -------------------------------------------------------------------------- //

/** Literal for creating a radian value from degrees **/
constexpr long double operator"" _Deg(long double degrees)
{
  return f32((degrees / 180.0) * Constants::kPi64);
}

}