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
#include "olivine/core/common.hpp"

// ========================================================================== //
// HeapKind Enumeration
// ========================================================================== //

namespace olivine {

/** \enum HeapKind
 * \author Filip Björklund
 * \date 25 september 2019 - 21:29
 * \brief Heap kinds.
 * \details
 * Enumeration of memory heap kinds.
 */
enum class HeapKind : u32
{
  /* Default memory heap. This kind should be used for memory that is only used
   * by the GPU and that is not changed often */
  kDefault,
  /* Upload heap. This kind should be used for memory that is going to be
   * written to by the CPU and read by the GPU */
  kUpload,
  /* Readback heap. This kind should be used for memory that is going to be
   * written to by the GPU and read by the CPU */
  kReadback
};

}

// ========================================================================== //
// ResourceState Enumeration
// ========================================================================== //

namespace olivine {

/** \enum ResourceState
 * \author Filip Björklund
 * \date 26 september 2019 - 12:26
 * \brief Resource states.
 * \details
 * Enumeration of resources states.
 */
enum class ResourceState : u32
{
  /* Common resource state. Should only be used for multi-queue operations */
  kCommon,
  /* State in which the resource can be used as a render-target */
  kRenderTarget,
  /* State in which the resource can be used for unordered access */
  kUnorderedAccess,
  /* State in which the resource can be used as a present source */
  kPresent,
  /* State in which the resource can be the source of a copy operation */
  kCopySrc,
  /* State in which the resource can be the target of a copy operation */
  kCopyDst
};

}

// ========================================================================== //
// ShaderKind Enumeration
// ========================================================================== //

namespace olivine {

/** \class ShaderStage
 * \author Filip Björklund
 * \date 26 september 2019 - 20:50
 * \brief Shader stages.
 * \details
 * Enumeration of shader stages.
 */
enum class ShaderStage : u32
{
  /* No shader stage */
  kNone = 0,
  /* Vertex shader */
  kVertex = Bit(0u),
  /* Pixel shader */
  kPixel = Bit(1u),
  /* Compute shader */
  kCompute = Bit(2u),
  /* All shader kinds combined */
  kAll = kVertex | kPixel | kCompute
};

// -------------------------------------------------------------------------- //

inline ShaderStage
operator|(ShaderStage lhs, ShaderStage rhs)
{
  return static_cast<ShaderStage>(static_cast<u32>(lhs) |
                                  static_cast<u32>(rhs));
}

// -------------------------------------------------------------------------- //

inline ShaderStage operator&(ShaderStage lhs, ShaderStage rhs)
{
  return static_cast<ShaderStage>(static_cast<u32>(lhs) &
                                  static_cast<u32>(rhs));
}

// -------------------------------------------------------------------------- //

inline ShaderStage&
operator|=(ShaderStage& lhs, ShaderStage rhs)
{
  lhs = (lhs | rhs);
  return lhs;
};

}

// ========================================================================== //
// PrimitiveTopology Enumeration
// ========================================================================== //

namespace olivine {

/** \enum PrimitiveTopology
 * \author Filip Björklund
 * \date 27 september 2019 - 17:47
 * \brief Primitive topologies.
 * \details
 * Enumeration of primitive topologies.
 */
enum class PrimitiveTopology : u32
{
  /* List of triangles */
  kTriangleList
};

}

// ========================================================================== //
// ComparisonFunction
// ========================================================================== //

namespace olivine {

/** \class ComparisonFunction
 * \author Filip Björklund
 * \date 28 september 2019 - 18:45
 * \brief Comparison functions.
 * \details
 * Enumeration of comparison functions.
 */
enum class ComparisonFunction : u32
{
  /* Comparisons never pass */
  kNever,
  /* Comparisons always pass */
  kAlways,
  /* Comparisons pass if the source is less than the destination */
  kLess,
  /* Comparisons pass if the source is greater than the destination */
  kGreater,
  /* Comparisons pass if the source is less than or equal to the destination */
  kLessEqual,
  /* Comparisons pass if the source is greater than or equal to the
   * destination */
  kGreaterEqual,
  /* Comparison pass if the source and destination are equal */
  kEqual,
  /* Comparison pass if the source and destination are not equal */
  kNotEqual,
};

}

// ========================================================================== //
// WindingOrder Enumeration
// ========================================================================== //

namespace olivine {

/** \enum WindingOrder
 * \author Filip Björklund
 * \date 29 september 2019 - 14:12
 * \brief
 * \details
 * Enumeration of winding orders for geometry. Clockwise means that front-faces
 * of the geometry are have vertices that are wound (layed out) in clockwise
 * order. Counter clockwise is the opposite.
 */
enum class WindingOrder : u32
{
  /* Front-faces have clockwise wound vertices */
  kClockwise,
  /* Front-faces have counter-clockwise wound vertices */
  kCounterClockwise,
};

}

// ========================================================================== //
// CullMode Enumeration
// ========================================================================== //

namespace olivine {

/** \enum CullMode
 * \author Filip Björklund
 * \date 29 september 2019 - 14:10
 * \brief Culling modes.
 * \details
 * Enumeration of culling modes.
 */
enum class CullMode : u32
{
  /* Cull no faces. Everything is rendered */
  kNone = 0,
  /* Cull front faces (What constitutes a front face is determined by the
   * winding order).*/
  kFront,
  /* Cull back faces. These are the opposite of the front faces */
  kBack
};

}

// ========================================================================== //
// Viewport Declaration
// ========================================================================== //

namespace olivine {

/** \class Viewport
 * \author Filip Björklund
 * \date 26 september 2019 - 12:50
 * \brief Viewport.
 * \details
 * Represents a viewport.
 */
class Viewport
{
public:
  /* Width of viewport */
  f32 width;
  /* Height of viewport */
  f32 height;
  /* Top-left corner X */
  f32 topLeftX;
  /* Top-left corner Y */
  f32 topLeftY;
  /* Minimum depth */
  f32 minDepth;
  /* Maximum depth */
  f32 maxDepth;

public:
  /** Make a viewport with the specified parameters.
   * \brief Make viewport.
   * \param width Width of viewport.
   * \param height Height of viewport.
   * \param topLeftX Top left corner x coordinate.
   * \param topLeftY Top left corner y coordinate.
   * \param minDepth Minimum depth.
   * \param maxDepth Maximum depth.
   * \return Created viewport.
   */
  static Viewport Make(f32 width,
                       f32 height,
                       f32 topLeftX = 0.0f,
                       f32 topLeftY = 0.0f,
                       f32 minDepth = 0.0f,
                       f32 maxDepth = 1.0f)

  {
    return Viewport{ width, height, topLeftX, topLeftY, minDepth, maxDepth };
  }
};

}

// ========================================================================== //
// Rectangle Declaration
// ========================================================================== //

namespace olivine {

/** \class Rectangle
 * \author Filip Björklund
 * \date 26 september 2019 - 12:51
 * \brief Rectangle.
 * \details
 * Represents a rectangle. This can be used for setting the scissor rectangle.
 */
class Rectangle
{
public:
  /* Right edge */
  u32 right;
  /* Bottom edge */
  u32 bottom;
  /* Left edge */
  u32 left;
  /* Top edge */
  u32 top;

public:
  /** Make a rectangle with the specified edges.
   * \brief Make rectangle.
   * \param right Right edge.
   * \param bottom Bottom edge.
   * \param left Left edge.
   * \param top Top edge.
   * \return Created Rectangle.
   */
  static Rectangle Make(u32 right, u32 bottom, u32 left = 0, u32 top = 0)
  {
    return Rectangle{ right, bottom, left, top };
  }
};

}