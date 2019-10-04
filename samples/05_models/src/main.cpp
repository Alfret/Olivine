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

#include <olivine/app/app.hpp>
#include <olivine/core/console.hpp>
#include <olivine/core/file/path.hpp>
#include <olivine/core/image.hpp>
#include <olivine/core/time.hpp>
#include <olivine/math/literals.hpp>
#include <olivine/math/vector3f.hpp>
#include <olivine/math/vector4f.hpp>
#include <olivine/math/matrix4f.hpp>
#include <olivine/render/color.hpp>
#include <olivine/render/api/device.hpp>
#include <olivine/render/api/swap_chain.hpp>
#include <olivine/render/api/command_list.hpp>
#include <olivine/render/api/common.hpp>
#include <olivine/render/api/root_signature.hpp>
#include <olivine/render/api/pipeline_state.hpp>
#include <olivine/render/api/vertex_buffer.hpp>
#include <olivine/render/api/index_buffer.hpp>
#include <olivine/render/api/texture.hpp>
#include <olivine/render/api/constant_buffer.hpp>
#include <olivine/render/scene/model.hpp>
#include "olivine/render/api/descriptor_allocator.hpp"

// ========================================================================== //
// Sample
// ========================================================================== //

using namespace olivine;

/** Sample app **/
class Sample final : public App
{
public:
  /* Structure with all per-frame resources */
  struct Frame
  {
    /* Command list */
    CommandList* list = nullptr;
    /* Frame sync semaphore */
    Semaphore* sem = nullptr;
    /* Next value to wait for with semaphore */
    u32 semVal = 0;

    /* Constant buffer for transforms */
    ConstantBuffer* constBuf;
  };

  /** Vertex structure **/
  struct Vertex
  {
    /* Position */
    Vector3F pos;
    /* Color */
    Color color;
  };

private:
  /* Frame resources */
  Frame mFrames[SwapChain::kBufferCount];
  /* Clear color */
  Color mClearColor = Color::FromHex(0x8ba0c1ff);

  /* Command list for uploading */
  CommandList* mUploadList;

  /* Root signature */
  RootSignature* mRootSignature = nullptr;
  /* Pipeline state */
  PipelineState* mPipelineState = nullptr;

  /* Model */
  Model* mModel = nullptr;
  /* SRV heap */
  DescriptorHeap* mHeapSRV = nullptr;

public:
  /** Construct **/
  explicit Sample(const CreateInfo& createInfo)
    : App(createInfo)
  {
    /* Init frame resources */
    for (Frame& frame : mFrames) {
      frame.list = new CommandList(CommandQueue::Kind::kGraphics);
      frame.sem = new Semaphore;
      frame.semVal = 0;
      frame.constBuf = new ConstantBuffer(sizeof(Matrix4F), HeapKind::kUpload);
    }

    // Create root signature
    RootSignature::CreateInfo rootSignatureInfo;
    RootSignature::RootTableRange rootTableRange0 =
      RootSignature::RootTableRange{
        RootSignature::RootDescriptorKind::kSrv, 1, 0, 0
      };
    RootSignature::RootTable rootTable;
    rootTable.ranges = { rootTableRange0 };
    RootSignature::RootParameter rootParam0 =
      RootSignature::RootParameter(rootTable, ShaderStage::kPixel);
    RootSignature::RootDescriptor rootDescriptor0 = {
      0, 0, RootSignature::RootDescriptorKind::kCbv
    };
    RootSignature::RootParameter rootParam1 = RootSignature::RootParameter(
      rootDescriptor0, ShaderStage::kVertex | ShaderStage::kPixel);
    rootSignatureInfo.parameters.push_back(rootParam0);
    rootSignatureInfo.parameters.push_back(rootParam1);
    RootSignature::StaticSampler staticSampler0;
    staticSampler0.reg = 0;
    staticSampler0.accessibleStages = ShaderStage::kPixel;
    staticSampler0.magFilter = Sampler::Filter::kLinear;
    rootSignatureInfo.staticSamplers.push_back(staticSampler0);
    mRootSignature = new RootSignature(rootSignatureInfo);

    // Create pipeline state
    PipelineState::CreateInfo pipelineStateInfo{};
    pipelineStateInfo.kind = PipelineState::Kind::kGraphics;
    pipelineStateInfo.rootSignature = mRootSignature;
    pipelineStateInfo.renderTargetFormats.push_back(
      GetSwapChain()->GetFormat());
    pipelineStateInfo.vs =
      PipelineState::LoadShader(Path{ "res/forward_vs.cso" });
    pipelineStateInfo.ps =
      PipelineState::LoadShader(Path{ "res/forward_ps.cso" });
    pipelineStateInfo.vertexAttributes = {
      PipelineState::VertexAttribute{
        "POSITION", 0, PipelineState::VertexAttributeKind::kFloat3 },
      PipelineState::VertexAttribute{
        "NORMAL",
        0,
        PipelineState::VertexAttributeKind::kFloat3,
        offsetof(Model::Vertex, normals) },
      PipelineState::VertexAttribute{
        "TEXCOORD",
        0,
        PipelineState::VertexAttributeKind::kFloat2,
        offsetof(Model::Vertex, uv) }
    };
    pipelineStateInfo.cullMode = CullMode::kBack;
    mPipelineState = new PipelineState(pipelineStateInfo);
    mPipelineState->SetName("MainPipelineState");

    // Create upload command list
    mUploadList = new CommandList(CommandQueue::Kind::kCopy);

    // Load model
    mModel = new Model(Path{ "res/cube.obj" });
    mModel->Upload(GetCopyQueue(), mUploadList);

    // Create SRV heap
    mHeapSRV = new DescriptorHeap(Descriptor::Kind::kCbvSrvUav, 1, true);
    mHeapSRV->WriteDescriptorSRV(0, mModel->GetMaterial().mAlbedo);
    mHeapSRV->SetName("MainShaderResourceHeap");
  }

