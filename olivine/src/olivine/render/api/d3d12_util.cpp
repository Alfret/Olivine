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
#include "olivine/app/app.hpp"
#include "olivine/core/string.hpp"
#include "olivine/core/assert.hpp"
#include "olivine/render/api/common.hpp"
#include "olivine/render/api/device.hpp"

// ========================================================================== //
// D3D12Util Implementation
// ========================================================================== //

namespace olivine {

void
D3D12Util::Assert(HRESULT hresult, const String& message)
{
  // Return if success
  if (SUCCEEDED(hresult)) {
    return;
  }

  // Otherwise assert (with added device-removed reason
  if (hresult == DXGI_ERROR_DEVICE_REMOVED) {
    Device* device = App::Instance()->GetDevice();
    const HRESULT reason = device->GetHandle()->GetDeviceRemovedReason();
    olivine::Assert(
      SUCCEEDED(hresult), "{} (reason: {:#x})", message, u32(reason));
  } else {
    olivine::Assert(SUCCEEDED(hresult), "{}", message);
  }
}

// -------------------------------------------------------------------------- //

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

D3D12_HEAP_TYPE
D3D12Util::ToHeapType(HeapKind kind)
{
  switch (kind) {
    case HeapKind::kDefault: {
      return D3D12_HEAP_TYPE_DEFAULT;
    }
    case HeapKind::kUpload: {
      return D3D12_HEAP_TYPE_UPLOAD;
    }
    case HeapKind::kReadback: {
      return D3D12_HEAP_TYPE_READBACK;
    }
    default: {
      return (D3D12_HEAP_TYPE)UINT_MAX;
    }
  }
}

// -------------------------------------------------------------------------- //

D3D12_RESOURCE_STATES
D3D12Util::ToResourceStates(ResourceState state)
{
  switch (state) {
    case ResourceState::kCommon: {
      return D3D12_RESOURCE_STATE_COMMON;
    }
    case ResourceState::kRenderTarget: {
      return D3D12_RESOURCE_STATE_RENDER_TARGET;
    }
    case ResourceState::kUnorderedAccess: {
      return D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    }
    case ResourceState::kPresent: {
      return D3D12_RESOURCE_STATE_PRESENT;
    }
    case ResourceState::kCopySrc: {
      return D3D12_RESOURCE_STATE_COPY_SOURCE;
    }
    case ResourceState::kCopyDst: {
      return D3D12_RESOURCE_STATE_COPY_DEST;
    }
    default: {
      return static_cast<D3D12_RESOURCE_STATES>(UINT_MAX);
    }
  }
}

// -------------------------------------------------------------------------- //

D3D12_PRIMITIVE_TOPOLOGY
D3D12Util::ToPrimitiveTopology(PrimitiveTopology topology)
{
  switch (topology) {
    case PrimitiveTopology::kTriangleList: {
      return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    }
    default: {
      Panic("Invalid primitive topology");
    }
  }
}

// -------------------------------------------------------------------------- //

D3D12_COMPARISON_FUNC
D3D12Util::ToComparisonFunc(ComparisonFunction func)
{
  switch (func) {
    case ComparisonFunction::kNever: {
      return D3D12_COMPARISON_FUNC_NEVER;
    }
    case ComparisonFunction::kAlways: {
      return D3D12_COMPARISON_FUNC_ALWAYS;
    }
    case ComparisonFunction::kLess: {
      return D3D12_COMPARISON_FUNC_LESS;
    }
    case ComparisonFunction::kGreater: {
      return D3D12_COMPARISON_FUNC_GREATER;
    }
    case ComparisonFunction::kLessEqual: {
      return D3D12_COMPARISON_FUNC_LESS_EQUAL;
    }
    case ComparisonFunction::kGreaterEqual: {
      return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
    }
    case ComparisonFunction::kEqual: {
      return D3D12_COMPARISON_FUNC_EQUAL;
    }
    case ComparisonFunction::kNotEqual: {
      return D3D12_COMPARISON_FUNC_NOT_EQUAL;
    }
    default: {
      Panic("Invalid comparison function");
    }
  }
}

// -------------------------------------------------------------------------- //

D3D12_CULL_MODE
D3D12Util::ToCullMode(CullMode cullMode)
{
  switch (cullMode) {
    case CullMode::kNone: {
      return D3D12_CULL_MODE_NONE;
    }
    case CullMode::kFront: {
      return D3D12_CULL_MODE_FRONT;
    }
    case CullMode::kBack: {
      return D3D12_CULL_MODE_BACK;
    }
    default: {
      Panic("Invalid cull mode");
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
