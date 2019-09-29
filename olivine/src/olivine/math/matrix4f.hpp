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
#include "olivine/math/simd.hpp"

// ========================================================================== //
// Matrix4F Declaration
// ========================================================================== //

namespace olivine {

OL_FORWARD_DECLARE(Vector3F);
OL_FORWARD_DECLARE(Vector4F);

/** \class Matrix4F
 * \author Filip Björklund
 * \date 29 september 2019 - 14:39
 * \brief 4-by-4, f32 valued matrix.
 * \details
 * Represents a 4-by-4 matrix of f32 values.
 */
class Matrix4F
{
public:
  /* Number of elements in matrix */
  static constexpr u32 kElementCount = 16;

private:
  /* Data union for 'simd' and 'array' */
  union Data
  {
    /* Simd */
    Float4x32 simd[4]{};
    /* Array */
    f32 elements[16];
  } mData{};

public:
  /** Construct a 4-by-4 matrix with the specified value set for the diagonal
   * elements.
   * \brief Construct matrix with diagonal value.
   * \param diagonal Value to set for diagonal elements.
   */
  explicit Matrix4F(f32 diagonal = 0.0f);

  /** Construct matrix from list of values. Pointer is expected to point to an
   * array of at least 16 f32 values **/
  explicit Matrix4F(f32* values);

  /** Construct matrix from initializer list. If list is shorter than 16 then
   * zeroes are used to pad **/
  Matrix4F(const std::initializer_list<f32>& list);

  /** Copy constructor **/
  Matrix4F(const Matrix4F& other);

  /** Copy assignment **/
  Matrix4F& operator=(const Matrix4F& other);

  /** Addition **/
  Matrix4F operator+(const Matrix4F& other) const;

  /** Compound addition **/
  void operator+=(const Matrix4F& other);

  /** Subtraction **/
  Matrix4F operator-(const Matrix4F& other) const;

  /** Compound subtraction **/
  void operator-=(const Matrix4F& other);

  /** Compound multiplication **/
  void operator*=(const Matrix4F& other);

  /** Matrix-Scalar multiplication **/
  void operator*=(f32 scalar);

  /** Transpose the matrix.
   * \brief Transpose matrix.
   * \return Itself.
   */
  Matrix4F& Transpose();

  /** Returns the matrix data **/
  OL_NODISCARD f32* GetData() { return mData.elements; }

  /** Returns a column from the matrix. Note that columns are not layed out
   * linear in memory thus retrieveing columns is costly.
   * \brief Returns column.
   * \param column Column to return.
   * \return Column.
   */
  Vector4F GetColumn(u32 column) const;

  friend Matrix4F operator*(const Matrix4F& lhs, const Matrix4F& rhs);

  friend Vector4F operator*(const Matrix4F& lhs, const Vector4F& rhs);

  friend Vector4F operator*(const Vector4F& lhs, const Matrix4F& rhs);

  friend Matrix4F operator*(const Matrix4F& lhs, f32 rhs);

  friend Matrix4F operator*(f32 lhs, const Matrix4F& rhs);

private:
  /** Construct matrix from 4 simd values for rows **/
  Matrix4F(Float4x32 r0, Float4x32 r1, Float4x32 r2, Float4x32 r3);

public:
  /** Returns a matrix representing the identity matrix. All the values on the
   * diagonal are set to 1.0.
   * \brief Returns identity matrix.
   * \return Identity matrix.
   */
  static Matrix4F Identity();

  /** Returns a matrix that represents a translation.
   * \brief Returns translation matrix.
   * \param x Translation in X axis.
   * \param y Translation in Y axis.
   * \param z Translation in Z axis.
   * \return Translation matrix
   */
  static Matrix4F Translation(f32 x, f32 y, f32 z);

  /** Returns a matrix that represents a translation.
   * \brief Returns translation matrix.
   * \param vector Vector to create translation for. First element corresponds
   * to X, second to Y and third to Z.
   * \return Translation matrix
   */
  static Matrix4F Translation(const Vector3F& vector);

  /** Returns a matrix that represents a translation.
   * \brief Returns translation matrix.
   * \param vector Vector to create translation for. First element corresponds
   * to X, second to Y and third to Z. Forth value is ignored.
   * \return Translation matrix
   */
  static Matrix4F Translation(const Vector4F& vector);

  /** Returns a matrix that represents a rotation around the X axis by the
   * specified number of radians.
   * \brief Returns rotation around X axis.
   * \param radians Number of radians to rotate with.
   * \return Rotation matrix.
   */
  static Matrix4F RotationX(f32 radians);

  /** Returns a matrix that represents a rotation around the Y axis by the
   * specified number of radians.
   * \brief Returns rotation around Y axis.
   * \param radians Number of radians to rotate with.
   * \return Rotation matrix.
   */
  static Matrix4F RotationY(f32 radians);

  /** Returns a matrix that represents a rotation around the Z axis by the
   * specified number of radians.
   * \brief Returns rotation around Z axis.
   * \param radians Number of radians to rotate with.
   * \return Rotation matrix.
   */
  static Matrix4F RotationZ(f32 radians);

  /** Returns a matrix that represents a rotation about all axis at the same
   * time. The rotation for each axis are specified by the respective element in
   * the vector.
   * \brief Returns rotation matrix.
   * \param vector Vector with radians angles for each axis.
   * \return Rotation matrix.
   */
  static Matrix4F Rotation(const Vector3F& vector);

  /** Returns a matrix that represents a uniform scaling on all three axis.
   * \brief Returns uniform scaling matrix.
   * \param scale Scaling factor.
   * \return Uniform scaling matrix.
   */
  static Matrix4F Scale(f32 scale);

  /** Returns a matrix that represents a non-uniform scaling. Meaning each axis
   * can be scaled by a different amount.
   * \brief Returns scaling matrix.
   * \param x Scaling factor for X axis.
   * \param y Scaling factor for Y axis.
   * \param z Scaling factor for Z axis.
   * \return Non-uniform scaling matrix.
   */
  static Matrix4F Scale(f32 x, f32 y, f32 z);

  /** Returns a matrix that represents an orthographic projection. The
   * projection is created from the specifed viewport width and height, aswell
   * as the distances for the near and far planes.
   * \brief Returns orthographic projection.
   * \param width Width of the viewport.
   * \param height Height of the viewport.
   * \param zNear Near plane.
   * \param zFar Far plane.
   * \return Orthographic projection matrix.
   */
  static Matrix4F Orthographic(f32 width, f32 height, f32 zNear, f32 zFar);

  /** Returns a matrix that represents an orthographic projection. The
   * projection is created from the specifed viewport edge values, aswell
   * as the distances for the near and far planes.
   * \brief Returns orthographic projection.
   * \param top Top edge of the viewport.
   * \param bottom Bottom edge of the viewport.
   * \param left Left edge of the viewport.
   * \param right Right edge of the viewport.
   * \param zNear Near plane.
   * \param zFar Far plane.
   * \return Orthographic projection matrix.
   */
  static Matrix4F Orthographic(f32 top,
                               f32 bottom,
                               f32 left,
                               f32 right,
                               f32 zNear,
                               f32 zFar);

  static Matrix4F Perspective(f32 top,
                              f32 bottom,
                              f32 left,
                              f32 right,
                              f32 zNear,
                              f32 zFar);

  static Matrix4F Perspective(f32 verticalFov,
                              f32 aspectRatio,
                              f32 zNear,
                              f32 zFar);
};

}