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
#include "olivine/math/matrix4f.hpp"

// ========================================================================== //
// Entity Declaration
// ========================================================================== //

namespace olivine {

OL_FORWARD_DECLARE(String);

OL_FORWARD_DECLARE(Loader);
OL_FORWARD_DECLARE(Model);

/** \class Entity
 * \author Filip Björklund
 * \date 06 december 2019 - 17:17
 * \brief
 * \details
 */
class Entity
{
private:
  /* Model */
  const Model* mModel = nullptr;
  /* Transform */
  Matrix4F mTransform;

public:
  explicit Entity(const Model* model);

  explicit Entity(const Loader* loader, const String& name);

  const Model* GetModel() const { return mModel; }

  const Matrix4F& GetTransform() const { return mTransform; }

  void SetTransform(const Matrix4F& transform) { mTransform = transform; }
};

}