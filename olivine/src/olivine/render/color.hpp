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
// Color Declaration
// ========================================================================== //

namespace olivine {

/** Color **/
class Color
{
public:
  /** Maximum color channel value **/
  static constexpr u8 kColorMax = 255;

  /** Black **/
  static const Color kBlack;
  /** White **/
  static const Color kWhite;

  /** Red **/
  static const Color kRed;
  /** Green **/
  static const Color kGreen;
  /** Blue **/
  static const Color kBlue;
  /** Cyan **/
  static const Color kCyan;
  /** Magenta **/
  static const Color kMagenta;
  /** Yellow **/
  static const Color kYellow;

  /** Cornflower blue **/
  static const Color kCornflowerBlue;

private:
  /** Color channel values **/
  u8 mRed, mGreen, mBlue, mAlpha;

public:
  /** Construct a default color. The color will be white.
   * \brief Construct default color.
   */
  Color();

  /** Construct color **/
  Color(u32 red, u32 green, u32 blue, u32 alpha = u32(kColorMax)) noexcept;

  /** Construct color from normalized float values **/
  Color(f32 red, f32 green, f32 blue, f32 alpha = 1.0f) noexcept;

  /** Returns red channel **/
  u8& Red() { return mRed; }

  /** Returns red channel **/
  OL_NODISCARD const u8& Red() const { return mRed; }

  /** Returns green channel **/
  u8& Green() { return mGreen; }

  /** Returns green channel **/
  OL_NODISCARD const u8& Green() const { return mGreen; }

  /** Returns blue channel **/
  u8& Blue() { return mBlue; }

  /** Returns blue channel **/
  OL_NODISCARD const u8& Blue() const { return mBlue; }

  /** Returns alpha channel **/
  u8& Alpha() { return mAlpha; }

  /** Returns alpha channel **/
  OL_NODISCARD const u8& Alpha() const { return mAlpha; }

  /** Returns red as float **/
  f32 GetRedF32() const { return u32(mRed) / f32(kColorMax); }

  /** Returns green as float **/
  f32 GetGreenF32() const { return u32(mGreen) / f32(kColorMax); }

  /** Returns blue as float **/
  f32 GetBlueF32() const { return u32(mBlue) / f32(kColorMax); }

  /** Returns alpha as float **/
  f32 GetAlphaF32() const { return u32(mAlpha) / f32(kColorMax); }

public:
  /** Make a color from a hex value.
   * \brief Make color from hex.
   * \param value Color value in hex.
   * \return Made color.
   */
  static Color FromHex(u32 value);
};

}