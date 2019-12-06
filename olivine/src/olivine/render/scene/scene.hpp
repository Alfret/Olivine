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

#pragma once

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/core/macros.hpp"

// ========================================================================== //
// Scene Declaration
// ========================================================================== //

namespace olivine {

OL_FORWARD_DECLARE(Path);
OL_FORWARD_DECLARE(Model);
OL_FORWARD_DECLARE(Loader);

/** \class Scene
 * \author Filip Bj�rklund
 * \date 04 oktober 2019 - 11:19
 * \brief Scene.
 * \details
 * Represents a scene.
 */
class Scene
{
public:
  /* Results */
  enum class Result
  {
    kSuccess = 0,
    kUnknownError
  };

private:
  /* Resource loader */
  Loader* mLoader;

public:
  /** Construct an empty scene
   *
   */
  Scene();

  /** Load the data for the entire scene from a file at the specified path. If
   * there is anything else loaded into the scene before this call it will be
   * deleted.
   * \brief Load entire scene from file.
   * \param path Path to scene file.
   * \return Result.
   */
  Result Load(const Path& path);

  /**
   *
   */
  Loader* GetLoader() const { return mLoader; }
};

}