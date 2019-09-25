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
#include "olivine/render/api/d3d12_util.hpp"

// ========================================================================== //
// Context Declaration
// ========================================================================== //

namespace olivine {

/** \class Context
 * \author Filip Björklund
 * \date 25 september 2019 - 16:06
 * \brief Render context.
 * \details
 * Represents a Direct3D 12 render context.
 */
class Context
{
public:
  /** Creation flags **/
  enum class Flag : u32
  {
    /* No specific flags */
    kNone = 0,
    /* Specifies that the context should be created with debug options turned
     * on */
    kDebug,
  };
  OL_ENUM_CLASS_OPERATORS(friend, Flag, u32);

  /* Creation information */
  struct CreateInfo
  {
#if defined(_DEBUG)
    /** Creation flags (Defaults to enabling debug) **/
    Flag flags = Flag::kDebug;
#else
    /** Creation flags **/
    Flag flags = Flag::kNone;
#endif
  };

private:
  /* DXGI factory */
  IDXGIFactory4* mFactory;

public:
  /** Create a render context.
   * \brief Create context.
   * \param createInfo Creation information.
   */
  explicit Context(const CreateInfo& createInfo);

  /** Destruct the render context.
   * \brief Destruct render context.
   */
  ~Context();

  /** Returns the DXGI factory.
   * \brief Returns DXGI factory.
   * \return DXGI factory.
   */
  IDXGIFactory4* GetFactory() const { return mFactory; }
};

}