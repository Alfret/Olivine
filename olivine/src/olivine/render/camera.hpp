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
#include "olivine/math/matrix4f.hpp"
#include "olivine/math/vector3f.hpp"

// ========================================================================== //
// Camera Declaration
// ========================================================================== //

namespace olivine {

/** \class Camera
 * \author Filip Björklund
 * \date 07 december 2019 - 13:49
 * \brief
 * \details
 */
class Camera
{
private:
  /* Project matrix */
  Matrix4F mProjection;
  /* View matrix */
  Matrix4F mView;

  /* Near z-depth */
  f32 mZNear;
  /* Far z-depth */
  f32 mZFar;
  /* Position */
  Vector3F mPos;

public:
  Camera(f32 fov, f32 aspectRatio, f32 zNear, f32 zFar);

  void Resize(f32 width, f32 height);

  const Matrix4F& GetProjection() const { return mProjection; }

  const Matrix4F& GetView() const { return mView; }

  const Vector3F& GetPosition() const { return mPos; }

  void SetPosition(const Vector3F& position) { mPos = position; }
};

}