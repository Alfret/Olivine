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

#include "olivine/render/color.hpp"

// ========================================================================== //
// Header
// ========================================================================== //

// Project headers
#include "olivine/math/math.hpp"

// ========================================================================== //
// Color Implementation
// ========================================================================== //

namespace olivine {

const Color Color::kBlack{ 0u, 0u, 0u };

// -------------------------------------------------------------------------- //

const Color Color::kWhite{ 0255u, 0255u, 0255u };

// -------------------------------------------------------------------------- //

const Color Color::kRed{ 0255u, 0u, 0u };

// -------------------------------------------------------------------------- //

const Color Color::kGreen{ 0u, 0255u, 0u };

// -------------------------------------------------------------------------- //

const Color Color::kBlue{ 0u, 0u, 0255u };

// -------------------------------------------------------------------------- //

const Color Color::kCyan{ 0u, 0255u, 0255u };

// -------------------------------------------------------------------------- //

const Color Color::kMagenta{ 0255u, 0u, 0255u };

// -------------------------------------------------------------------------- //

const Color Color::kYellow{ 0255u, 0255u, 0u };

// -------------------------------------------------------------------------- //

const Color Color::kCornflowerBlue{ 100u, 149u, 237u };

// -------------------------------------------------------------------------- //

Color::Color(u32 red, u32 green, u32 blue, u32 alpha) noexcept
  : mRed(Clamp(red, u32(0), u32(kColorMax)))
  , mGreen(Clamp(green, u32(0), u32(kColorMax)))
  , mBlue(Clamp(blue, u32(0), u32(kColorMax)))
  , mAlpha(Clamp(alpha, u32(0), u32(kColorMax)))
{}

// -------------------------------------------------------------------------- //

Color::Color(f32 red, f32 green, f32 blue, f32 alpha) noexcept
  : mRed(u8(Clamp(red, 0.0f, 1.0f) * kColorMax))
  , mGreen(u8(Clamp(green, 0.0f, 1.0f) * kColorMax))
  , mBlue(u8(Clamp(blue, 0.0f, 1.0f) * kColorMax))
  , mAlpha(u8(Clamp(alpha, 0.0f, 1.0f) * kColorMax))
{}

// -------------------------------------------------------------------------- //

Color
Color::FromHex(u32 value)
{
  return Color{ ((value >> 24u) & 0xFF),
                ((value >> 16u) & 0xFF),
                ((value >> 8u) & 0xFF),
                ((value >> 0u) & 0xFF) };
}

}
