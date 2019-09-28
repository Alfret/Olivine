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
#include "olivine/core/string.hpp"
#include "olivine/render/format.hpp"

// Platform headers
#include <dxgi1_6.h>
#include <d3d12.h>

#if defined(_DEBUG)
#include <dxgidebug.h>
#endif

// Thirdparty headers
#include "thirdparty/d3d12ma/D3D12MemAlloc.h"

// ========================================================================== //
// D3D12Util Declaration
// ========================================================================== //

namespace olivine {

OL_FORWARD_DECLARE(String);
OL_FORWARD_DECLARE_ENUM(HeapKind : u32);
OL_FORWARD_DECLARE_ENUM(ResourceState : u32);
OL_FORWARD_DECLARE_ENUM(PrimitiveTopology : u32);
OL_FORWARD_DECLARE_ENUM(ComparisonFunction : u32);

/** \class D3D12Util
 * \author Filip Björklund
 * \date 25 september 2019 - 16:12
 * \brief Direct3D 12 utils.
 * \details
 * Class with only static Direct3D 12 utilities.
 */
class D3D12Util
{
  OL_NAMESPACE_CLASS(D3D12Util);

public:
  /** Assert that an HRESULT represents success.
   * \brief Assert HRESULT.
   * \param hresult Result to check.
   * \param format Format for the message string.
   * \param arguments Arguments to format string.
   */
  template<typename... ARGS>
  static void Assert(HRESULT hresult,
                     const String& format,
                     ARGS&&... arguments);

  /** Assert that an HRESULT represents success.
   * \brief Assert HRESULT.
   * \param hresult Result to check.
   * \param message Message.
   */
  static void Assert(HRESULT hresult, const String& message);

  /** Convert from 'Format' to 'DXGI_FORMAT'
   * \brief Convert to DXGI format.
   * \param format Format to convert.
   * \return Converted format.
   */
  static DXGI_FORMAT ToDXGIFormat(Format format);

  /** Convert from 'HeapKind' to 'D3D12_HEAP_TYPE'.
   * \brief Convert to heap type.
   * \param kind Kind to convert.
   * \return Converted kind.
   */
  static D3D12_HEAP_TYPE ToHeapType(HeapKind kind);

  /** Convert from 'ResourceState' to 'D3D12_RESOURCE_STATES'.
   * \brief Convert to resource state.
   * \param state State to convert.
   * \return Converted state.
   */
  static D3D12_RESOURCE_STATES ToResourceStates(ResourceState state);

  /** Convert from 'PrimitiveTopology' to 'D3D12_PRIMITIVE_TOPOLOGY'.
   * \brief Convert to primitive topology.
   * \param topology Primitive topology to convert.
   * \return Converted primitive topology.
   */
  static D3D12_PRIMITIVE_TOPOLOGY ToPrimitiveTopology(
    PrimitiveTopology topology);

  /** Convert from 'ComparisonFunction' to 'D3D12_COMPARISON_FUNC'.
   * \brief Convert to comparison function.
   * \param func Comparison function to convert.
   * \return Converted comparison function.
   */
  static D3D12_COMPARISON_FUNC ToComparisonFunc(ComparisonFunction func);

  /** Set the name of a IDXGIObject.
   * \brief Set IDXGIObject name.
   */
  static void SetName(IDXGIObject* object, const String& name);

  /** Set the name of a ID3D12Object.
   * \brief Set ID3D12Object name.
   */
  static void SetName(ID3D12Object* object, const String& name);
};

// -------------------------------------------------------------------------- //

template<typename... ARGS>
void
D3D12Util::Assert(HRESULT hresult, const String& format, ARGS&&... arguments)
{
  Assert(hresult, String::Format(format, std::forward<ARGS>(arguments)...));
}

}
