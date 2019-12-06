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
#include "olivine/render/api/swap_chain.hpp"

// ========================================================================== //
// Renderer Declaration
// ========================================================================== //

namespace olivine {

OL_FORWARD_DECLARE(CommandList);
OL_FORWARD_DECLARE(Semaphore);
OL_FORWARD_DECLARE(ConstantBuffer);
OL_FORWARD_DECLARE(RootSignature);
OL_FORWARD_DECLARE(PipelineState);
OL_FORWARD_DECLARE(DescriptorHeap);

OL_FORWARD_DECLARE(Scene);

/** \class Renderer
 * \author Filip Björklund
 * \date 06 december 2019 - 15:59
 * \brief
 * \details
 */
class Renderer
{
private:
  /* Max entities that can be rendered in a scene */
  static constexpr u32 MAX_ENTITY = 128;
  /* Max lights that can be rendered in a scene */
  static constexpr u32 MAX_LIGHT = 16;

  /* Material descriptor start */
  static constexpr u32 DESC_START_MAT = 0;

  /* Per-frame resources */
  struct Frame
  {
    /* Constant buffer for per-model data */
    ConstantBuffer* modelCB;
    /* Constant buffer for light data */
    ConstantBuffer* lightCB;
  };

private:
  /* Frame resources */
  Frame mFrames[SwapChain::kBufferCount];

  /* GPU-visible descriptor heap */
  DescriptorHeap* mDescriptorHeap;

  /* Root signature for the PBR shader */
  RootSignature* mRootSignature;
  /* PBR forward shader */
  PipelineState* mPipelineState;

public:
  Renderer();

  ~Renderer();

  void Render(CommandList* list, const Scene* scene);

private:
  /* Setup the root signature and the PSO */
  void SetupPSO();
};

}