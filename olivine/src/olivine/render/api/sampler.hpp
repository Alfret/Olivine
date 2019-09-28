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
#include "olivine/render/api/d3d12_util.hpp"

// ========================================================================== //
// Sampler Declaration
// ========================================================================== //

namespace olivine {

/** \class Sampler
 * \author Filip Björklund
 * \date 28 september 2019 - 18:25
 * \brief Sampler.
 * \details
 * Represents a sampler.
 */
class Sampler
{
public:
  /* Sampling filters */
  enum class Filter : u32
  {
    /* Specifies that point sampling should be used. This will pick a single
     * value */
    kPoint,
    /* Specifies that linear interpolation should be used. This will pick two
     * points and linearly interpolate the values between */
    kLinear,
  };

  /* Filter reduction kind */
  enum class ReductionKind
  {
    /* Standard filter kind */
    kStandard,
    /* Comparison filter kind */
    kComparison,
    /* Minimum filter kind */
    kMinimum,
    /* Maximum filter kind */
    kMaximum
  };

  /* Address modes */
  enum class AddressMode : u32
  {
    /* Specifies that texture coordinates are "wrapped back" to 0.0 after every
     * whole integer number. "0.5" and "1.5" would sample the same location */
    kWrap,
    /* Instead of wrapping the texture is mirrored every repetion. The value
     * "0.75" and "1.25" would sample the same location */
    kMirror,
    /* Mirror only once around origin. Clamping is used for values below "-1.0"
     * and values above "1.0" */
    kMirrorOrigin,
    /* Clamp values outside the range "0.0" to "1.0" to the range. "1.0" and
     * ">1.0" would sample the same location */
    kClamp,
    /* Coordinates outside the range "0.0" to "1.0" does not sample the texture
     * and instead uses the border color set when creating the sampler
     * (or static sampler) */
    kBorder,
  };

  /* Border colors for sampling with 'kBorder' address mode. These are only used
   * by static samplers. Standard samplers can use any color */
  enum class BorderColor
  {
    /* White (opaque) */
    kWhite,
    /* Black (opaque) */
    kBlack,
    /* Black (transparent) */
    kBlackTransparent
  };

public:
  /** Determine a 'D3D12_FILTER' from the separate minification, magnification
   * and mip-level filters. Whether the filter should use anisotropic filtering
   * and what the reduction kind should be can also specified.
   * \brief Convert to 'D3D12_FILTER' from individual filter values.
   * \param min Minification filter.
   * \param mag Magnification filter.
   * \param mip Mip-level filter.
   * \param anisotropic Whether anisotropic filtering should be used.
   * \param reductionKind Reduction kind to use.
   */
  static D3D12_FILTER ToFilter(
    Filter min,
    Filter mag,
    Filter mip,
    bool anisotropic,
    ReductionKind reductionKind = ReductionKind::kStandard);

  /** Convert from 'Filter' to 'D3D12_TEXTURE_ADDRESS_MODE' **/
  static D3D12_TEXTURE_ADDRESS_MODE ToAddressMode(AddressMode addressMode);

  /** Convert from 'BorderColor' to 'D3D12_STATIC_BORDER_COLOR' **/
  static D3D12_STATIC_BORDER_COLOR ToBorderColor(BorderColor borderColor);
};

}