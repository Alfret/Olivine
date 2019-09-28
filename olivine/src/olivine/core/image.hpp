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
#include "olivine/core/macros.hpp"
#include "olivine/math/limits.hpp"

// ========================================================================== //
// Image Declaration
// ========================================================================== //

namespace olivine {

OL_FORWARD_DECLARE(Path);
OL_FORWARD_DECLARE(Color);

/** \class Image
 * \author Filip Björklund
 * \date 28 september 2019 - 11:24
 * \brief Image.
 * \details
 * Represents a CPU-side image that can be loaded from file or created from
 * scratch.
 */
class Image
{
  OL_NO_COPY(Image);

public:
  /* Results */
  enum class Result
  {
    /* Success */
    kSuccess = 0,
    /* Unknown error */
    kUnknownError,
    /* Failed to open/read image file */
    kFailedToReadFile,
    /* Failed to load image data */
    kFailedToLoadData
  };

  /* Image formats */
  enum class Format
  {
    /* Unknown format */
    kUnknown = 0,
    /* 4-channel RGBA format */
    kRGBA,
    /* 4-channel BGRA format */
    kBGRA,
    /* 3-channel RGB format */
    kRGB,
    /* 3-channel BGR format */
    kBGR,
    /* 1-channel R format */
    kRed,
    /* 1-channel A format */
    kAlpha,
  };

  /* Image resizing filters */
  enum class Filter
  {
    /* Default filter */
    kDefault = 0,
    /* Box filter */
    kBox,
    /* Triangle filter */
    kTriangle,
    /* Cubic spline */
    kCubicSpline,
    /* Catmull-rom */
    kCatmullRom,
    /* Mitchell */
    kMitchell
  };

  /* Image file kinds */
  enum class FileKind
  {
    /* PNG */
    kPng,
    /* TGA */
    kTga
  };

  /* Creation information */
  struct CreateInfo
  {
    /* Width of the image in pixels */
    u32 width;
    /* Height of the image in pixels */
    u32 height;
    /* Format of the image */
    Format format;
  };

private:
  /** Width **/
  u32 mWidth = 0;
  /** Height **/
  u32 mHeight = 0;
  /** Format **/
  Format mFormat = Format::kUnknown;

  /** Data **/
  u8* mData = nullptr;
  /** Size of data **/
  u64 mDataSize = 0;

public:
  /** Construct an empty image with no data. This image cannot be used before
   * the user has either called 'Create' or 'Load' to initialize it with data.
   * \brief Construct empty image.
   */
  Image() = default;

  /** Destruct the image.
   * \brief Destruct image.
   */
  ~Image();

  /** Create an image from the specified creation information. The image will be
   * initialized with an empty data buffer. The entire data can be cleared to a
   * specific color by calling 'Fill' or by setting each pixel individually
   * using 'SetPixel'.
   * \brief Create texture.
   * \param createInfo Creation information.
   * \return Result.
   */
  Result Create(const CreateInfo& createInfo);

  /** Load data for the image from the file at the specified path.
   * \brief Load image.
   * \param path Path to the image file.
   * \return Result.
   */
  Result Load(const Path& path);

  /** Create and return a copy of the image.
   * \brief Copy image.
   * \return Copy of the image.
   */
  Image Copy() const;

  /** Resize the image to the specified width and height using the specified
   * filter for performing the sampling.
   * \brief Resize image.
   * \param width Width to resize to.
   * \param height Height to resize to.
   * \param filter Filter to use for sampling the image.
   */
  Result Resize(u32 width, u32 height, Filter filter = Filter::kDefault);

  /** Blit the a sub-part of the 'src' texture onto the image. The part is
   * specified by the offsets on each respective image and the width and height
   * of the sub-image to blit.
   * \brief Blit image.
   * \param src Source image.
   * \param dstOffsetX X offset on the destination image to blit at.
   * \param dstOffsetY Y offset on the destination image to blit at.
   * \param srcOffsetX X offset on the source image to blit from.
   * \param srcOffsetY Y offset on the source image to blit from.
   * \param width Width of the sub-image that is being copied. If this is set to
   * the maximum value of an 'u32' then the entire width of the source image is
   * used.
   * \param height Width of the sub-image that is being copied. If this is set
   * to the maximum value of an 'u32' then the entire height of the source image
   * is used.
   * \return Result.
   */
  Result Blit(const Image& src,
              u32 dstOffsetX,
              u32 dstOffsetY,
              u32 srcOffsetX,
              u32 srcOffsetY,
              u32 width = Limits::kU32Max,
              u32 height = Limits::kU32Max);

  /** Fill the entire image with a solid color.
   * \brief Fill image.
   * \param color Color to fill image with.
   */
  void Fill(Color color);

  /** Save the image to a file in the file system at the specified path.
   * \brief Save image.
   * \param path Path to save image at.
   * \param overwrite Whether to overwrite any existing file.
   * \param kind Kind of file to save image in.
   * \return Result.
   */
  Result Save(const Path& path,
              bool overwrite = false,
              FileKind kind = FileKind::kPng);

  /** Returns the color of the pixel at the specified 'x' and 'y' location in
   * the image specified from the top-left corned.
   * \brief Returns pixel color.
   * \param x X position to get pixel at (0 is left edge).
   * \param y Y position to get pixel at (0 is top edge).
   * \return Color of pixel.
   */
  Color GetPixel(u32 x, u32 y) const;

  /** Set the color of the pixel at the specified 'x' and 'y' location in the
   * image specified from the top-left corned.
   * \brief Set pixel color.
   * \param x X position to set pixel at (0 is left edge).
   * \param y Y position to set pixel at (0 is top edge).
   * \param color Color to set pixel to.
   */
  void SetPixel(u32 x, u32 y, Color color);

  /** Returns the width of the image in pixels.
   * \brief Returns width.
   * \return Width.
   */
  u32 GetWidth() const { return mWidth; }

  /** Returns the height of the image in pixels.
   * \brief Returns height.
   * \return Height.
   */
  u32 GetHeight() const { return mHeight; }

  /** Returns the format of the image.
   * \brief Returns format.
   * \retutn Image format.
   */
  Format GetFormat() const { return mFormat; }

  /** Returns the underlying image data.
   * \brief Returns data.
   * \return Image data.
   */
  u8* GetData() { return mData; }

  /** Returns the underlying image data.
   * \brief Returns data.
   * \return Image data.
   */
  const u8* GetData() const { return mData; }

  /** Returns the stride of the image data.
   * \brief Returns stride.
   * \return Data stride.
   */
  u32 GetStride();

private:
  /** Internal constructor to construct an image from existing data **/
  Image(u32 width, u32 height, Format format, u8* data, u64 dataSize);

public:
  /** Returns the number of channels that a specific image format has.
   * \brief Returns image format channel count.
   * \return Channel count for the specified format.
   */
  static u32 GetFormatChannelCount(Format format);

  /** Returns the image row-stride for a specified format and width of the row.
   * This is the number of bytes that a row in the image data takes up,
   * including any padding.
   * \brief Returns row stride.
   * \return Row stride.
   */
  static u32 GetFormatRowStride(Format format, u32 width);
};

}