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
#include "olivine/math/simd.hpp"

// ========================================================================== //
// Vector4F Declaration
// ========================================================================== //

namespace olivine {

/** Vector4F  **/
class Vector4F
{
private:
  /** Simd data **/
  Float4x32 mData;

public:
  /** Construct from values **/
  Vector4F(f32 x, f32 y, f32 z, f32 w = 0.0f);

  /** Construct and fill with a single value **/
  Vector4F(f32 value = 0.0f);

  /** Addition **/
  Vector4F operator+(const Vector4F& other);

  /** Compound addition **/
  void operator+=(const Vector4F& other);

  /** Subtraction **/
  Vector4F operator-(const Vector4F& other);

  /** Compound subtraction **/
  void operator-=(const Vector4F& other);

  /** Multiplication **/
  Vector4F operator*(const Vector4F& other);

  /** Compound multiplication **/
  void operator*=(const Vector4F& other);

  /** Division **/
  Vector4F operator/(const Vector4F& other);

  /** Compound division **/
  void operator/=(const Vector4F& other);

  /** Equality **/
  bool operator==(const Vector4F& other);

  /** Inequality **/
  bool operator!=(const Vector4F& other);

  /** Returns value and index **/
  f32& operator[](u32 index);

  /** Returns value and index **/
  const f32& operator[](u32 index) const;

  /** Dot product **/
  f32 Dot(const Vector4F& other) const;

  /** Returns the simd data **/
  Float4x32 GetData() const { return mData; };

  /** Returns the X value of the vector.
   * \brief Returns X value.
   * \return X value.
   */
  f32& X() { return mData.GetX(); }

  /** Returns the X value of the vector.
   * \brief Returns X value.
   * \return X value.
   */
  const f32& X() const { return mData.GetX(); }

  /** Returns the Y value of the vector.
   * \brief Returns Y value.
   * \return Y value.
   */
  f32& Y() { return mData.GetY(); }

  /** Returns the Y value of the vector.
   * \brief Returns Y value.
   * \return Y value.
   */
  const f32& Y() const { return mData.GetY(); }

  /** Returns the Z value of the vector.
   * \brief Returns Z value.
   * \return Z value.
   */
  f32& Z() { return mData.GetZ(); }

  /** Returns the Z value of the vector.
   * \brief Returns Z value.
   * \return Z value.
   */
  const f32& Z() const { return mData.GetZ(); }

  /** Returns the W value of the vector.
   * \brief Returns W value.
   * \return W value.
   */
  f32& W() { return mData.GetW(); }

  /** Returns the W value of the vector.
   * \brief Returns W value.
   * \return W value.
   */
  const f32& W() const { return mData.GetW(); }

private:
  /** Construct from data **/
  Vector4F(const Float4x32& data);
};

}