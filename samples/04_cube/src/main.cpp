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
#include <olivine/math/vector2f.hpp>
#include <olivine/math/vector3f.hpp>
#include <olivine/render/color.hpp>
#include <olivine/render/api/swap_chain.hpp>
#include <olivine/render/api/command_list.hpp>
#include <olivine/render/api/common.hpp>
#include <olivine/render/api/root_signature.hpp>
#include <olivine/render/api/pipeline_state.hpp>
#include <olivine/render/api/vertex_buffer.hpp>
#include <olivine/render/api/index_buffer.hpp>
#include <olivine/render/api/texture.hpp>
#include <olivine/render/api/upload.hpp>

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
  Color mClearColor = Color::FromHex(0x80bb01ff);

  /* Command list for uploading */
  CommandList* mUploadList;

  /* Root signature */
  RootSignature* mRootSignature = nullptr;
  /* Pipeline state */
  PipelineState* mPipelineState = nullptr;

  /* Vertex buffer */
  VertexBuffer* mVertexBuffer = nullptr;
  /* Index buffer */
  IndexBuffer* mIndexBuffer = nullptr;

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
    }

    // Create root signature
    RootSignature::CreateInfo rootSignatureInfo;
    mRootSignature = new RootSignature(rootSignatureInfo);

    // Create pipeline state
    PipelineState::CreateInfo pipelineStateInfo{};
    pipelineStateInfo.kind = PipelineState::Kind::kGraphics;
    pipelineStateInfo.rootSignature = mRootSignature;
    pipelineStateInfo.renderTargetFormats.push_back(
      GetSwapChain()->GetFormat());
    pipelineStateInfo.vs = PipelineState::LoadShader(Path{ "res/cube_vs.cso" });
    pipelineStateInfo.ps = PipelineState::LoadShader(Path{ "res/cube_ps.cso" });
    pipelineStateInfo.vertexAttributes = {
      PipelineState::VertexAttribute{
        "POSITION", 0, PipelineState::VertexAttributeKind::kFloat3 },
      PipelineState::VertexAttribute{
        "COLOR",
        0,
        PipelineState::VertexAttributeKind::kByte4,
        offsetof(Vertex, color) }
    };
    pipelineStateInfo.cullMode = CullMode::kBack;
    mPipelineState = new PipelineState(pipelineStateInfo);
    mPipelineState->SetName("MainPipelineState");

    // Create upload command list
    mUploadList = new CommandList(CommandQueue::Kind::kCopy);

    // Create vertex buffer
    Vertex vertices[4];
    vertices[0].pos = Vector3F{ -0.28f, -0.5f, 0.0f };
    vertices[0].color = Color::kWhite;
    vertices[1].pos = Vector3F{ -0.28f, 0.5f, 0.0f };
    vertices[1].color = Color::kWhite;
    vertices[2].pos = Vector3F{ 0.28f, 0.5f, 0.0f };
    vertices[2].color = Color::kWhite;
    vertices[3].pos = Vector3F{ 0.28f, -0.5f, 0.0f };
    vertices[3].color = Color::kWhite;
    VertexBuffer::CreateInfo vertexBufferInfo;
    vertexBufferInfo.size = sizeof vertices;
    vertexBufferInfo.stride = sizeof Vertex;
    vertexBufferInfo.heapKind = HeapKind::kUpload;
    mVertexBuffer = new VertexBuffer(vertexBufferInfo);
    mVertexBuffer->Write((u8*)vertices, sizeof vertices);
    mVertexBuffer->SetName("MainVertexBuffer");

    // Create index buffer
    u16 indices[6] = { 0, 1, 2, 0, 2, 3 };
    IndexBuffer::CreateInfo indexBufferInfo;
    indexBufferInfo.count = 6;
    indexBufferInfo.format = IndexBuffer::Format::kU16;
    indexBufferInfo.heapKind = HeapKind::kUpload;
    mIndexBuffer = new IndexBuffer(indexBufferInfo);
    mIndexBuffer->Write(indices, 6);
    mIndexBuffer->SetName("MainIndexBuffer");
  }

  /** Cleanup **/
  ~Sample()
  {
    FlushQueues();
    for (Frame& frame : mFrames) {
      delete frame.list;
      delete frame.sem;
    }
    delete mIndexBuffer;
    delete mVertexBuffer;
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
    frame.list->SetVertexBuffer(mVertexBuffer);
    frame.list->SetIndexBuffer(mIndexBuffer);
    frame.list->DrawIndexed(6);

    frame.list->TransitionResource(
      buffer, ResourceState::kRenderTarget, ResourceState::kPresent);
    frame.list->Close();

    // Submit commands
    GetGraphicsQueue()->Submit(frame.list);
    GetGraphicsQueue()->SignalSemaphore(frame.sem, ++frame.semVal);

    // Present
    GetSwapChain()->Present();
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
  appInfo.title = "04 - Cube";
  appInfo.window.width = 800;
  appInfo.window.height = 450;
  appInfo.flags = App::Flag::kExitOnEscape;
  appInfo.toggleFullscreenKey = Key::kF;
  Sample app(appInfo);

  // Run app
  app.Run();

  return 0;
}