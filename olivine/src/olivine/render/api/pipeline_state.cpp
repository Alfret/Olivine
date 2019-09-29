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

#include "olivine/render/api/pipeline_state.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/app/app.hpp"
#include "olivine/core/file/file_io.hpp"
#include "olivine/core/file/file_system.hpp"
#include "olivine/render/api/device.hpp"
#include "olivine/render/api/root_signature.hpp"

// ========================================================================== //
// PipelineState Implementation
// ========================================================================== //

namespace olivine {

PipelineState::PipelineState(const CreateInfo& createInfo)
  : mKind(createInfo.kind)
{
  Assert(createInfo.rootSignature != nullptr,
         "Invalid pipeline state creation information. Root signature cannot "
         "be NULL");

  // Setup for grahics
  if (mKind == Kind::kGraphics) {
    SetupForGraphics(createInfo);
  } else if (mKind == Kind::kCompute) {
    SetupForCompute(createInfo);
  } else if (mKind == Kind::kRayTracing) {
    SetupForRayTracing(createInfo);
  }
}

// -------------------------------------------------------------------------- //

PipelineState::~PipelineState()
{
  // Release handle
  mHandle->Release();
  mHandle = nullptr;
}

// -------------------------------------------------------------------------- //

void
PipelineState::SetName(const String& name)
{
  D3D12Util::SetName(mHandle, name);
}

// -------------------------------------------------------------------------- //

void
PipelineState::SetupForGraphics(const CreateInfo& createInfo)
{
  Device* device = App::Instance()->GetDevice();

  // Setup desc
  D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
  desc.pRootSignature = createInfo.rootSignature->GetHandle();

  // Setup shader bytecode
  desc.VS.pShaderBytecode = createInfo.vs.bytes.data();
  desc.VS.BytecodeLength = createInfo.vs.bytes.size();
  desc.PS.pShaderBytecode = createInfo.ps.bytes.data();
  desc.PS.BytecodeLength = createInfo.ps.bytes.size();

  // Setup stream output state
  desc.StreamOutput.pSODeclaration = nullptr;
  desc.StreamOutput.NumEntries = 0;
  desc.StreamOutput.pBufferStrides = nullptr;
  desc.StreamOutput.NumStrides = 0;
  desc.StreamOutput.RasterizedStream = 0;

  // Setup blend state
  desc.BlendState.AlphaToCoverageEnable = false;
  desc.BlendState.IndependentBlendEnable = false;
  for (D3D12_RENDER_TARGET_BLEND_DESC& entry : desc.BlendState.RenderTarget) {
    entry.BlendEnable = false;
    entry.LogicOpEnable = false;
    entry.SrcBlend = D3D12_BLEND_ONE;
    entry.DestBlend = D3D12_BLEND_ZERO;
    entry.BlendOp = D3D12_BLEND_OP_ADD;
    entry.SrcBlendAlpha = D3D12_BLEND_ONE;
    entry.DestBlendAlpha = D3D12_BLEND_ZERO;
    entry.BlendOpAlpha = D3D12_BLEND_OP_ADD;
    entry.LogicOp = D3D12_LOGIC_OP_NOOP;
    entry.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
  }

  // Setup sample mask
  desc.SampleMask = 0xFFFFFFFF;

  // Setup rasterizer state
  desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
  desc.RasterizerState.CullMode = D3D12Util::ToCullMode(createInfo.cullMode);
  desc.RasterizerState.FrontCounterClockwise =
    createInfo.windingOrder == WindingOrder::kCounterClockwise;
  desc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
  desc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
  desc.RasterizerState.SlopeScaledDepthBias =
    D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
  desc.RasterizerState.DepthClipEnable = true;
  desc.RasterizerState.MultisampleEnable = false;
  desc.RasterizerState.AntialiasedLineEnable = false;
  desc.RasterizerState.ForcedSampleCount = 0;
  desc.RasterizerState.ConservativeRaster =
    D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

  // Setup depth-stencil state
  desc.DepthStencilState.DepthEnable = false;
  desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
  desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
  desc.DepthStencilState.StencilEnable = false;
  desc.DepthStencilState.StencilReadMask = 0;
  desc.DepthStencilState.StencilWriteMask = 0;
  desc.DepthStencilState.FrontFace = {};
  desc.DepthStencilState.BackFace = {};

  // Setup input layout
  u32 attributeCount = u32(createInfo.vertexAttributes.size());
  std::vector<D3D12_INPUT_ELEMENT_DESC> elements;
  elements.resize(attributeCount);
  for (u32 i = 0; i < attributeCount; i++) {
    D3D12_INPUT_ELEMENT_DESC& element = elements[i];
    const VertexAttribute& attribute = createInfo.vertexAttributes[i];
    element.SemanticName = attribute.semanticName.GetUTF8();
    element.SemanticIndex = attribute.semanticIndex;
    element.Format = FromAttributeKind(attribute.kind);
    element.InputSlot = attribute.assemblerSlot;
    element.AlignedByteOffset = attribute.offset;
    element.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
    element.InstanceDataStepRate = 0;
  }
  desc.InputLayout.NumElements = (UINT)attributeCount;
  desc.InputLayout.pInputElementDescs = elements.data();

  // Setup primitive topology
  desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

  // Setup attachment info
  u32 renderTargetCount = u32(createInfo.renderTargetFormats.size());
  Assert(renderTargetCount <= D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT,
         "Cannot have more than 8 render targets output to at the same time");
  desc.NumRenderTargets = renderTargetCount;
  for (u32 i = 0; i < renderTargetCount; i++) {
    desc.RTVFormats[i] =
      D3D12Util::ToDXGIFormat(createInfo.renderTargetFormats[i]);
  }
  desc.DSVFormat = DXGI_FORMAT_UNKNOWN;

  // Setup sample desc
  desc.SampleDesc.Count = 1;
  desc.SampleDesc.Quality = 0;

  // Setup rest
  desc.NodeMask = 0x00;
  desc.CachedPSO = { nullptr, 0 };
  desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

  // Create pipeline state
  const HRESULT hresult = device->GetHandle()->CreateGraphicsPipelineState(
    &desc, __uuidof(ID3D12PipelineState), reinterpret_cast<void**>(&mHandle));
  Assert(SUCCEEDED(hresult), "Failed to create pipeline state");
}

// -------------------------------------------------------------------------- //

void
PipelineState::SetupForCompute(const CreateInfo& createInfo)
{}

// -------------------------------------------------------------------------- //

void
PipelineState::SetupForRayTracing(const CreateInfo& createInfo)
{}

// -------------------------------------------------------------------------- //

PipelineState::ShaderBinary
PipelineState::LoadShader(const Path& path)
{
  ShaderBinary binary;

  // Resize code buffer
  const u64 fileSize = FileSystem::GetSize(path);
  binary.bytes.resize(fileSize);

  // Open file
  FileIO io(path);
  FileResult result = io.Open(FileIO::Flag::kRead);
  if (result != FileResult::kSuccess) {
    return ShaderBinary{ {} };
  }

  // Read file
  u64 read;
  result = io.Read(binary.bytes.data(), fileSize, read);
  if (result != FileResult::kSuccess) {
    return ShaderBinary{ {} };
  }

  // Success
  return binary;
}

// -------------------------------------------------------------------------- //

DXGI_FORMAT
PipelineState::FromAttributeKind(VertexAttributeKind kind)
{
  switch (kind) {
    case VertexAttributeKind::kByte: {
      return DXGI_FORMAT_R8_UNORM;
    }
    case VertexAttributeKind::kByte4: {
      return DXGI_FORMAT_R8G8B8A8_UNORM;
    }
    case VertexAttributeKind::kFloat: {
      return DXGI_FORMAT_R32_FLOAT;
    }
    case VertexAttributeKind::kFloat2: {
      return DXGI_FORMAT_R32G32_FLOAT;
    }
    case VertexAttributeKind::kFloat3: {
      return DXGI_FORMAT_R32G32B32_FLOAT;
    }
    case VertexAttributeKind::kFloat4: {
      return DXGI_FORMAT_R32G32B32A32_FLOAT;
    }
    default: {
      Panic("Invalid vertex attribute kind");
    }
  }
}

}
