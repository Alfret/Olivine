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
#include "olivine/render/api/d3d12_util.hpp"
#include "olivine/render/api/queue.hpp"

// ========================================================================== //
// CommandList Declaration
// ========================================================================== //

namespace olivine {

OL_FORWARD_DECLARE(Texture);
OL_FORWARD_DECLARE(Descriptor);
OL_FORWARD_DECLARE(Color);
OL_FORWARD_DECLARE(Viewport);
OL_FORWARD_DECLARE(Rectangle);

/** \class CommandList
 * \author Filip Björklund
 * \date 26 september 2019 - 12:07
 * \brief
 * \details
 * Represents a command list for recording commands that can later be submitted
 * for execution on the GPU.
 */
class CommandList
{
private:
  /* List handle */
  ID3D12GraphicsCommandList* mHandle;
  /* Command allocator */
  ID3D12CommandAllocator* mAllocator;

  /* Kind of command queue that this list can be used with */
  CommandQueue::Kind mKind;

public:
  /** Create a command list for submitting commands to a command queue of the
   * kind specified.
   * \brief Create command list.
   * \param kind Kind of command queue that this command list will be used with.
   */
  explicit CommandList(CommandQueue::Kind kind);

  /** Destruct the command list.
   * \brief Destruct command list.
   */
  ~CommandList();

  /** Reset the command list to begin recording commands to it.
   * \note The user must make sure the list is not in use when this is called.
   * \brief Reset command list.
   */
  void Reset();

  /** Close the command list after recording commands to it. This will put the
   * command list into a state where it can be submitted for execution.
   * \brief Close command list.
   */
  void Close();

  /** Transition a resource between the states 'from' and 'to'.
   * \brief Transition resource state.
   * \param texture Texture to transition.
   * \param from State to transition from.
   * \param to State to transition to.
   */
  void TransitionResource(Texture* texture,
                          ResourceState from,
                          ResourceState to);

  /** Set the render target.
   * \brief Set render target.
   * \param descriptor Render-target view descriptor.
   */
  void SetRenderTarget(Descriptor descriptor);

  /** Clear a render target to the specified color.
   * \brief Clear render target.
   * \param descriptor Render-target view descriptor.
   * \param color Color to clear target to.
   */
  void ClearRenderTarget(Descriptor descriptor, Color color);

  /** Set the viewport.
   * \brief Set viewport.
   * \param viewport Viewport to set.
   */
  void SetViewport(Viewport viewport);

  /** Set the scissor rectangle.
   * \brief Set scissor rectangle.
   * \brief rectangle Scissor rectangle to set.
   */
  void SetScissorRectangle(Rectangle rectangle);

  /** Returns the kind of the command list.
   * \brief Returns the kind.
   * \return Kind.
   */
  CommandQueue::Kind GetKind() const { return mKind; }

  /** Set the name of the command list.
   * \brief Set name.
   * \param name Name to set.
   */
  void SetName(const String& name);

  /** Returns the command list handle.
   * \brief Returns handle.
   * \return Handle.
   */
  ID3D12GraphicsCommandList* GetHandle() const { return mHandle; }
};

}