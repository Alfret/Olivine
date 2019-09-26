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
#include "olivine/core/platform/headers.hpp"
#include "olivine/core/types.hpp"
#include "olivine/core/string.hpp"
#include "olivine/render/format.hpp"
#include "olivine/render/api/d3d12_util.hpp"
#include "olivine/render/api/descriptor.hpp"

// ========================================================================== //
// SwapChain Declaration
// ========================================================================== //

namespace olivine {

OL_FORWARD_DECLARE(CommandQueue);
OL_FORWARD_DECLARE(Texture);

/** \class SwapChain
 * \author Filip Björklund
 * \date 25 september 2019 - 20:08
 * \brief Swap chain.
 * \details
 * Represents a swap chain.
 */
class SwapChain
{
public:
  /* Number of buffers used by swap chain */
  static constexpr u32 kBufferCount = 3;

  /* Creation information */
  struct CreateInfo
  {
    /* Handle of window to create swap chain for */
    HWND window;
    /* Presentation queue */
    CommandQueue* presentQueue;

    /* Width of buffers */
    u32 bufferWidth;
    /* Height of buffers */
    u32 bufferHeight;
    /* Format of the buffer textures */
    Format format;
  };

private:
  /* Handle */
  IDXGISwapChain4* mHandle = nullptr;
  /* Creation flags */
  UINT mFlags = 0;

  /* Queue that presentation is made to */
  CommandQueue* mPresentQueue = nullptr;

  /* Buffer format (DXGI_FORMAT) */
  DXGI_FORMAT mFormatDXGI;
  /* Buffer format (Format) */
  Format mFormat;

  /* Current buffer index */
  u32 mIndex = 0;
  /* Buffer width */
  u32 mWidth;
  /* Buffer height */
  u32 mHeight;

  /* Textures for the buffer */
  Texture* mBuffers[kBufferCount];
  /* Render-targets descriptor heap */
  DescriptorHeap* mHeapRTV;

  /* Name that is set. Used to re-set after resize */
  String mName;

public:
  /** Create a swap chain from a creation information structure.
   * \brief Create swap chain.
   * \param createInfo Creation information.
   */
  explicit SwapChain(const CreateInfo& createInfo);

  /** Destroy the swap chain.
   * \note The user is responsible for making sure the swap chain buffers are
   * not in use by the graphics device when the swap chain (including the
   * buffers themselves) are being destroyed.
   * \brief Destroy swap chain.
   */
  ~SwapChain();

  /** Present the next buffer of the swap chain to the associated window.
   * \brief Present.
   */
  void Present();

  /** Resize the buffers of the swap chain to the specified width and height.
   * \brief Resize swap chain buffers.
   * \param width Width to resize buffers to.
   * \param height Height to resize buffers to.
   */
  void Resize(u32 width, u32 height);

  /** Returns the current buffer index for the swap chain. This index should be
   * used to pick the correct buffer to use as a render target.
   * \brief Returns buffer index.
   * \return Current buffer index.
   */
  u32 Index() const { return mIndex; }

  /** Returns the buffer at the specified index in the chain
   * \brief Returns buffer at index.
   * \param index Index of buffer.
   * \return Buffer.
   */
  Texture* Buffer(u32 index) const { return mBuffers[index]; }

  /** Returns the buffer that is current.
   * \brief Returns current buffer.
   * \return Current buffer
   */
  Texture* CurrentBuffer() const { return Buffer(mIndex); }

  /** Returns the render-target descriptor for the buffer at the specified
   * index.
   * \brief Returns RTV for buffer index.
   * \param index Buffer index.
   * \return RTV.
   */
  Descriptor RT(u32 index) const { return mHeapRTV->At(index); }

  /** Returns the current render-target descriptor.
   * \brief Returns current RTV.
   * \return Current RTV.
   */
  Descriptor CurrentRT() const { return RT(mIndex); };

  /** Set the name of the swap chain.
   * \brief Set name.
   * \param name Name to set.
   */
  void SetName(const String& name);

private:
  /** Setup the buffer textures and their RTVs **/
  void SetupBuffers();
};

}