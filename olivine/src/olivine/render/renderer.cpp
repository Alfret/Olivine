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

#include "olivine/render/renderer.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/app/app.hpp"
#include "olivine/core/file/path.hpp"
#include "olivine/render/color.hpp"
#include "olivine/render/camera.hpp"
#include "olivine/render/scene/scene.hpp"
#include "olivine/render/scene/entity.hpp"
#include "olivine/render/api/constant_buffer.hpp"
#include "olivine/render/api/root_signature.hpp"
#include "olivine/render/api/pipeline_state.hpp"
#include "olivine/render/api/command_list.hpp"
#include "olivine/render/scene/model.hpp"
#include "olivine/render/scene/loader.hpp"

// ========================================================================== //
// Renderer Implementation
// ========================================================================== //

namespace olivine {

Renderer::Renderer()
{
  // Setup frame resources
  u32 idx = 0;
  for (Frame& frame : mFrames) {
    frame.modelCB =
      new ConstantBuffer(sizeof(Matrix4F) * MAX_ENTITY, HeapKind::kUpload);
    frame.modelCB->SetName(String::Format("renderer_model_cb_{}", idx));
    frame.cameraCB = new ConstantBuffer(sizeof(CameraData), HeapKind::kUpload);
    frame.cameraCB->SetName(String::Format("renderer_camera_cb_{}", idx));
    frame.lightCB = new ConstantBuffer(sizeof(LightData), HeapKind::kUpload);
    frame.lightCB->SetName(String::Format("renderer_light_cb_{}", idx));
    idx++;
  }

  // Setup descriptor heap
  mDescriptorHeap =
    new DescriptorHeap(Descriptor::Kind::kCbvSrvUav, 1024, true);

  // Setup RS/PSO
  SetupPSO();
}

// -------------------------------------------------------------------------- //

Renderer::~Renderer()
{
  delete mPipelineState;
  delete mRootSignature;
  delete mDescriptorHeap;
  for (Frame& frame : mFrames) {
    delete frame.lightCB;
    delete frame.cameraCB;
    delete frame.modelCB;
  }
}

// -------------------------------------------------------------------------- //

void
Renderer::Render(CommandList* list, const Camera* camera, const Scene* scene)
{
  SwapChain* swapChain = App::Instance()->GetSwapChain();
  // Device* device = App::Instance()->GetDevice();
  Loader* loader = scene->GetLoader();

  // Retrieve frame resources
  const u32 index = swapChain->Index();
  Frame& frame = mFrames[index];

  // Copy material descriptor
  mDescriptorHeap->CopyFrom(loader->GetSrvHeap(),
                            Loader::MAX_MAT * Loader::SRV_PER_MAT);

  // Setup camera information
  CameraData cameraData;
  cameraData.proj = camera->GetProjection();
  cameraData.view = camera->GetView();
  frame.cameraCB->Write(cameraData, 1);

  // Setup light information
  LightData lightData;
  Memory::Clear(&lightData, sizeof(LightData));
  lightData.pos = Vector3F(2.0f, 2.0f, 0.0f);
  const Color col = Color::kWhite;
  lightData.color = Vector4F{
    col.GetRedF32(), col.GetGreenF32(), col.GetBlueF32(), col.GetAlphaF32()
  };
  frame.lightCB->Write(lightData, 1);

  // Record common commands
  list->SetPrimitiveTopology(PrimitiveTopology::kTriangleList);
  list->SetRootSignatureGraphics(mRootSignature);
  list->SetPipelineState(mPipelineState);
  list->SetDescriptorHeap(mDescriptorHeap);
  list->SetRootDescriptorGraphics(2, frame.lightCB);
  list->SetRootDescriptorGraphics(3, frame.cameraCB);

  // Render each entity
  u32 idx = 0;
  for (auto entity : scene->GetEntities()) {
    const Model* model = entity->GetModel();
    u32 matIdx = loader->GetMaterialSrvHeapOffset(model->GetMaterial());

    const Matrix4F& transform = entity->GetTransform();
    frame.modelCB->Write(transform, idx);

    list->SetVertexBuffer(model->GetVertexBuffer());
    list->SetRootDescriptorGraphics(1, frame.modelCB);
    list->SetRootDescriptorTableGraphics(0, mDescriptorHeap->At(4 * matIdx));
    list->Draw(model->GetVertexCount());

    // Offset CB
    idx++;
  }
}

// -------------------------------------------------------------------------- //

void
Renderer::SetupPSO()
{
  // [TABLE] SRVs for materials
  RootSignature::RootTableRange rootTableRange0 = RootSignature::RootTableRange{
    RootSignature::RootDescriptorKind::kSrv, 4, 0, 0
  };
  RootSignature::RootTable rootTable;
  rootTable.ranges = { rootTableRange0 };
  RootSignature::RootParameter rootParam0 =
    RootSignature::RootParameter(rootTable, ShaderStage::kPixel);

  // [DESC] CBV for transform
  RootSignature::RootDescriptor rootDescriptor0 = {
    0, 0, RootSignature::RootDescriptorKind::kCbv
  };
  RootSignature::RootParameter rootParam1 = RootSignature::RootParameter(
    rootDescriptor0, ShaderStage::kVertex | ShaderStage::kPixel);

  // [DESC] CBV for light data
  RootSignature::RootDescriptor rootDescriptor1 = {
    1, 0, RootSignature::RootDescriptorKind::kCbv
  };
  RootSignature::RootParameter rootParam2 = RootSignature::RootParameter(
    rootDescriptor1, ShaderStage::kVertex | ShaderStage::kPixel);

  // [DESC] CBV for camera data
  RootSignature::RootDescriptor rootDescriptor2 = {
    2, 0, RootSignature::RootDescriptorKind::kCbv
  };
  RootSignature::RootParameter rootParam3 = RootSignature::RootParameter(
    rootDescriptor2, ShaderStage::kVertex | ShaderStage::kPixel);

  // Create root signture
  RootSignature::CreateInfo rootSignatureInfo;
  rootSignatureInfo.parameters.push_back(rootParam0);
  rootSignatureInfo.parameters.push_back(rootParam1);
  rootSignatureInfo.parameters.push_back(rootParam2);
  rootSignatureInfo.parameters.push_back(rootParam3);
  RootSignature::StaticSampler staticSampler0;
  staticSampler0.reg = 0;
  staticSampler0.accessibleStages = ShaderStage::kPixel;
  staticSampler0.magFilter = Sampler::Filter::kLinear;
  rootSignatureInfo.staticSamplers.push_back(staticSampler0);
  mRootSignature = new RootSignature(rootSignatureInfo);

  // Create pipeline state
  Format swapChainFormat = App::Instance()->GetSwapChain()->GetFormat();
  PipelineState::CreateInfo pipelineStateInfo{};
  pipelineStateInfo.kind = PipelineState::Kind::kGraphics;
  pipelineStateInfo.rootSignature = mRootSignature;
  pipelineStateInfo.renderTargetFormats.push_back(swapChainFormat);
  pipelineStateInfo.vs =
    PipelineState::LoadShader(Path{ "res/forward_vs.cso" });
  pipelineStateInfo.ps =
    PipelineState::LoadShader(Path{ "res/forward_ps.cso" });
  pipelineStateInfo.vertexAttributes = {
    PipelineState::VertexAttribute{
      "POSITION", 0, PipelineState::VertexAttributeKind::kFloat3 },
    PipelineState::VertexAttribute{ "NORMAL",
                                    0,
                                    PipelineState::VertexAttributeKind::kFloat3,
                                    offsetof(Model::Vertex, normals) },
    PipelineState::VertexAttribute{ "TEXCOORD",
                                    0,
                                    PipelineState::VertexAttributeKind::kFloat2,
                                    offsetof(Model::Vertex, uv) }
  };
  pipelineStateInfo.cullMode = CullMode::kBack;
  mPipelineState = new PipelineState(pipelineStateInfo);
  mPipelineState->SetName("MainPipelineState");
}

}
