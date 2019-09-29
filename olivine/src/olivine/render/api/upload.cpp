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

#include "olivine/render/api/upload.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/core/assert.hpp"
#include "olivine/core/image.hpp"
#include "olivine/core/memory.hpp"
#include "olivine/math/math.hpp"
#include "olivine/render/api/texture.hpp"

// ========================================================================== //
// UploadManager Implementation
// ========================================================================== //

namespace olivine {

u64 UploadManager::sNextTempBuffer = 0;

// -------------------------------------------------------------------------- //

UploadManager::UploadManager()
  : mList(CommandQueue::Kind::kCopy)
  , mBuffer(kBufferSize, Buffer::Usage::kNone, HeapKind::kUpload)
{}

// -------------------------------------------------------------------------- //

void
UploadManager::Begin(CommandQueue* queue)
{
  // Assert preconditions
  Assert(!mHasActiveSessions,
         "Cannot begin a new upload session when one is already active");
  Assert(queue != nullptr,
         "Cannot begin an upload sessions with a command queue that is NULL");

  // Setup session
  mSessionQueue = queue;
  mOffset = 0;

  // Submit command to wait for previous session
  mSessionQueue->WaitSemaphore(&mSem, mSemValue++);

  // Begin session
  mHasActiveSessions = true;
}

// -------------------------------------------------------------------------- //

void
UploadManager::End()
{
  // Make sure there is an active session
  Assert(mHasActiveSessions,
         "Cannot end sessions when there is no active session currently");

  // Submit work
  mSessionQueue->Submit(&mList);
  mSessionQueue->SignalSemaphore(&mSem, mSemValue);

  // End session
  mHasActiveSessions = false;
}

// -------------------------------------------------------------------------- //

u64
UploadManager::Upload(Buffer* dst,
                      Buffer* src,
                      u64 size,
                      u64 dstOffset,
                      u64 srcOffset)
{
  // Check size
  if (size == 0) {
    Assert(dst->GetSize() >= src->GetSize(),
           "Destination buffer is not large enough to hold data after upload");
    size = src->GetSize();
  }

  // Align size
  const u64 alignment = AlignUp(u64(mOffset), src->GetAlignment());
  mOffset = alignment;

  // Copy data from source to upload buffer

  // Record copy from upload buffer to destination buffer
  mList.Copy(dst, &mBuffer, size, 0, mOffset);

  return 0;
}

// -------------------------------------------------------------------------- //

void
UploadManager::Wait()
{
  mSem.Wait(mSemValue);
}

// -------------------------------------------------------------------------- //

void
UploadManager::Upload(CommandQueue* queue,
                      CommandList* list,
                      Texture* dst,
                      Image* src)
{
  // Retrieve buffer requirements for texture
  const Texture::BufferRequirements bufferRequirements =
    dst->GetBufferRequirements();

  // Create upload buffer
  auto buffer = new Buffer(bufferRequirements.size,
                           Buffer::Usage::kNone,
                           HeapKind::kUpload,
                           bufferRequirements.alignment);
  buffer->SetName(String::Format("TmpUploadBuffer{}", sNextTempBuffer++));

  // Put data into digestible format
  u8* mapped = buffer->Map();
  for (u32 i = 0; i < src->GetHeight(); i++) {
    Memory::Copy(mapped + (bufferRequirements.rowStride * i),
                 src->GetData() + (src->GetStride() * i),
                 src->GetStride());
  }
  buffer->Unmap();

  // Upload data
  list->Reset();
  list->Copy(dst, buffer);
  list->Close();
  queue->Submit(list);
  queue->Flush();

  // Delete buffer
  delete buffer;
}

// -------------------------------------------------------------------------- //

void
UploadManager::Upload(CommandQueue* queue,
                      CommandList* list,
                      Buffer* dst,
                      u8* src,
                      u64 size,
                      u64 dstOffset)
{
  // Create upload buffer
  auto buffer = new Buffer(size, Buffer::Usage::kNone, HeapKind::kUpload);
  buffer->SetName(String::Format("TmpUploadBuffer{}", sNextTempBuffer++));
  buffer->Write(src, size);

  // Upload data
  list->Reset();
  list->Copy(dst, buffer, size, dstOffset, 0);
  list->Close();
  queue->Submit(list);
  queue->Flush();

  // Delete buffer
  delete buffer;
}

}
