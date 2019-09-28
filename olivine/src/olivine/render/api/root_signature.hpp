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

// ========================================================================== //
// RootSignature Declaration
// ========================================================================== //

namespace olivine {

class RootSignature
{
public:
  /* Root descriptor table info */
  struct Table
  {};

  /* Root constant info*/
  struct Constant
  {
    /** Register **/
    u32 reg;
    /** Space **/
    u32 space;
    /** Number of constants values **/
    u32 numValues;
  };

  /* Root descriptor info */
  struct Descriptor
  {
    /** Register **/
    u32 reg;
    /** Space **/
    u32 space;
  };

  /* Parameter */
  struct Parameter
  {
    /* Parameter data */
    std::variant<Table, Constant, Descriptor> data;
    /* Shader stages where parameter is accesible */
    ShaderStage stages;

    /** Create for table **/
    explicit Parameter(Table table, ShaderStage stages)
      : data(table)
      , stages(stages)
    {}

    /** Create for constant **/
    explicit Parameter(Constant constant, ShaderStage stages)
      : data(constant)
      , stages(stages)
    {}

    /** Create for descriptor **/
    explicit Parameter(Descriptor descriptor, ShaderStage stages)
      : data(descriptor)
      , stages(stages)
    {}
  };

  /* Creation information */
  struct CreateInfo
  {
    /* List of root parameters */
    std::vector<Parameter> parameters;
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
};

}