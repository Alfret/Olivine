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
  // Assert preconditions
  Assert(createInfo.parameters.size() <= kMaxRootParameters,
         "Maximum number of root parameters exceeded");

  Device* device = App::Instance()->GetDevice();

  // Temporary descriptor ranges
  D3D12_DESCRIPTOR_RANGE totalRanges[kMaxRootParameters]
                                    [kMaxRootDescriptorTableRanges];

  // Setup parameters
  ShaderStage stages = ShaderStage::kNone;
  std::vector<D3D12_ROOT_PARAMETER> rootParams;
  rootParams.resize(createInfo.parameters.size());
  for (u32 i = 0; i < u32(createInfo.parameters.size()); i++) {
    D3D12_ROOT_PARAMETER& rootParam = rootParams[i];
    const RootParameter& param = createInfo.parameters[i];
    rootParam.ShaderVisibility = ToShaderVisibility(param.stages);

    // Setup depending on type
    switch (param.data.index()) {
      case 0: {
        const RootTable& table = std::get<RootTable>(param.data);
        Assert(table.ranges.size() <= kMaxRootDescriptorTableRanges,
               "Maximum number of root table descriptor ranges exceeded");

        rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParam.DescriptorTable.NumDescriptorRanges = table.ranges.size();
        rootParam.DescriptorTable.pDescriptorRanges = totalRanges[i];

        // Build ranges
        for (u32 j = 0; j < table.ranges.size(); j++) {
          D3D12_DESCRIPTOR_RANGE& rootRange = totalRanges[i][j];
          const RootTableRange& range = table.ranges[i];
          rootRange.RangeType = ToDescriptorRangeType(range.kind);
          rootRange.NumDescriptors = range.count;
          rootRange.BaseShaderRegister = range.baseReg;
          rootRange.RegisterSpace = range.space;
          rootRange.OffsetInDescriptorsFromTableStart = range.offsetInTable;
        }
        break;
      }
      case 1: {
        const RootConstant& constant = std::get<RootConstant>(param.data);
        rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
        rootParam.Constants.ShaderRegister = constant.reg;
        rootParam.Constants.RegisterSpace = constant.space;
        rootParam.Constants.Num32BitValues = constant.numValues;
        break;
      }
      case 2: {
        const RootDescriptor& descriptor = std::get<RootDescriptor>(param.data);
        rootParam.ParameterType = ToRootParameterType(descriptor.kind);
        rootParam.Descriptor.ShaderRegister = descriptor.reg;
        rootParam.Descriptor.RegisterSpace = descriptor.space;
        break;
      }
      default: {
        Panic("Invalid root parameter type");
      }
    }
  }

  // Setup static samplers
  D3D12_STATIC_SAMPLER_DESC totalStaticSamplers[kMaxStaticSamplers];
  for (u32 i = 0; i < createInfo.staticSamplers.size(); i++) {
    D3D12_STATIC_SAMPLER_DESC& staticSamplerDesc = totalStaticSamplers[i];
    const StaticSampler& staticSampler = createInfo.staticSamplers[i];

    staticSamplerDesc.Filter =
      Sampler::ToFilter(staticSampler.minFilter,
                        staticSampler.magFilter,
                        staticSampler.mipFilter,
                        staticSampler.anisotropicFiltering,
                        staticSampler.reductionKind);
    staticSamplerDesc.AddressU =
      Sampler::ToAddressMode(staticSampler.addressModeU);
    staticSamplerDesc.AddressV =
      Sampler::ToAddressMode(staticSampler.addressModeV);
    staticSamplerDesc.AddressW =
      Sampler::ToAddressMode(staticSampler.addressModeW);
    staticSamplerDesc.MipLODBias = staticSampler.mipLODBias;
    staticSamplerDesc.MaxAnisotropy = staticSampler.maxAnisotropy;
    staticSamplerDesc.ComparisonFunc =
      D3D12Util::ToComparisonFunc(staticSampler.comparisonFunction);
    staticSamplerDesc.BorderColor =
      Sampler::ToBorderColor(staticSampler.borderColor);
    staticSamplerDesc.MinLOD = staticSampler.minLOD;
    staticSamplerDesc.MaxLOD = staticSampler.maxLOD;
    staticSamplerDesc.ShaderRegister = staticSampler.reg;
    staticSamplerDesc.RegisterSpace = staticSampler.space;
    staticSamplerDesc.ShaderVisibility =
      ToShaderVisibility(staticSampler.accessibleStages);
  }

  // Setup root signature desc
  D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc;
  rootSignatureDesc.NumParameters = rootParams.size();
  rootSignatureDesc.pParameters = rootParams.data();
  rootSignatureDesc.NumStaticSamplers = createInfo.staticSamplers.size();
  rootSignatureDesc.pStaticSamplers = totalStaticSamplers;
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
      Panic("Root signature serialization error: {}\n", errorMessage);
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

// -------------------------------------------------------------------------- //

D3D12_SHADER_VISIBILITY
RootSignature::ToShaderVisibility(ShaderStage stages)
{
  D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL;
  if (stages == ShaderStage::kVertex) {
    visibility = D3D12_SHADER_VISIBILITY_VERTEX;
  } else if (stages == ShaderStage::kPixel) {
    visibility = D3D12_SHADER_VISIBILITY_PIXEL;
  }
  return visibility;
}

// -------------------------------------------------------------------------- //

D3D12_ROOT_PARAMETER_TYPE
RootSignature::ToRootParameterType(RootDescriptorKind kind)
{
  switch (kind) {
    case RootDescriptorKind::kSrv: {
      return D3D12_ROOT_PARAMETER_TYPE_SRV;
    }
    case RootDescriptorKind::kUav: {
      return D3D12_ROOT_PARAMETER_TYPE_UAV;
    }
    case RootDescriptorKind::kCbv: {
      return D3D12_ROOT_PARAMETER_TYPE_CBV;
    }
    case RootDescriptorKind::kSampler:
    default: {
      Panic("Invalid root descriptor kind");
    }
  }
}

// -------------------------------------------------------------------------- //

D3D12_DESCRIPTOR_RANGE_TYPE
RootSignature::ToDescriptorRangeType(RootDescriptorKind kind)
{
  switch (kind) {
    case RootDescriptorKind::kSrv: {
      return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    }
    case RootDescriptorKind::kUav: {
      return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
    }
    case RootDescriptorKind::kCbv: {
      return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    }
    case RootDescriptorKind::kSampler: {
      return D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
    }
    default: {
      Panic("Invalid root descriptor range kind");
    }
  }
}

}
