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

#include "olivine/render/api/semaphore.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/app/app.hpp"
#include "olivine/render/api/device.hpp"

// ========================================================================== //
// Semaphore Implementation
// ========================================================================== //

namespace olivine {

Semaphore::Semaphore(u64 initialValue)
{
  Device* device = App::Instance()->GetDevice();

  // Create fence
  const HRESULT hresult =
    device->GetHandle()->CreateFence(initialValue,
                                     D3D12_FENCE_FLAG_NONE,
                                     __uuidof(ID3D12Fence),
                                     reinterpret_cast<void**>(&mHandle));
  Assert(SUCCEEDED(hresult), "Failed to create semaphore");

  // Create event
  mEvent = CreateEventW(nullptr, false, false, nullptr);
  Assert(mEvent != nullptr, "Failed to create semaphore event");
}

// -------------------------------------------------------------------------- //

Semaphore::~Semaphore()
{
  // Close event handle
  CloseHandle(mEvent);
  mEvent = nullptr;

  // Release handle
  mHandle->Release();
  mHandle = nullptr;
}

// -------------------------------------------------------------------------- //

void
Semaphore::Signal(u64 value)
{
  const HRESULT hresult = mHandle->Signal(value);
  Assert(SUCCEEDED(hresult), "Failed to signal semaphore");
}

// -------------------------------------------------------------------------- //

void
Semaphore::Wait(u64 value) const
{
  if (GetValue() < value) {
    const HRESULT hresult = mHandle->SetEventOnCompletion(value, mEvent);
    Assert(SUCCEEDED(hresult), "Failed to set semaphore completion event");

    const DWORD result = WaitForSingleObject(mEvent, INFINITE);
    Assert(result == WAIT_OBJECT_0, "Failed to wait for semaphore event");
  }
}

// -------------------------------------------------------------------------- //

u64
Semaphore::GetValue() const
{
  return mHandle->GetCompletedValue();
}

// -------------------------------------------------------------------------- //

void
Semaphore::SetName(const String& name)
{
  D3D12Util::SetName(mHandle, name);
}

}
