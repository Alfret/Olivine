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

#pragma once

// ========================================================================== //
// Headers
// ========================================================================== //

// Standard headers
#include <vector>

// Project headers
#include "olivine/core/string.hpp"
#include "olivine/render/api/d3d12_util.hpp"

// ========================================================================== //
// PipelineState Declaration
// ========================================================================== //

namespace olivine {

OL_FORWARD_DECLARE(RootSignature);
OL_FORWARD_DECLARE(Path);

/** \class PipelineState
 * \author Filip Björklund
 * \date 26 september 2019 - 21:17
 * \brief Pipeline state.
 * \details
 * Represents a pipeline state.
 */
class PipelineState
{
public:
  /* Special attribute offset that specifies that the attribute should be
   * appended to the previous */
  static constexpr u32 kAttributeOffsetAppend = D3D12_APPEND_ALIGNED_ELEMENT;

  /* Pipeline kinds */
  enum class Kind
  {
    /* Graphics pipeline */
    kGraphics,
    /* Compute pipeline */
    kCompute,
    /* Ray-tracing pipeline */
    kRayTracing
  };

  /* Vertex attribute kinds */
  enum class VertexAttributeKind
  {
    /* 'byte' data */
    kByte,
    /* 'byte4' data */
    kByte4,
    /* 'float' data */
    kFloat,
    /* 'float2' data */
    kFloat2,
    /* 'float3' data */
    kFloat3,
    /* 'float4' data */
    kFloat4
  };

  /* Vertex attribute */
  struct VertexAttribute
  {
    /* HLSL semantic name (POSITION, TEXCOORD, ...)*/
    String semanticName;
    /* HLSL semantic index (POSITION0, POSITION1, ...)*/
    u32 semanticIndex;
    /* Attribute kind */
    VertexAttributeKind kind;
    /* Offset in bytes from the previous attribute. Can be set to */
    u32 offset = kAttributeOffsetAppend;
    /* Input slot. Default to zero (0), representing the first input-assembler
     * slot */
    u32 assemblerSlot;
  };

  /* Shader binary */
  struct ShaderBinary
  {
    /* Binary shader code (DXIL) */
    std::vector<u8> bytes;
  };

  /* Creation information */
  struct CreateInfo
  {
    /* Kind of pipeline */
    Kind kind;

    /* Root signature  */
    RootSignature* rootSignature;
    /* Vertex attributes */
    std::vector<VertexAttribute> vertexAttributes;
    /* Render target formats. The number of formats also specifiy the number of
     * render targets that will be used with the pipeline */
    std::vector<Format> renderTargetFormats;

    /* Vertex shader */
    ShaderBinary vs;
    /* Pixel shader */
    ShaderBinary ps;
    /* Compute shader */
    ShaderBinary cs;
  };

private:
  /* Pipeline state handle */
  ID3D12PipelineState* mHandle;

  /* Pipeline kind */
  Kind mKind;

public:
  /** Construct a pipeline state from a creation information structure.
   * \brief Construct pipeline state.
   * \param createInfo Creation information.
   */
  explicit PipelineState(const CreateInfo& createInfo);

  /** Destruct the pipeline state.
   * \brief Destruct pipeline state.
   */
  ~PipelineState();

  /** Set the name of the pipeline state.
   * \brief Set the name.
   * \param name Name to set.
   */
  void SetName(const String& name);

  /** Returns the handle for the pipeline state.
   * \brief Returns handle.
   * \return Handle.
   */
  ID3D12PipelineState* GetHandle() const { return mHandle; }

private:
  /** Setup the pipeline state for graphics **/
  void SetupForGraphics(const CreateInfo& createInfo);

  /** Setup the pipeline state for compute **/
  void SetupForCompute(const CreateInfo& createInfo);

  /** Setup the pipeline state for ray-tracing **/
  void SetupForRayTracing(const CreateInfo& createInfo);

public:
  /** Loads a shader from the specified path and returns the code as a shader
   * binary object.
   * \brief Load shader.
   * \param path Path to shader.
   * \return Shader binary.
   */
  static ShaderBinary LoadShader(const Path& path);

private:
  /** Convert from vertex attribute kind **/
  static DXGI_FORMAT FromAttributeKind(VertexAttributeKind kind);
};

}