  /** Cleanup **/
  ~Sample()
  {
    FlushQueues();
    for (Frame& frame : mFrames) {
      delete frame.list;
      delete frame.sem;
      delete frame.constBuf;
    }
    delete mHeapSRV;
    delete mModel;
    delete mPipelineState;
    delete mRootSignature;
    delete mUploadList;
  }

  /** Render **/
  void Render() override
  {
    // Retrieve and update frame resources
    const u32 index = GetSwapChain()->Index();
    Frame& frame = mFrames[index];
    frame.sem->Wait(frame.semVal);
    Texture* buffer = GetSwapChain()->CurrentBuffer();
    const Descriptor rt = GetSwapChain()->CurrentRT();

    // Update constant buffer
    const f32 rotX = f32(Time::Now().GetSeconds() / 2.0f);
    const f32 rotY = f32(Time::Now().GetSeconds());
    // const f32 rotX = GetGamepadAxis(GamepadAxis::kLeftY);
    // const f32 rotY = GetGamepadAxis(GamepadAxis::kLeftX);
    const Vector4F modelPos{ 0.0f, 0.0f, 2.8f };
    const Matrix4F m =
      Matrix4F::Perspective(f32(45._Deg), 16.0f / 9.0f, 0.1f, 1000.0f) *
      Matrix4F::Translation(modelPos) * Matrix4F::RotationY(rotY) *
      Matrix4F::RotationX(rotX);

    frame.constBuf->Write(m);

    // Record commands
    frame.list->Reset();
    frame.list->TransitionResource(
      buffer, ResourceState::kPresent, ResourceState::kRenderTarget);
    frame.list->SetRenderTarget(rt);
    frame.list->ClearRenderTarget(rt, mClearColor);
    frame.list->SetViewport(EntireViewport());
    frame.list->SetScissorRectangle(EntireRectangle());

    frame.list->SetPrimitiveTopology(PrimitiveTopology::kTriangleList);
    frame.list->SetRootSignatureGraphics(mRootSignature);
    frame.list->SetPipelineState(mPipelineState);
    frame.list->SetVertexBuffer(mModel->GetVertexBuffer());
    frame.list->SetDescriptorHeap(mHeapSRV);
    frame.list->SetRootDescriptorTableGraphics(0, mHeapSRV->At(0));
    frame.list->SetRootDescriptorGraphics(1, frame.constBuf);
    frame.list->Draw(mModel->GetVertexCount());

    frame.list->TransitionResource(
      buffer, ResourceState::kRenderTarget, ResourceState::kPresent);
    frame.list->Close();

    // Submit commands
    GetGraphicsQueue()->Submit(frame.list);
    GetGraphicsQueue()->SignalSemaphore(frame.sem, ++frame.semVal);

    // Present
    GetSwapChain()->Present();
  }

  /* Update (Fixed) */
  void FixedUpdate() override
  {
    // Set window title based on VRAM usage
    const f64 usageGb = GetDevice()->GetMemoryUsage() / f64(1024 * 1024 * 1024);
    const f64 budgetGb =
      GetDevice()->GetMemoryBudget() / f64(1024 * 1024 * 1024);
    const String title = String::Format(
      "05 - Models (VRAM usage: {:.4f}/{:.4f}Gib)", usageGb, budgetGb);
    SetWindowTitle(title);
  }
};

// ========================================================================== //
// Main Function
// ========================================================================== //

int
main()
{
  // Create app
  App::CreateInfo appInfo{};
  appInfo.title = "05 - Models";
  appInfo.window.width = 1280;
  appInfo.window.height = 720;
  appInfo.flags = App::Flag::kExitOnEscape;
  appInfo.toggleFullscreenKey = Key::kF;
  Sample app(appInfo);

  // Run app
  app.Run();

  return 0;
}