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

#include "olivine/render/api/context.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/core/assert.hpp"

// ========================================================================== //
// Context Implementation
// ========================================================================== //

namespace olivine {

Context::Context(const CreateInfo& createInfo)
{
  UINT factoryFlags = 0;
  HRESULT hresult;

#if defined(_DEBUG)
  // Enable debug layers
  ID3D12Debug* debugInterface;
  hresult = D3D12GetDebugInterface(__uuidof(ID3D12Debug),
                                   reinterpret_cast<void**>(&debugInterface));
  if (SUCCEEDED(hresult)) {
    debugInterface->EnableDebugLayer();
    debugInterface->Release();
  }

  // Set message severity to break on
  IDXGIInfoQueue* infoQueue;
  hresult = DXGIGetDebugInterface1(
    0, __uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(&infoQueue));
  if (SUCCEEDED(hresult)) {
    // Set to break on error, corruption and warnings
    infoQueue->SetBreakOnSeverity(
      DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, TRUE);
    infoQueue->SetBreakOnSeverity(
      DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, TRUE);
    infoQueue->SetBreakOnSeverity(
      DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING, TRUE);

    infoQueue->Release();
  }
  factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

  // Create factory
  hresult = CreateDXGIFactory2(
    factoryFlags, __uuidof(IDXGIFactory4), reinterpret_cast<void**>(&mFactory));
  Assert(SUCCEEDED(hresult), "Failed to create DXGI Factory");
  D3D12Util::SetName(mFactory, "ContextFactory");
}

// -------------------------------------------------------------------------- //

Context::~Context()
{
  // Release factory
  mFactory->Release();
  mFactory = nullptr;

#if defined(_DEBUG)
  // Report live objects
  IDXGIDebug1* dxgiDebug;
  const HRESULT hresult = DXGIGetDebugInterface1(
    0, __uuidof(IDXGIDebug1), reinterpret_cast<void**>(&dxgiDebug));
  if (SUCCEEDED(hresult)) {
    dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
    dxgiDebug->Release();
  }
#endif
}

}
