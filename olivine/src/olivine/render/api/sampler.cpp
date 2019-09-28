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

#include "olivine/render/api/sampler.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/core/assert.hpp"

// ========================================================================== //
// Sampler Implementation
// ========================================================================== //

namespace olivine {

D3D12_FILTER
Sampler::ToFilter(Filter min,
                  Filter mag,
                  Filter mip,
                  bool anisotropic,
                  ReductionKind reductionKind)
{
  // Function to convert filter type
  static auto ToFilterType = [](Filter filter) -> D3D12_FILTER_TYPE {
    return filter == Filter::kPoint ? D3D12_FILTER_TYPE_POINT
                                    : D3D12_FILTER_TYPE_LINEAR;
  };

  // Function to convert reduction type
  static auto ToReductionType =
    [](ReductionKind kind) -> D3D12_FILTER_REDUCTION_TYPE {
    switch (kind) {
      case ReductionKind::kStandard: {
        return D3D12_FILTER_REDUCTION_TYPE_STANDARD;
      }
      case ReductionKind::kComparison: {
        return D3D12_FILTER_REDUCTION_TYPE_COMPARISON;
      }
      case ReductionKind::kMinimum: {
        return D3D12_FILTER_REDUCTION_TYPE_MINIMUM;
      }
      case ReductionKind::kMaximum: {
        return D3D12_FILTER_REDUCTION_TYPE_MAXIMUM;
      }
      default: {
        Panic("Invalid filter reduction type");
      }
    }
  };

  // Create filter
  if (anisotropic) {
    return D3D12_ENCODE_ANISOTROPIC_FILTER(ToFilterType(min),
                                           ToFilterType(mag),
                                           ToFilterType(mip),
                                           ToReductionType(reductionKind));
  }
  return D3D12_ENCODE_BASIC_FILTER(ToFilterType(min),
                                   ToFilterType(mag),
                                   ToFilterType(mip),
                                   ToReductionType(reductionKind));
}

// -------------------------------------------------------------------------- //

D3D12_TEXTURE_ADDRESS_MODE
Sampler::ToAddressMode(AddressMode addressMode)
{
  switch (addressMode) {
    case AddressMode::kWrap: {
      return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    }
    case AddressMode::kMirror: {
      return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
    }
    case AddressMode::kMirrorOrigin: {
      return D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE;
    }
    case AddressMode::kClamp: {
      return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    }
    case AddressMode::kBorder: {
      return D3D12_TEXTURE_ADDRESS_MODE_BORDER;
    }
    default: {
      Panic("Invalid sampler address mode");
    }
  }
}

// -------------------------------------------------------------------------- //

D3D12_STATIC_BORDER_COLOR
Sampler::ToBorderColor(BorderColor borderColor)
{
  switch (borderColor) {
    case BorderColor::kWhite: {
      return D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
    }
    case BorderColor::kBlack: {
      return D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
    }
    case BorderColor::kBlackTransparent: {
      return D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    }
    default: {
      Panic("Invalid sampler border color");
    }
  }
}

}
