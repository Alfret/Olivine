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

#include "olivine/core/image.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/core/assert.hpp"
#include "olivine/core/file/file_io.hpp"
#include "olivine/core/file/file_system.hpp"
#include "olivine/render/color.hpp"

// stb_image header
#define STBI_ASSERT(x) OL_ASSERT(!!(x), "Assertion failed in library stb_image")
#define STBI_NO_STDIO
#define STB_IMAGE_IMPLEMENTATION
#include <thirdparty/stb/stb_image.h>

// stb_image_write header
#define STBIW_ASSERT(x)                                                        \
  OL_ASSERT(!!(x), "Assertion failed in library stb_image")
#define STB_IMAGE_WRITE_IMPLEMENTATION
#if defined(ALFLIB_TARGET_WINDOWS)
#define STBI_MSC_SECURE_CRT
#endif
#include <thirdparty/stb/stb_image_write.h>

// stb_image_resize header
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STBIR_ASSERT(x)                                                        \
  OL_ASSERT(!!(x), "Assertion failed in library stb_image_resize")
#include <thirdparty/stb/stb_image_resize.h>

// ========================================================================== //
// Image Implementation
// ========================================================================== //

namespace olivine {

Image::~Image()
{
  // Free image data
  Memory::Free(mData);
  mData = nullptr;
}

// -------------------------------------------------------------------------- //

Image::Result
Image::Create(const CreateInfo& createInfo)
{
  mWidth = createInfo.width;
  mHeight = createInfo.height;
  mFormat = createInfo.format;
  mDataSize = u64(GetFormatRowStride(mFormat, mWidth)) * u64(mHeight);
  mData = static_cast<u8*>(Memory::Allocate(mDataSize));

  return Result::kSuccess;
}

// -------------------------------------------------------------------------- //

Image::Result
Image::Load(const Path& path)
{
  // Read file into buffer
  u64 size = FileSystem::GetSize(path);
  FileIO io(path);
  FileResult result = io.Open(FileIO::Flag::kRead);
  if (result != FileResult::kSuccess) {
    return Result::kFailedToReadFile;
  }
  u8* buffer = static_cast<u8*>(Memory::Allocate(size));
  if (!buffer) {
    return Result::kFailedToReadFile;
  }
  result = io.Read(buffer, size);
  if (result != FileResult::kSuccess) {
    Memory::Free(buffer);
    return Result::kFailedToReadFile;
  }

  // Load image data
  s32 x, y, c;
  stbi_uc* data =
    stbi_load_from_memory(buffer, (int)size, &x, &y, &c, STBI_default);
  Memory::Free(buffer);
  if (!data) {
    return Result::kFailedToLoadData;
  }

  // Determine format
  mWidth = u32(x);
  mHeight = u32(y);
  mFormat = Format::kUnknown;
  if (c == 1) {
    mFormat = Format::kRed;
  } else if (c == 3) {
    mFormat = Format::kRGB;
  } else if (c == 4) {
    mFormat = Format::kRGBA;
  }

  // Copy data
  mDataSize = u64(GetFormatRowStride(mFormat, mWidth)) * u64(mHeight);
  mData = static_cast<u8*>(Memory::Allocate(mDataSize));
  if (!mData) {
    stbi_image_free(data);
    return Result::kFailedToLoadData;
  }
  Memory::Copy(mData, data, mDataSize);
  stbi_image_free(data);

  // Success
  return Result::kSuccess;
}

// -------------------------------------------------------------------------- //

Image
Image::Copy() const
{
  u8* data = static_cast<u8*>(Memory::Allocate(mDataSize));
  Memory::Copy(data, mData, mDataSize);
  return Image{ mWidth, mHeight, mFormat, data, mDataSize };
}

// -------------------------------------------------------------------------- //

Image::Result
Image::Resize(u32 width, u32 height, Filter filter)
{
  // Function to convert filter type
  auto ConvFilter = [](Filter _filter) -> stbir_filter {
    switch (_filter) {
      case Filter::kBox: {
        return STBIR_FILTER_BOX;
      }
      case Filter::kTriangle: {
        return STBIR_FILTER_TRIANGLE;
      }
      case Filter::kCubicSpline: {
        return STBIR_FILTER_CUBICBSPLINE;
      }
      case Filter::kCatmullRom: {
        return STBIR_FILTER_CATMULLROM;
      }
      case Filter::kMitchell: {
        return STBIR_FILTER_MITCHELL;
      }
      case Filter::kDefault: {
        return STBIR_FILTER_DEFAULT;
      }
      default: {
        Panic("Invalid filter type");
      }
    }
  };

  // Create resized data
  const u64 dataSize = u64(GetFormatRowStride(mFormat, width)) * u64(height);
  u8* data = static_cast<u8*>(Memory::Allocate(dataSize));
  const int success =
    stbir_resize_uint8_generic(mData,
                               mWidth,
                               mHeight,
                               0,
                               data,
                               width,
                               height,
                               0,
                               GetFormatChannelCount(mFormat),
                               GetFormatChannelCount(mFormat) == 4,
                               0,
                               STBIR_EDGE_CLAMP,
                               ConvFilter(filter),
                               STBIR_COLORSPACE_LINEAR,
                               NULL);
  if (!success) {
    Memory::Free(data);
    return Result::kUnknownError;
  }

  // Update data
  mWidth = width;
  mHeight = height;
  Memory::Free(mData);
  mData = data;
  mDataSize = dataSize;

  // Success
  return Result::kSuccess;
}

// -------------------------------------------------------------------------- //

Image::Result
Image::Blit(const Image& src,
            u32 dstOffsetX,
            u32 dstOffsetY,
            u32 srcOffsetX,
            u32 srcOffsetY,
            u32 width,
            u32 height)
{
  // Set the width and height if the special values (AN_U32_MAX) has been
  // specified
  if (width == Limits::kU32Max) {
    width = mWidth;
  }
  if (height == Limits::kU32Max) {
    height = mHeight;
  }

  // Assert preconditions
  Assert(srcOffsetX + width <= src.mWidth,
         "Source image X offset plus width must not be greater than "
         "the width of the source image");
  Assert(srcOffsetY + height <= src.mHeight,
         "Source image Y offset plus height must not be greater than "
         "the height of the source image");
  Assert(dstOffsetX + width <= mWidth,
         "Destination image X offset plus width must not be greater than "
         "the width of the destination image");
  Assert(dstOffsetY + height <= mHeight,
         "Destination image Y offset plus height must not be greater than "
         "the height of the destination image");

  // Fast-path?
  if (mFormat == src.mFormat) {
    // Copy rows
    for (u32 row = 0; row < height; row++) {
      const u32 bytesPerPixel = GetFormatChannelCount(mFormat);
      u8* dstPointer =
        mData + bytesPerPixel * (mWidth * (row + dstOffsetY) + dstOffsetX);
      u8* srcPointer =
        src.mData +
        bytesPerPixel * (src.mWidth * (row + srcOffsetY) + srcOffsetX);
      memcpy(dstPointer, srcPointer, bytesPerPixel * width);
    }
  } else { // Otherwise slow path (pixel-by-pixel)
    for (u32 y = 0; y < height; y++) {
      for (u32 x = 0; x < width; x++) {
        SetPixel(x + dstOffsetX,
                 y + dstOffsetY,
                 src.GetPixel(x + srcOffsetX, y + srcOffsetY));
      }
    }
  }

  // Success
  return Result::kSuccess;
}

// -------------------------------------------------------------------------- //

void
Image::Fill(Color color)
{
  for (u32 y = 0; y < mHeight; y++) {
    for (u32 x = 0; x < mWidth; x++) {
      SetPixel(x, y, color);
    }
  }
}

// -------------------------------------------------------------------------- //

Image::Result
Image::Save(const Path& path, bool overwrite, FileKind kind)
{
  // Context structure
  struct Context
  {
    FileIO& io;
    FileResult result;
  };

  // Write callback
  static auto WriteCallback = [](void* _context, void* data, int size) {
    auto context = static_cast<Context*>(_context);
    context->result = context->io.Write(static_cast<u8*>(data), u64(size));
  };

  // Setup context
  FileIO io(path);
  Context context{ io, FileResult::kSuccess };
  FileIO::Flag ioFlags = FileIO::Flag::kWrite | FileIO::Flag::kCreate;
  if (overwrite) {
    ioFlags |= FileIO::Flag::kOverwrite;
  }
  const FileResult result = io.Open(ioFlags);
  if (result != FileResult::kSuccess) {
    return Result::kUnknownError;
  }

  // Write file
  int writeSuccess = false;
  if (kind == FileKind::kPng) {
    writeSuccess = stbi_write_png_to_func(WriteCallback,
                                          &context,
                                          mWidth,
                                          mHeight,
                                          GetFormatChannelCount(mFormat),
                                          mData,
                                          GetStride());
  } else if (kind == FileKind::kTga) {
    writeSuccess = stbi_write_tga_to_func(WriteCallback,
                                          &context,
                                          mWidth,
                                          mHeight,
                                          GetFormatChannelCount(mFormat),
                                          mData);
  }
  if (!writeSuccess) {
    return Result::kUnknownError;
  }

  // Check result
  if (context.result != FileResult::kSuccess) {
    return Result::kUnknownError;
  }
  return Result::kSuccess;
}

// -------------------------------------------------------------------------- //

Color
Image::GetPixel(u32 x, u32 y) const
{
  // Retrieve data
  const u64 _x = (u64)x;
  const u64 _y = (u64)y;
  const u32 bytesPerPixel = GetFormatChannelCount(mFormat);
  const u8* address =
    mData + (_y * mWidth * bytesPerPixel) + (_x * bytesPerPixel);

  // Set pixel
  switch (mFormat) {
    // Format:: RGBA
    case Format::kRGBA: {
      return Color(u32(address[0]), address[1], address[2], address[3]);
    }
    // Format:: RGBA
    case Format::kBGRA: {
      return Color(u32(address[2]), address[1], address[0], address[3]);
    }
    // Format:: RGBA
    case Format::kRGB: {
      return Color(u32(address[0]), address[1], address[2]);
    }
    // Format:: RGBA
    case Format::kBGR: {
      return Color(u32(address[2]), address[1], address[0]);
    }
    // Format: Red
    case Format::kRed: {
      return Color(address[0], 0u, 0u, 0u);
    }
    // Format: Alpha
    case Format::kAlpha: {
      return Color(0u, 0u, 0u, address[0]);
    }
    // Default
    case Format::kUnknown: {
      return Color(0u, 0u, 0u, 0u);
    }
    default: {
      Panic("Invalid image format");
    }
  }
}

// -------------------------------------------------------------------------- //

void
Image::SetPixel(u32 x, u32 y, Color color)
{
  // Retrieve data
  const u64 _x = (u64)x;
  const u64 _y = (u64)y;
  const u32 bytesPerPixel = GetFormatChannelCount(mFormat);
  u8* address = mData + (_y * mWidth * bytesPerPixel) + (_x * bytesPerPixel);

  // Set pixel
  switch (mFormat) {
    // Format:: RGBA
    case Format::kRGBA: {
      address[0] = color.Red();
      address[1] = color.Green();
      address[2] = color.Blue();
      address[3] = color.Alpha();
      break;
    }
    // Format:: RGBA
    case Format::kBGRA: {
      address[0] = color.Blue();
      address[1] = color.Green();
      address[2] = color.Red();
      address[3] = color.Alpha();
      break;
    }
    // Format:: RGBA
    case Format::kRGB: {
      address[0] = color.Red();
      address[1] = color.Green();
      address[2] = color.Blue();
      break;
    }
    // Format:: RGBA
    case Format::kBGR: {
      address[0] = color.Blue();
      address[1] = color.Green();
      address[2] = color.Red();
      break;
    }
    // Format: Red
    case Format::kRed: {
      address[0] = color.Red();
      break;
    }
    // Format: Alpha
    case Format::kAlpha: {
      address[0] = color.Alpha();
      break;
    }
    // Default
    case Format::kUnknown: {
      break;
    }
    default: {
      Panic("Invalid image format");
    }
  }
}

// -------------------------------------------------------------------------- //

u32
Image::GetStride()
{
  return GetFormatRowStride(mFormat, mWidth);
}

// -------------------------------------------------------------------------- //

Image::Image(u32 width, u32 height, Format format, u8* data, u64 dataSize)
  : mWidth(width)
  , mHeight(height)
  , mFormat(format)
  , mData(data)
  , mDataSize(dataSize)
{}

// -------------------------------------------------------------------------- //

u32
Image::GetFormatChannelCount(Format format)
{
  switch (format) {
    case Format::kRGBA:
    case Format::kBGRA: {
      return 4;
    }
    case Format::kRGB:
    case Format::kBGR: {
      return 3;
    }
    case Format::kRed:
    case Format::kAlpha: {
      return 1;
    }
    case Format::kUnknown: {
      return 0;
    }
    default: {
      Panic("Invalid image format");
    }
  }
}

// -------------------------------------------------------------------------- //

u32
Image::GetFormatRowStride(Format format, u32 width)
{
  switch (format) {
    case Format::kRGBA:
    case Format::kBGRA: {
      return 4 * width;
    }
    case Format::kRGB:
    case Format::kBGR: {
      // TODO(Filip Björklund): stb_image does not pad?
      return 3 * width;
    }
    case Format::kRed:
    case Format::kAlpha: {
      return 1 * width;
    }
    case Format::kUnknown: {
      return 0 * width;
    }
    default: {
      Panic("Invalid image format");
    }
  }
}

}
