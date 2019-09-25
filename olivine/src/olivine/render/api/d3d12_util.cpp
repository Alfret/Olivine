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

#include "olivine/render/api/d3d12_util.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/core/string.hpp"

// ========================================================================== //
// D3D12Util Implementation
// ========================================================================== //

namespace olivine {

DXGI_FORMAT
D3D12Util::ToDXGIFormat(Format format)
{
  switch (format) {
    case Format::kR8Unorm: {
      return DXGI_FORMAT_R8_UNORM;
    }
    case Format::kR8G8B8A8Unorm: {
      return DXGI_FORMAT_R8G8B8A8_UNORM;
    }
    case Format::kB8G8R8A8Unorm: {
      return DXGI_FORMAT_B8G8R8A8_UNORM;
    }
    case Format::kD32Float: {
      return DXGI_FORMAT_D32_FLOAT;
    }
    case Format::kD24UnormS8Uint: {
      return DXGI_FORMAT_D24_UNORM_S8_UINT;
    }
    case Format::kInvalid:
    default: {
      return DXGI_FORMAT_UNKNOWN;
    }
  }
}

// -------------------------------------------------------------------------- //

void
D3D12Util::SetName(IDXGIObject* object, const String& name)
{
  if (object) {
    char16* _name = name.GetUTF16();
    object->SetPrivateData(
      WKPDID_D3DDebugObjectNameW, UINT(wcslen(_name) * sizeof(char16)), _name);
    delete _name;
  }
}

// -------------------------------------------------------------------------- //

void
D3D12Util::SetName(ID3D12Object* object, const String& name)
{
  if (object) {
    char16* _name = name.GetUTF16();
    object->SetName(_name);
    delete _name;
  }
}

}
