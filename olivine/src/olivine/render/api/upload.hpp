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
#include "olivine/math/literals.hpp"
#include "olivine/render/api/command_list.hpp"
#include "olivine/render/api/buffer.hpp"

// ========================================================================== //
// UploadManager Declaration
// ========================================================================== //

namespace olivine {
class Image;

OL_FORWARD_DECLARE(CommandQueue);

/** \class UploadManager
 * \author Filip Björklund
 * \date 28 september 2019 - 13:36
 * \brief Upload manager.
 * \details
 * Represents an upload manager that is used to upload data to GPU memory.
 */
class UploadManager
{
public:
  /* Size of upload buffer */
  static constexpr u64 kBufferSize = 64_MiB;

  /* Monotonically increasing temporary buffer id */
  static u64 sNextTempBuffer;

private:
  /* Command list for uploading */
  CommandList mList;
  /* Upload stack buffer */
  Buffer mBuffer;
  /* Upload semaphore */
  Semaphore mSem;
  /* Semaphore value to wait on for uploads */
  u64 mSemValue = 0;
  /* Current offset in upload buffer. This must be aligned to maximum offset for
   * worst case (common) scenarios */
  u64 mOffset = 0;

  /* Whether a sessions is currently active */
  bool mHasActiveSessions = false;
  /* Command queue for current sessions. Null if no session is active */
  CommandQueue* mSessionQueue = nullptr;

public:
  UploadManager();

  /** Begin a new upload session with the specified command queue.
   *
   * During a sessions the data buffer gets filled. Actual uploading is either
   * done when the buffer is full or the user ends the session.
   *
   * \brief Begin upload sessions.
   */
  void Begin(CommandQueue* queue);

  /** End the current upload session. At this point the uploads will be
   * submitted for execution on the GPU. Values returned from the various
   * 'Upload' calls will be signalled as the uploads are being completed.
   * \brief End session.
   */
  void End();

  /** Add an upload request for the current sessions. This upload will do a copy
   * from the 'src' buffer to the 'dst' buffer. Offsets can be specified to do
   * the copy at offset locations in the 'src' and/or 'dst' buffers.
   * \brief Add upload request.
   * \param dst Destination buffer.
   * \param src Source buffer.
   * \param size Size of region to copy. If size is zero (0) then the entire
   * source buffer is copied. This means the destination buffer must be large
   * enough to support this copy.
   * \param dstOffset Offset in destination buffer to copy to.
   * \param srcOffset Offset in source buffer to copy from.
   * \return Value to wait for in order to make sure this request is done. This
   * will not be signalled until the session ends or if the upload buffer was
   * filled.
   */
  u64 Upload(Buffer* dst,
             Buffer* src,
             u64 size,
             u64 dstOffset = 0,
             u64 srcOffset = 0);

  /** Wait for all uploads to complete.
   * \brief Wait for uploads.
   */
  void Wait();

  /** Returns the semaphore that the upload manager uses keep track of uploads.
   * The user can use this semaphore to wait on uploads to complete. The values
   * to wait for are returned from the respective calls to 'Upload'.
   * \brief Returns upload semaphore.
   * \return Semaphore for managing uploads.
   */
  const Semaphore& GetSemaphore() const { return mSem; }

public:
  /** Utility function for quickly uploading data from an image to a texture.
   * \note This function is not very performant as it will block until the
   * upload has been completed and creates a lot of temporary resources. It also
   * does not work well with command lists that are used for other things.
   * \note This function flushes the command queue to ensure that uploading is
   * complete.
   * \brief Upload image to texture.
   * \param queue Command queue to perform upload with.
   * \param list Command list to perform upload with.
   * \param dst Destination texture.
   * \param src Source image.
   */
  static void Upload(CommandQueue* queue,
                     CommandList* list,
                     Texture* dst,
                     Image* src);

  /** Utility function for quickly uploading data from a memory to a buffer.
   * \note This function is not very performant as it will block until the
   * upload has been completed and creates a lot of temporary resources. It also
   * does not work well with command lists that are used for other things.
   * \note This function flushes the command queue to ensure that uploading is
   * complete.
   * \brief Upload memory to buffer.
   * \param queue Command queue to perform upload with.
   * \param list Command list to perform upload with.
   * \param dst Destination buffer.
   * \param src Source memory.
   * \param size Size of memory to upload, in bytes.
   * \param dstOffset Offset in destination buffer to upload to.
   */
  static void Upload(CommandQueue* queue,
                     CommandList* list,
                     Buffer* dst,
                     u8* src,
                     u64 size,
                     u64 dstOffset = 0);
};

}