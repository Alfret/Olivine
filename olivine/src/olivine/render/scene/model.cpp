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

#include "olivine/render/scene/model.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/core/assert.hpp"
#include "olivine/core/file/path.hpp"
#include "olivine/core/file/file.hpp"
#include "olivine/core/console.hpp"
#include "olivine/core/image.hpp"
#include "olivine/render/api/vertex_buffer.hpp"
#include "olivine/render/api/upload.hpp"
#include "olivine/render/api/texture.hpp"

// Thirdparty headers
#define TINYOBJLOADER_IMPLEMENTATION
#include "thirdparty/tinyobjloader/tiny_obj_loader.h"

// ==========================================================================
// // Model Implementation
// ==========================================================================
// //

namespace olivine {

Model::Model(const Path& path)
{
  // Material reader
  class MatReader : public tinyobj::MaterialReader
  {
  private:
    Path mDirectory;

  public:
    explicit MatReader(const Path& directory)
      : mDirectory(directory)
    {}

    /* Called to load material */
    bool operator()(const std::string& matId,
                    std::vector<tinyobj::material_t>* materials,
                    std::map<std::string, int>* matMap,
                    std::string* warn,
                    std::string* err) override
    {
      const Path path = mDirectory + matId.c_str();
      std::ifstream handle(path.GetPathStringUTF8(), std::ios::in);
      std::string warning, error;
      tinyobj::LoadMtl(matMap, materials, &handle, &warning, &error);
      return true;
    }
  };

  // Open stream
  std::ifstream handle(path.GetPathStringUTF8(), std::ios::in);

  // Read file
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warning, error;
  MatReader materialReader(path.GetDirectory());

  bool ret = tinyobj::LoadObj(&attrib,
                              &shapes,
                              &materials,
                              &warning,
                              &error,
                              &handle,
                              &materialReader,
                              true,
                              true);

  if (!warning.empty()) {
    Console::WriteLine("Warning while loading model ({}): {}",
                       path.GetPathStringUTF8(),
                       warning);
  }
  if (!error.empty()) {
    Console::WriteLine(
      "Error while loading model ({}): {}", path.GetPathStringUTF8(), warning);
  }
  if (!ret) {
    Console::WriteLine("Failed to load model {}", path.GetPathStringUTF8());
  }

  Assert(shapes.size() > 0, "Model does not contains any meshes");

  // Set name
  mName = String::Format("Model({})", shapes[0].name);

  // Build vertex data
  tinyobj::mesh_t& mesh = shapes[0].mesh;
  mVertexCount = mesh.indices.size();
  mVertices = new Vertex[mVertexCount];
  for (u32 i = 0; i < mVertexCount; i++) {
    tinyobj::index_t& index = mesh.indices[i];
    mVertices[i].pos =
      Vector3F{ attrib.vertices[(index.vertex_index * 3)],
                attrib.vertices[(index.vertex_index * 3) + 1],
                attrib.vertices[(index.vertex_index * 3) + 2] };
    mVertices[i].normals =
      Vector3F{ attrib.normals[(index.normal_index * 3)],
                attrib.normals[(index.normal_index * 3) + 1],
                attrib.normals[(index.normal_index * 3) + 2] };
    mVertices[i].uv =
      Vector2F{ attrib.texcoords[(index.texcoord_index * 2)],
                attrib.texcoords[(index.texcoord_index * 2) + 1] };
  }

  // Create buffers
  mVertexBuffer =
    new VertexBuffer(sizeof(Vertex) * mVertexCount, sizeof(Vertex));
  mVertexBuffer->SetName(mName + "VB");

  // Load material
  Assert(materials.size() > 0, "Mesh does not have any material");
  tinyobj::material_t& material = materials[0];

  mMaterial.mAlbedoPath =
    (path.GetDirectory() + material.diffuse_texname.c_str())
      .GetPathStringUTF8();
}

// --------------------------------------------------------------------------
// //

Model::~Model()
{
  // Delete vertex and index data
  delete mVertices;
  mVertices = nullptr;

  // Delete vertex buffer
  delete mVertexBuffer;

  // Delete material
  delete mMaterial.mAlbedo;
}

// --------------------------------------------------------------------------
// //

void
Model::Upload(CommandQueue* queue, CommandList* list)
{
  // Upload vertices to vertex buffer in default heap
  Buffer* buffer = &mVertexBuffer->GetBuffer();
  UploadManager::Upload(queue,
                        list,
                        buffer,
                        reinterpret_cast<u8*>(mVertices),
                        mVertexCount * sizeof(Vertex));

  // Create and upload texture
  Image image;
  const Image::Result result = image.Load(mMaterial.mAlbedoPath);
  Assert(result == Image::Result::kSuccess,
         "Failed to load image ({})",
         mMaterial.mAlbedoPath);

  Texture::CreateInfo texInfo;
  texInfo.width = image.GetWidth();
  texInfo.height = image.GetHeight();
  texInfo.dimension = Texture::Dim::k2D;
  texInfo.format = image.GetFormat() == Image::Format::kRGB
                     ? Format::kInvalid
                     : Format::kR8G8B8A8Unorm;
  texInfo.heapKind = HeapKind::kDefault;
  texInfo.usages = Texture::Usage::kShaderResource;
  mMaterial.mAlbedo = new Texture(texInfo);
  mMaterial.mAlbedo->SetName(mName + "MaterialAlbedoTex");
  UploadManager::Upload(queue, list, mMaterial.mAlbedo, &image);
}
}
