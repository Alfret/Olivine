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
#include "olivine/render/api/d3d12_util.hpp"
#include "olivine/render/api/common.hpp"

// ========================================================================== //
// Texture Declaration
// ========================================================================== //

namespace olivine {

/** \class Texture
 * \author Filip Björklund
 * \date 25 september 2019 - 21:18
 * \brief Texture.
 * \details
 * Represents a texture.
 */
class Texture
{
public:
  /* Texture dimensions */
  enum class Dim : u32
  {
    /* 1-dimensional texture */
    k1D,
    /* 2-dimensional texture */
    k2D,
    /* 3-dimensional texture */
    k3D
  };

  /* Texture usages */
  enum class Usage : u32
  {
    /* Invalid usage */
    kInvalid = 0,
    /* Texture will be used as a color attachment */
    kColorAttachment = Bit(0u),
    /* Texture will be used as a depth-stencil attachment */
    kDepthStencilAttachment = Bit(1u),
    /* Texture will be used as a shader resource */
    kShaderResource = Bit(2u)
  };
  OL_ENUM_CLASS_OPERATORS(friend, Usage, u32);

  /* Requirements for storing texture data in a buffer */
  struct BufferRequirements
  {
    /* Minimum size of the buffer */
    u64 size;
    /* Minimum alignment of the buffer */
    u64 alignment;
    /* Stride for data of each row */
    u64 rowStride;
  };

  /* Creation information */
  struct CreateInfo
  {
    /* Width */
    u32 width;
    /* Height */
    u32 height;
    /* Depth layers */
    u32 depth = 1;
    /* Array size */
    u32 arraySize = 1;
    /* Mip levels */
    u32 mipLevels = 1;
    /* Dimension */
    Dim dimension = Dim::k2D;
    /* Format */
    Format format = Format::kInvalid;
    /* Usage */
    Usage usages = Usage::kInvalid;
    /* Memory heap kind */
    HeapKind heapKind;
  };

private:
  /* Texture resource */
  ID3D12Resource* mResource = nullptr;

  /* Width of texture */
  u32 mWidth;
  /* Height of texture */
  u32 mHeight;
  /* Depth */
  u32 mDepth;
  /* Dimension of texture */
  Dim mDimension;
  /* Format of texture  */
  Format mFormat;
  /* Usages of texture */
  Usage mUsages;

  /* Allocation */
  D3D12MA::Allocation* mAllocation = nullptr;

public:
  /** Create a texture from creation information.
   * \brief Create texture.
   * \param createInfo Creation information.
   */
  explicit Texture(const CreateInfo& createInfo);

  /** Create a texture from an existing resource.
   * \brief Create texture from resource.
   * \param resource Resource.
   * \param width Width.
   * \param height Height.
   * \param depth Depth.
   * \param dimension Dimension of texture.
   * \param format Format of texture.
   * \param usages Usages for texture.
   */
  Texture(ID3D12Resource* resource,
          u32 width,
          u32 height,
          u32 depth,
          Dim dimension,
          Format format,
          Usage usages);

  /** Destruct the texture.
   * \brief Destruct texture.
   */
  ~Texture();

  /** Returns the width of the texture.
   * \brief Returns width.
   * \return Width.
   */
  u32 GetWidth() const { return mWidth; }

  /** Returns the height of the texture.
   * \brief Returns height.
   * \return Height.
   */
  u32 GetHeight() const { return mHeight; }

  /** Returns the format of the texture.
   * \brief Returns format.
   * \return Format.
   */
  Format GetFormat() const { return mFormat; }

  /** Returns the requirements for a buffer that can hold data that can be used
   * by this texture.
   * \brief Returns buffer requirements.
   * \return Buffer requirements.
   */
  BufferRequirements GetBufferRequirements() const;

  /** Returns the resource for the texture.
   * \brief Returns resource.
   * \return Resource handle.
   */
  ID3D12Resource* GetResource() const { return mResource; }

  /** Set the name of the texture.
   * \brief Set name.
   * \param name Name to set.
   */
  void SetName(const String& name);

public:
  /** Convert to resource dimension **/
  static D3D12_RESOURCE_DIMENSION ToResourceDim(Dim dimension);
};

}