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

// Standard headers
#include <unordered_map>

// Project headers
#include "olivine/core/types.hpp"
#include "olivine/core/macros.hpp"
#include "olivine/core/string.hpp"
#include "olivine/render/api/descriptor.hpp"

// ========================================================================== //
// Loader Declaration
// ========================================================================== //

namespace olivine {

OL_FORWARD_DECLARE(Model);
OL_FORWARD_DECLARE(Material);
OL_FORWARD_DECLARE(Path);

OL_FORWARD_DECLARE(CommandList);
OL_FORWARD_DECLARE(CommandQueue);

/** \class Loader
 * \author Filip Björklund
 * \date 06 december 2019 - 12:51
 * \brief
 * \details
 */
class Loader
{
private:
  /* Number of SRV used per material (One per texture) */
  static constexpr u32 SRV_PER_MAT = 4;

public:
  /* Loader results */
  enum class Result
  {
    kSuccess,
    kUnknownError,
  };

private:
  /* Model reference */
  struct ModelRef
  {
    Model* model;
  };

  /* Material reference */
  struct MatRef
  {
    Material* material;
    u32 idxStart;
  };

private:
  /** Srv heap **/
  DescriptorHeap* mSrvHeap = nullptr;

  /* Map of registered models */
  std::unordered_map<String, ModelRef> mModels;
  /* Map of registered materials */
  std::unordered_map<String, MatRef> mMaterials;

public:
  Loader();

  /** This functions triggers the loading of all the models and material from
   * disc. It also manages the uploading of data to the GPU for the resources
   * that requires it.
   * \param
   */
  void Load(CommandQueue* queue, CommandList* list);

  /**
   *
   */
  Result AddModel(const String& name, const Path& path);

  /**
   *
   */
  Result AddMaterial(const String& name,
                     const Path& pathAlbedo,
                     const Path& pathRoughness,
                     const Path& pathMetallic,
                     const Path& pathNormal);
};

}
