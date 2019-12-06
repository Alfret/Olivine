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
#include "olivine/core/file/path.hpp"

// ========================================================================== //
// Material Declaration
// ========================================================================== //

namespace olivine {

OL_FORWARD_DECLARE(Texture);
OL_FORWARD_DECLARE(CommandList);
OL_FORWARD_DECLARE(CommandQueue);

/** \class Material
 * \author Filip Björklund
 * \date 06 december 2019 - 12:54
 * \brief
 * \details
 */
class Material
{
private:
  /* Name of the material */
  String mName;

  /* Albedo path */
  Path mPathAlbedo;
  /* Albedo path */
  Path mPathRoughness;
  /* Albedo path */
  Path mPathMetallic;
  /* Albedo path */
  Path mPathNormal;

  /* Albedo texture */
  Texture* mTexAlbedo = nullptr;
  /* Roughness texture */
  Texture* mTexRoughness = nullptr;
  /* Metallic texture */
  Texture* mTexMetallic = nullptr;
  /* Normal texture */
  Texture* mTexNormal = nullptr;

public:
  Material(const String& name,
           const Path& pathAlbedo,
           const Path& pathRoughness,
           const Path& pathMetallic,
           const Path& pathNormal);

  /**
   *
   */
  void Upload(CommandQueue* queue, CommandList* list);
};

}