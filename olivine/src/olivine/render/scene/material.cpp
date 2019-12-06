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

#include "olivine/render/scene/material.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/core/image.hpp"
#include "olivine/render/api/texture.hpp"
#include "olivine/render/api/upload.hpp"

// ========================================================================== //
// Material Implementation
// ========================================================================== //

namespace olivine {

Material::Material(const String& name,
                   const Path& pathAlbedo,
                   const Path& pathRoughness,
                   const Path& pathMetallic,
                   const Path& pathNormal)
  : mName(name)
  , mPathAlbedo(pathAlbedo)
  , mPathRoughness(pathRoughness)
  , mPathMetallic(pathMetallic)
  , mPathNormal(pathNormal)
{}

// -------------------------------------------------------------------------- //

Material::~Material()
{
  delete mTexAlbedo;
  delete mTexRoughness;
  delete mTexMetallic;
  delete mTexNormal;
}

// -------------------------------------------------------------------------- //

void
Material::Upload(CommandQueue* queue, CommandList* list)
{
  // Create albedo texture
  {
    Assert(!mPathAlbedo.GetPathString().IsEmpty(),
           "Albedo path cannot be empty");
    Image image;
    const Image::Result result = image.Load(mPathAlbedo);
    Assert(result == Image::Result::kSuccess,
           "Failed to load albedo image ({})",
           mPathAlbedo);

    Texture::CreateInfo texInfo;
    texInfo.width = image.GetWidth();
    texInfo.height = image.GetHeight();
    texInfo.dimension = Texture::Dim::k2D;
    texInfo.format = image.GetFormat() == Image::Format::kRGB
                       ? Format::kInvalid
                       : Format::kR8G8B8A8Unorm;
    texInfo.heapKind = HeapKind::kDefault;
    texInfo.usages = Texture::Usage::kShaderResource;
    mTexAlbedo = new Texture(texInfo);
    mTexAlbedo->SetName("mat_" + mName + "_albedo");

    UploadManager::Upload(queue, list, mTexAlbedo, &image);
  }

  // Create roughness texture
  if (!mPathRoughness.GetPathString().IsEmpty()) {
    Image image;
    const Image::Result result = image.Load(mPathRoughness);
    Assert(result == Image::Result::kSuccess,
           "Failed to load roughness image ({})",
           mPathRoughness);

    Texture::CreateInfo texInfo;
    texInfo.width = image.GetWidth();
    texInfo.height = image.GetHeight();
    texInfo.dimension = Texture::Dim::k2D;
    texInfo.format = image.GetFormat() == Image::Format::kRGB
                       ? Format::kInvalid
                       : Format::kR8G8B8A8Unorm;
    texInfo.heapKind = HeapKind::kDefault;
    texInfo.usages = Texture::Usage::kShaderResource;
    mTexRoughness = new Texture(texInfo);
    mTexRoughness->SetName("mat_" + mName + "_roughness");

    UploadManager::Upload(queue, list, mTexRoughness, &image);
  }

  // Create metallic texture
  if (!mPathMetallic.GetPathString().IsEmpty()) {
    Image image;
    const Image::Result result = image.Load(mPathMetallic);
    Assert(result == Image::Result::kSuccess,
           "Failed to load metallic image ({})",
           mPathMetallic);

    Texture::CreateInfo texInfo;
    texInfo.width = image.GetWidth();
    texInfo.height = image.GetHeight();
    texInfo.dimension = Texture::Dim::k2D;
    texInfo.format = image.GetFormat() == Image::Format::kRGB
                       ? Format::kInvalid
                       : Format::kR8G8B8A8Unorm;
    texInfo.heapKind = HeapKind::kDefault;
    texInfo.usages = Texture::Usage::kShaderResource;
    mTexMetallic = new Texture(texInfo);
    mTexMetallic->SetName("mat_" + mName + "_metallic");

    UploadManager::Upload(queue, list, mTexMetallic, &image);
  }

  // Create normal texture
  if (!mPathNormal.GetPathString().IsEmpty()) {
    Image image;
    const Image::Result result = image.Load(mPathNormal);
    Assert(result == Image::Result::kSuccess,
           "Failed to load normal image ({})",
           mPathNormal);

    Texture::CreateInfo texInfo;
    texInfo.width = image.GetWidth();
    texInfo.height = image.GetHeight();
    texInfo.dimension = Texture::Dim::k2D;
    texInfo.format = image.GetFormat() == Image::Format::kRGB
                       ? Format::kInvalid
                       : Format::kR8G8B8A8Unorm;
    texInfo.heapKind = HeapKind::kDefault;
    texInfo.usages = Texture::Usage::kShaderResource;
    mTexNormal = new Texture(texInfo);
    mTexNormal->SetName("mat_" + mName + "_normal");

    UploadManager::Upload(queue, list, mTexNormal, &image);
  }
}

}
