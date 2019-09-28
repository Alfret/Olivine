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

#include "olivine/render/api/root_signature.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/app/app.hpp"
#include "olivine/render/api/device.hpp"

// ========================================================================== //
// RootSignature Implementation
// ========================================================================== //

namespace olivine {

RootSignature::RootSignature(const CreateInfo& createInfo)
{
  Device* device = App::Instance()->GetDevice();

  // Setup root signature desc
  D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc;
  rootSignatureDesc.NumParameters = 0;
  rootSignatureDesc.pParameters = nullptr;
  rootSignatureDesc.NumStaticSamplers = 0;
  rootSignatureDesc.pStaticSamplers = nullptr;
  rootSignatureDesc.Flags =
    D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

  // Serialize root signature
  ID3DBlob* rootSignatureData = nullptr;
  ID3DBlob* rootSignatureError = nullptr;
  HRESULT hresult = D3D12SerializeRootSignature(&rootSignatureDesc,
                                                D3D_ROOT_SIGNATURE_VERSION_1,
                                                &rootSignatureData,
                                                &rootSignatureError);
  if (FAILED(hresult)) {
    if (rootSignatureError) {
      const char* errorMessage =
        static_cast<const char*>(rootSignatureError->GetBufferPointer());
      Panic("Root signature serialization error: %s\n", errorMessage);
    }
    Panic("Failed to serialize root signature");
  }
  Assert(rootSignatureData != nullptr,
         "Failed to serialize root signature. Invalid data");

  // Create root signature
  hresult = device->GetHandle()->CreateRootSignature(
    0x00,
    rootSignatureData->GetBufferPointer(),
    rootSignatureData->GetBufferSize(),
    __uuidof(ID3D12RootSignature),
    reinterpret_cast<void**>(&mHandle));
  Assert(SUCCEEDED(hresult), "Failed to create root signature");
}

// -------------------------------------------------------------------------- //

RootSignature::~RootSignature()
{
  // Release handle
  mHandle->Release();
  mHandle = nullptr;
}

// -------------------------------------------------------------------------- //

void
RootSignature::SetName(const String& name)
{
  D3D12Util::SetName(mHandle, name);
}

}
