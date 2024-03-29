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

// Project heeaders
#include "olivine/core/macros.hpp"
#include "olivine/core/string.hpp"
#include "olivine/math/vector2f.hpp"
#include "olivine/math/vector3f.hpp"

// ========================================================================== //
// Model Declaration
// ========================================================================== //

namespace olivine {

OL_FORWARD_DECLARE(VertexBuffer);
OL_FORWARD_DECLARE(IndexBuffer);
OL_FORWARD_DECLARE(Texture);
OL_FORWARD_DECLARE(Path);
OL_FORWARD_DECLARE(CommandList);
OL_FORWARD_DECLARE(CommandQueue);

OL_FORWARD_DECLARE(Loader);

/** \class Model
 * \author Filip Bj�rklund
 * \date 29 september 2019 - 18:55
 * \brief
 * \details
 */
class Model
{
  OL_NO_COPY(Model);

public:
  /* Error enumeration */
  enum class Error
  {
    kSuccess,
    kInvalidFileType,
    kFileNotFound,
    kMissingMaterial
  };

  /* Vertex structure */
  struct Vertex
  {
    /* Position */
    Vector3F pos;
    /* Normals */
    Vector3F normals;
    /* Texture coordinates */
    Vector2F uv;
  };

private:
  /* Name of model */
  String mName;

  /* Name of material for texture. Or empty for no material */
  String mMaterialName;

  /* Vertex data */
  Vertex* mVertices = nullptr;
  /* Number of vertices */
  u32 mVertexCount;

  /* Vertex buffer */
  VertexBuffer* mVertexBuffer = nullptr;

public:
  /**
   *
   */
  Model() = default;

  /**
   *
   */
  ~Model();

  Error Load(Loader* loader, const Path& path);

  /**
   *
   */
  void Upload(CommandQueue* queue, CommandList* list);

  /**
   *
   */
  u32 GetVertexCount() const { return mVertexCount; }

  /**
   *
   */
  VertexBuffer* GetVertexBuffer() const { return mVertexBuffer; }

  /**
   *
   */
  const String& GetMaterial() const { return mMaterialName; }

  /**
   *
   */
  void SetMaterial(const String& name) { mMaterialName = name; }

private:
  Error LoadObj(Loader* loader, const Path& path);

  Error LoadGltf(Loader* loader, const Path& path);
};

}