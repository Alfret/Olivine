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

#include "olivine/render/api/swap_chain.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/app/app.hpp"
#include "olivine/core/assert.hpp"
#include "olivine/render/api/context.hpp"
#include "olivine/render/api/device.hpp"
#include "olivine/render/api/queue.hpp"
#include "olivine/render/api/descriptor.hpp"
#include "olivine/render/api/texture.hpp"

// ========================================================================== //
// SwapChain Implementation
// ========================================================================== //

namespace olivine {

SwapChain::SwapChain(const CreateInfo& createInfo)
  : mPresentQueue(createInfo.presentQueue)
  , mFormatDXGI(D3D12Util::ToDXGIFormat(createInfo.format))
  , mFormat(createInfo.format)
  , mWidth(createInfo.bufferWidth)
  , mHeight(createInfo.bufferHeight)
{
  mFlags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

  Context* context = App::Instance()->GetContext();
  Device* device = App::Instance()->GetDevice();

  // Check present support
  D3D12_FEATURE_DATA_FORMAT_SUPPORT supportData;
  supportData.Format = mFormatDXGI;
  device->GetHandle()->CheckFeatureSupport(
    D3D12_FEATURE_FORMAT_SUPPORT,
    &supportData,
    sizeof(D3D12_FEATURE_DATA_FORMAT_SUPPORT));
  Assert(supportData.Support1 & D3D12_FORMAT_SUPPORT1_RENDER_TARGET,
         "Swap chain cannot be created with the specified format. It does not "
         "support presenting");

  // Setup desc
  DXGI_SWAP_CHAIN_DESC1 desc;
  desc.Width = mWidth;
  desc.Height = mHeight;
  desc.Format = mFormatDXGI;
  desc.Stereo = FALSE;
  desc.SampleDesc.Count = 1;
  desc.SampleDesc.Quality = 0;
  desc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
  desc.BufferCount = kBufferCount;
  desc.Scaling = DXGI_SCALING_NONE;
  desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
  desc.Flags = mFlags;

  // Create swap chain
  const HRESULT hresult = context->GetFactory()->CreateSwapChainForHwnd(
    mPresentQueue->GetHandle(),
    createInfo.window,
    &desc,
    nullptr,
    nullptr,
    reinterpret_cast<IDXGISwapChain1**>(&mHandle));
  Assert(SUCCEEDED(hresult), "Failed to create swap chain");

  // Retrieve frame index
  mIndex = mHandle->GetCurrentBackBufferIndex();

  // Create descriptor heap
  mHeapRTV = new DescriptorHeap(Descriptor::Kind::kRtv, kBufferCount);

  // Finalize
  SetupBuffers();
}

// -------------------------------------------------------------------------- //

SwapChain::~SwapChain()
{
  // Delete buffers
  for (auto* buffer : mBuffers) {
    delete buffer;
  }

  // Delete RTV heap
  delete mHeapRTV;

  // Release handle
  mHandle->Release();
  mHandle = nullptr;
}

// -------------------------------------------------------------------------- //

void
SwapChain::Present()
{
  // Present
  const HRESULT hresult = mHandle->Present(mVerticalSync ? 1 : 0, 0);
  Assert(SUCCEEDED(hresult), "Failed to present swap chain");

  // Retrieve next index
  mIndex = mHandle->GetCurrentBackBufferIndex();
}

// -------------------------------------------------------------------------- //

void
SwapChain::Resize(u32 width, u32 height)
{
  // Flush present queue
  mPresentQueue->Flush();

  // Delete old buffers
  for (Texture* buffer : mBuffers) {
    delete buffer;
  }

  // Resize buffers
  mWidth = width;
  mHeight = height;
  const HRESULT hresult =
    mHandle->ResizeBuffers(kBufferCount, width, height, mFormatDXGI, mFlags);
  Assert(SUCCEEDED(hresult), "Failed to resize swap chain buffers");

  // Setup images and index
  SetupBuffers();
  mIndex = mHandle->GetCurrentBackBufferIndex();
}

// -------------------------------------------------------------------------- //

void
SwapChain::SetName(const String& name)
{
  mName = name;

  // Set swap chain handle name
  D3D12Util::SetName(mHandle, mName);

  // Set descriptor heap name
  mHeapRTV->SetName(mName + "HeapRTV");

  // Set buffer name
  for (u32 i = 0; i < kBufferCount; i++) {
    mBuffers[i]->SetName(String::Format("{}Buf[{}]", mName, std::to_string(i)));
  }
}

// -------------------------------------------------------------------------- //

void
SwapChain::SetupBuffers()
{
  // Create textures and views
  for (u32 i = 0; i < kBufferCount; i++) {
    // Retrieve resource (image/buffer)
    ID3D12Resource* resource;
    const HRESULT hresult = mHandle->GetBuffer(
      i, __uuidof(ID3D12Resource), reinterpret_cast<void**>(&resource));
    Assert(SUCCEEDED(hresult), "Failed to retrieve swap chain buffer resource");
    mBuffers[i] = new Texture(resource,
                              mWidth,
                              mHeight,
                              1,
                              Texture::Dim::k2D,
                              mFormat,
                              Texture::Usage::kColorAttachment);
    resource->Release();

    // Set name if not empty
    if (!mName.IsEmpty()) {
      mBuffers[i]->SetName(
        String::Format("{}Buf[{}]", mName, std::to_string(i)));
    }

    // Create view
    mHeapRTV->WriteDescriptorRTV(i, mBuffers[i]);
  }
}

}
