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

#include "olivine/render/scene/scene.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/core/file/path.hpp"
#include "olivine/render/scene/loader.hpp"

// Thirdparty headers
#define TINYGLTF_NOEXCEPTION
#define JSON_NOEXCEPTION
#include "thirdparty/tinygltf/tiny_gltf.h"

// ========================================================================== //
// Scene Implementation
// ========================================================================== //

namespace olivine {

Scene::Scene()
{
  mLoader = new Loader;
}

// -------------------------------------------------------------------------- //

Scene::Result
Scene::Load(const Path& path)
{
  /*
  tinygltf::Model model;
  tinygltf::TinyGLTF loader;
  std::string err, warn;
  bool r =
    loader.LoadASCIIFromFile(&model, &err, &warn, path.GetPathStringUTF8(), 1);
  */
  return Result::kSuccess;
}

}
