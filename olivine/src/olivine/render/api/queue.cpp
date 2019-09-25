// MIT License
//
// Copyright (c) 2019 Filip Bj�rklund
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

// ========================================================================== //
// CommandQueue Implementation
// ========================================================================== //

namespace olivine {

CommandQueue::CommandQueue(Type type)
  : mType(type)
{
  D3D12_COMMAND_QUEUE_DESC desc{};
  desc.Type = ToCommandListType(type);
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
CommandQueue::SignalSemaphore(Semaphore* semaphore, u64 value) const
{
  HRESULT hresult = mHandle->Signal(semaphore->GetHandle(), value);
  Assert(SUCCEEDED(hresult), "Failed to submit command to signal semaphore");
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
CommandQueue::ToCommandListType(Type type)
{
  switch (type) {
    case Type::kGraphics: {
      return D3D12_COMMAND_LIST_TYPE_DIRECT;
    }
    case Type::kCompute: {
      return D3D12_COMMAND_LIST_TYPE_COMPUTE;
    }
    case Type::kCopy: {
      return D3D12_COMMAND_LIST_TYPE_COPY;
    }
    default: {
      return (D3D12_COMMAND_LIST_TYPE)UINT_MAX;
    }
  }
}
}
