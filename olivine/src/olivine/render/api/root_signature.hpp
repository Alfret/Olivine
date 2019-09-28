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
#include <variant>

// Project headers
#include "olivine/core/collection/array_list.hpp"
#include "olivine/render/api/d3d12_util.hpp"
#include "olivine/render/api/common.hpp"
#include "olivine/render/api/sampler.hpp"

// ========================================================================== //
// RootSignature Declaration
// ========================================================================== //

namespace olivine {

/** \class RootSignature
 * \author Filip Björklund
 * \date 28 september 2019 - 17:20
 * \brief Root signature.
 * \details
 * Represents a root signature that declares the shader descriptor interface.
 */
class RootSignature
{
public:
  /* Maximum number of root parameters */
  static constexpr u32 kMaxRootParameters = 32;
  /* Maximum number of root descriptor table ranges for a single root descriptor
   * table */
  static constexpr u32 kMaxRootDescriptorTableRanges = 16;
  /* Maximum number of static samplers */
  static constexpr u32 kMaxStaticSamplers = 32;
  /* Values that specifies that a descriptor range appears immediately after the
   * previous range in the descriptor table */
  static constexpr u32 kRootTableRangeAppend =
    D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

  /* Root descriptor kind. These are specified both for root table ranges and
   * for root descriptors */
  enum class RootDescriptorKind
  {
    /* SRV  */
    kSrv,
    /* UAV */
    kUav,
    /* CBV */
    kCbv,
    /* Sampler. Not supported for root descriptors */
    kSampler
  };

  /* Root descriptor table range */
  struct RootTableRange
  {
    /* Kind of root descriptor table range */
    RootDescriptorKind kind;
    /* Number of descriptors in range */
    u32 count;
    /* Base register */
    u32 baseReg;
    /* Space */
    u32 space;
    /* Offset in descriptor table where range starts. Use the special value
     * 'kRootTableRangeAppend' to specify that this range appears right after
     * the previous range in the root table */
    u32 offsetInTable = kRootTableRangeAppend;
  };

  /* Root descriptor table info */
  struct RootTable
  {
    /* Table ranges. The maxmimum number of ranges that are supported for a
     * single root descriptor table are 'kMaxRootDescriptorTableRanges' */
    std::vector<RootTableRange> ranges;
  };

  /* Root constant info*/
  struct RootConstant
  {
    /* Register */
    u32 reg;
    /* Space */
    u32 space;
    /* Number of constants values */
    u32 numValues;
  };

  /* Root descriptor info */
  struct RootDescriptor
  {
    /* Register */
    u32 reg;
    /* Space */
    u32 space;
    /* Kind of descriptor */
    RootDescriptorKind kind;
  };

  /* Root parameter */
  struct RootParameter
  {
    /* Root parameter data */
    std::variant<RootTable, RootConstant, RootDescriptor> data;
    /* Shader stages where parameter is accesible */
    ShaderStage stages;

    /** Create for table **/
    explicit RootParameter(RootTable table, ShaderStage stages)
      : data(table)
      , stages(stages)
    {}

    /** Create for constant **/
    explicit RootParameter(RootConstant constant, ShaderStage stages)
      : data(constant)
      , stages(stages)
    {}

    /** Create for descriptor **/
    explicit RootParameter(RootDescriptor descriptor, ShaderStage stages)
      : data(descriptor)
      , stages(stages)
    {}
  };

  /* Static sampler */
  struct StaticSampler
  {
    /* Minification filter */
    Sampler::Filter minFilter = Sampler::Filter::kPoint;
    /* Magnification filter */
    Sampler::Filter magFilter = Sampler::Filter::kPoint;
    /* Mip-level filter */
    Sampler::Filter mipFilter = Sampler::Filter::kPoint;
    /* Filter reduction kind */
    Sampler::ReductionKind reductionKind = Sampler::ReductionKind::kStandard;
    /* Whether to use anisotropic filtering */
    bool anisotropicFiltering = false;
    /** Address mode U **/
    Sampler::AddressMode addressModeU = Sampler::AddressMode::kWrap;
    /** Address mode V **/
    Sampler::AddressMode addressModeV = Sampler::AddressMode::kWrap;
    /** Address mode W **/
    Sampler::AddressMode addressModeW = Sampler::AddressMode::kWrap;
    /** Mip LOD bias **/
    f32 mipLODBias = 0.0f;
    /** Max anisotropy **/
    u32 maxAnisotropy = 0;
    /** Comparison function **/
    ComparisonFunction comparisonFunction = ComparisonFunction::kAlways;
    /** Border color **/
    Sampler::BorderColor borderColor = Sampler::BorderColor::kBlack;
    /** Min LOD **/
    f32 minLOD = 0.0f;
    /** Max LOD **/
    f32 maxLOD = 0.0f;

    /** Register **/
    u32 reg = 0;
    /** Space **/
    u32 space = 0;
    /** Shader stages that can access parameter **/
    ShaderStage accessibleStages = ShaderStage::kNone;
  };

  /* Creation information */
  struct CreateInfo
  {
    /* List of root parameters The maximum number of root parameters supported
     * are 'kMaxRootParameters' */
    std::vector<RootParameter> parameters;
    /* List of static samplers. The maximum number of static samplers supported
     * are 'kMaxStaticSamplers' */
    std::vector<StaticSampler> staticSamplers;
  };

private:
  /* Root signature handle */
  ID3D12RootSignature* mHandle;

public:
  /** Construct a root signature from a creation info structure that contains
   * information about all the root parameters and static samplers.
   * \brief Construct root signature.
   * \pararm createInfo Creation information.
   */
  explicit RootSignature(const CreateInfo& createInfo);

  /** Destruct the root signature.
   * \brief Destruct root signature.
   */
  ~RootSignature();

  /** Returns the handle of the root signature.
   * \brief Returns handle.
   * \return Handle.
   */
  ID3D12RootSignature* GetHandle() const { return mHandle; }

  /** Set the name of the root signature.
   * \brief Set name.
   * \param name Name to set.
   */
  void SetName(const String& name);

private:
  /** Convert 'ShaderStage' combination to 'D3D12_SHADER_VISIBILITY' **/
  static D3D12_SHADER_VISIBILITY ToShaderVisibility(ShaderStage stages);

  /** Convert from 'RootDescriptorKind' to 'D3D12_ROOT_PARAMETER_TYPE' **/
  static D3D12_ROOT_PARAMETER_TYPE ToRootParameterType(RootDescriptorKind kind);

  /** Convert from 'RootDescriptorKind' to 'D3D12_DESCRIPTOR_RANGE_TYPE' **/
  static D3D12_DESCRIPTOR_RANGE_TYPE ToDescriptorRangeType(
    RootDescriptorKind kind);
};

}