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

#include "olivine/render/scene/loader.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/render/scene/model.hpp"
#include "olivine/render/scene/material.hpp"

// ========================================================================== //
// Loader Implementation
// ========================================================================== //

namespace olivine {

Loader::Loader()
{
  mSrvHeap =
    new DescriptorHeap(Descriptor::Kind::kCbvSrvUav, SRV_PER_MAT * 100, false);
}

// -------------------------------------------------------------------------- //

void
Loader::Load(CommandQueue* queue, CommandList* list)
{}

// -------------------------------------------------------------------------- //

Loader::Result
Loader::AddModel(const String& name, const Path& path)
{
  // Create model
  Model* model = new Model;
  const Model::Error error = model->Load(this, path);
  if (error != Model::Error::kSuccess) {
    delete model;
    return Result::kUnknownError;
  }

  // Add model
  mModels[name] = ModelRef{ model };
  return Result::kSuccess;
}

// -------------------------------------------------------------------------- //

Loader::Result
Loader::AddMaterial(const String& name,
                    const Path& pathAlbedo,
                    const Path& pathRoughness,
                    const Path& pathMetallic,
                    const Path& pathNormal)
{
  // Create material
  Material* material =
    new Material(name, pathAlbedo, pathRoughness, pathMetallic, pathNormal);

  // Allocate descriptors
  const u32 idxAlbedo = mSrvHeap->Allocate();
  const u32 idxRoughness = mSrvHeap->Allocate();
  Assert(idxRoughness == idxAlbedo + 1,
         "Roughness SRV must be directly after albedo SRV");
  const u32 idxMetallic = mSrvHeap->Allocate();
  Assert(idxMetallic == idxRoughness + 1,
         "Metallic SRV must be directly after roughness SRV");
  const u32 idxNormal = mSrvHeap->Allocate();
  Assert(idxNormal == idxMetallic + 1,
         "Normal SRV must be directly after metallic SRV");

  // Add material
  mMaterials[name] = MatRef{ material, idxAlbedo };
  return Result::kUnknownError;
}

}
