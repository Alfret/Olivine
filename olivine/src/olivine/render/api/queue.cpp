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

#include "olivine/render/api/queue.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/app/app.hpp"
#include "olivine/render/api/device.hpp"
#include "olivine/render/api/command_list.hpp"

// ========================================================================== //
// CommandQueue Implementation
// ========================================================================== //

namespace olivine {

CommandQueue::CommandQueue(Kind kind)
  : mKind(kind)
{
  D3D12_COMMAND_QUEUE_DESC desc{};
  desc.Type = ToCommandListType(kind);
  desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
  desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
  desc.NodeMask = 0x00;

  Device* device = App::Instance()->GetDevice();
  const HRESULT hresult = device->GetHandle()->CreateCommandQueue(
    &desc, __uuidof(ID3D12CommandQueue), (void**)&mHandle);
  Assert(SUCCEEDED(hresult), "Failed to create command queue");
}

// -------------------------------------------------------------------------- //

CommandQueue::~CommandQueue()
{
  // Release handle
  mHandle->Release();
  mHandle = nullptr;
}

// -------------------------------------------------------------------------- //

void
CommandQueue::Submit(const CommandList* list)
{
#if defined(_DEBUG)
  if (list->GetKind() == Kind::kGraphics) {
    Assert(mKind == Kind::kGraphics,
           "Graphics command lists can only be submitted for execution on a "
           "graphics command queue");
  } else if (list->GetKind() == Kind::kCompute) {
    Assert(mKind == Kind::kGraphics || mKind == Kind::kCompute,
           "Compute command lists can only be submitted for execution on a "
           "graphics or compute command queue");
  }
#endif

  ID3D12CommandList* lists[] = { list->GetHandle() };
  mHandle->ExecuteCommandLists(1, lists);
}

// -------------------------------------------------------------------------- //

void
CommandQueue::SignalSemaphore(Semaphore* semaphore, u64 value) const
{
  const HRESULT hresult = mHandle->Signal(semaphore->GetHandle(), value);
  Assert(SUCCEEDED(hresult), "Failed to submit command to signal semaphore");
}

// -------------------------------------------------------------------------- //

void
CommandQueue::WaitSemaphore(Semaphore* semaphore, u64 value) const
{
  const HRESULT hresult = mHandle->Wait(semaphore->GetHandle(), value);
  Assert(SUCCEEDED(hresult), "Failed to submit command to wait on semaphore");
}

// -------------------------------------------------------------------------- //

void
CommandQueue::Upload(CommandList* list,
                     Texture* dst,
                     Buffer* src,
                     u64 srcOffset)
{
  list->Reset();
  list->Copy(dst, src, srcOffset);
  list->Close();
  Submit(list);
  Flush();
}

// -------------------------------------------------------------------------- //

void
CommandQueue::Flush()
{
  const u64 nextValue = mSemaphore.GetValue() + 1;
  SignalSemaphore(&mSemaphore, nextValue);
  mSemaphore.Wait(nextValue);
}

// -------------------------------------------------------------------------- //

void
CommandQueue::SetName(const String& name)
{
  D3D12Util::SetName(mHandle, name);
  mSemaphore.SetName(name + "FlushSem");
}

// -------------------------------------------------------------------------- //

D3D12_COMMAND_LIST_TYPE
CommandQueue::ToCommandListType(Kind kind)
{
  switch (kind) {
    case Kind::kGraphics: {
      return D3D12_COMMAND_LIST_TYPE_DIRECT;
    }
    case Kind::kCompute: {
      return D3D12_COMMAND_LIST_TYPE_COMPUTE;
    }
    case Kind::kCopy: {
      return D3D12_COMMAND_LIST_TYPE_COPY;
    }
    default: {
      return (D3D12_COMMAND_LIST_TYPE)UINT_MAX;
    }
  }
}
}
