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
#include "olivine/render/api/semaphore.hpp"
#include "olivine/render/api/d3d12_util.hpp"

// ========================================================================== //
// Queue Declaration
// ========================================================================== //

namespace olivine {

OL_FORWARD_DECLARE(CommandList);

/** \class CommandQueue
 * \author Filip Björklund
 * \date 25 september 2019 - 17:57
 * \brief Command queue.
 * \details
 * Represents a command queue.
 */
class CommandQueue
{
public:
  /* Command queue kinds */
  enum class Kind
  {
    /* Graphics queue. Supports graphics, compute and copy commands */
    kGraphics,
    /* Compute queue. Supports compute and copy commands */
    kCompute,
    /* Copy queue. Supports copy commands */
    kCopy
  };

private:
  /* Command queue handle */
  ID3D12CommandQueue* mHandle = nullptr;
  /* Kind of the command queue */
  Kind mKind;
  /* Semaphore for flushing the queue */
  Semaphore mSemaphore;

public:
  /** Create a command queue of the specified kind.
   * \brief Create command queue.
   */
  explicit CommandQueue(Kind kind);

  /** Destroy the command queue.
   * \note The user is responsible for making sure no work is being done or has
   * been scheduled on the command queue when it's being destroyed.
   * \brief Destroy command queue.
   */
  ~CommandQueue();

  /** Submit a command list for execution on the device with the specified
   * command queue.
   * \brief Submit command list.
   * \param list Command list to submit.
   */
  void Submit(const CommandList* list);

  /** Signal a semaphore with the specified value on the device.
   * \note This will not immediately signal the semaphore but rather insert a
   * command into the queue that will be executed on the device to set the value
   * from the GPU timeline.
   * \brief Signal semaphore.
   * \param semaphore Semaphore to signal.
   * \param value Value to signal semaphore with.
   */
  void SignalSemaphore(Semaphore* semaphore, u64 value) const;

  /** Flush the command queue by waiting for all the pending and executing
   * commands to finish running.
   * \brief Flush command queue.
   */
  void Flush();

  /** Returns the kind of the command queue.
   * \brief Returns kind.
   * \return Command queue kind.
   */
  Kind GetKind() const { return mKind; }

  /** Set the name of the command queue.
   * \brief Set name.
   * \param name Name to set.
   */
  void SetName(const String& name);

  /** Returns the handle of the command queue.
   * \brief Returns handle.
   * \return Handle.
   */
  ID3D12CommandQueue* GetHandle() const { return mHandle; }

public:
  /** Convert to command-list type **/
  static D3D12_COMMAND_LIST_TYPE ToCommandListType(Kind kind);
};

}