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
#include "olivine/render/api/d3d12_util.hpp"

// ========================================================================== //
// Semaphore Declaration
// ========================================================================== //

namespace olivine {

/** \class Semaphore
 * \author Filip Björklund
 * \date 25 september 2019 - 18:10
 * \brief Timeline semaphore.
 * \details
 * Represents a timeline semaphore.
 */
class Semaphore
{
private:
  /* Fence handle that is used to represent the timeline semaphore. Their
   * workings are the same */
  ID3D12Fence* mHandle;
  /* Event that is being used to wait on the semaphore to reach a specified
   * value  */
  HANDLE mEvent;

public:
  /** Create a semaphore with the specified initial value
   * \brief Create semaphore.
   */
  explicit Semaphore(u64 initialValue = 0);

  /** Destroy the semaphore.
   * \brief Destroy semaphore.
   */
  ~Semaphore();

  /** Signal the semaphore with the specified value.
   * \brief Signal semaphore.
   * \param value Value to signal semaphore with.
   */
  void Signal(u64 value);

  /** Wait for semaphore to reach the specified value.
   * \brief Wait for semaphore.
   * \param value Value to wait for the semaphore to reach.
   */
  void Wait(u64 value) const;

  /** Returns the current value of the semaphore.
   * \brief Returns current semaphore value.
   * \return Current value.
   */
  u64 GetValue() const;

  /** Sets the name of the semaphore.
   * \brief Set name.
   * \param name Name to set.
   */
  void SetName(const String& name);

  /** Returns the handle of the semaphore.
   * \brief Returns handle.
   * \return Handle.
   */
  ID3D12Fence* GetHandle() const { return mHandle; }
};

}