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

// Project headers
#include "olivine/render/api/d3d12_util.hpp"

// ========================================================================== //
// Descriptor Declaration
// ========================================================================== //

namespace olivine {

/** \class Descriptor
 * \author Filip Björklund
 * \date 25 september 2019 - 22:00
 * \brief Descriptor.
 * \details
 * Represents a descriptor in a descriptor heap.
 */
class Descriptor
{
  OL_DEFAULT_COPY(Descriptor);

public:
  /* Descriptor kinds */
  enum class Kind
  {
    /* CBV, SRV and UAV (combined) descriptor type kind */
    kCbvSrvUav,
    /* Sampler descriptor kind */
    kSampler,
    /* Render-target view, descriptor kind */
    kRtv,
    /* Depth-stenicl view, descriptor kind */
    kDsv,
  };

private:
  /* Kind */
  Kind mKind;
  /* CPU handle */
  D3D12_CPU_DESCRIPTOR_HANDLE mCpuHandle;
  /* GPU handle */
  D3D12_GPU_DESCRIPTOR_HANDLE mGpuHandle;

public:
  /** Create a descriptor from a specified type and 'CPU' and 'GPU' handles.
   * \brief Create descriptor.
   * \param kind Descriptor kind.
   * \param cpuHandle CPU handle.
   * \param gpuHandle GPU handle.
   */
  explicit Descriptor(Kind kind,
                      D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = { 0 },
                      D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = { 0 });

  /** Returns the kind of the descriptor.
   * \brief Returns kind.
   * \return Kind.
   */
  Kind GetKind() const { return mKind; }

  /** Returns the CPU handle for the descriptor.
   * \brief Returns CPU handle.
   * \return CPU handle.
   */
  D3D12_CPU_DESCRIPTOR_HANDLE CPU() const { return mCpuHandle; }

  /** Returns the GPU handle for the descriptor.
   * \brief Returns GPU handle.
   * \return GPU handle.
   */
  D3D12_GPU_DESCRIPTOR_HANDLE GPU() const { return mGpuHandle; }

public:
  /** Convert a 'DescriptorKind' to 'D3D12_DESCRIPTOR_HEAP_TYPE'
   * \brief Convert descriptor kind.
   * \param kind Kind to convert.
   * \return Converted kind.
   */
  static D3D12_DESCRIPTOR_HEAP_TYPE ToDescriptorType(Kind kind);
};

}

// ========================================================================== //
// DescriptorHeap Declaration
// ========================================================================== //

namespace olivine {

OL_FORWARD_DECLARE(Texture);

/** \class DescriptorHeap
 * \author Filip Björklund
 * \date 25 september 2019 - 20:44
 * \brief Descriptor heap.
 * \details
 * Represents a descriptor heap to where descriptors can be written and used by
 * the device.
 */
class DescriptorHeap
{
  OL_NO_COPY(DescriptorHeap);

private:
  /* Handle */
  ID3D12DescriptorHeap* mHandle;
  /* Kind of descriptors stored in heap */
  Descriptor::Kind mKind;
  /* Size of handles in heap */
  u32 mHandleSize;
  /* Capacity of heap */
  u32 mCount;

public:
  /** Create a descriptor heap for 'count' number of descriptors of the
   * specified descriptor type.
   * \brief Create descriptor heap.
   * \param kind Kind of descriptors that heap store.
   * \param count Number of descriptors of the kind that can be stored in the
   * descriptor heap.
   */
  DescriptorHeap(Descriptor::Kind kind, u32 count);

  /** Destroy the descriptor heap.
   * \brief Destroy descriptor heap.
   */
  ~DescriptorHeap();

  /** Write a render-target view (RTV) into the descriptor at the specified
   * index.
   * \brief Write RTV descriptor.
   * \param index Index to write resource at.
   * \param texture Texture to write RTV for.
   * \param format Format of the RTV.
   */
  Descriptor WriteDescriptorRTV(u32 index,
                                Texture* texture,
                                Format format = Format::kInvalid);

  /** Returns the descriptor in the heap at the specified index.
   * \brief Returns descriptor at index.
   * \param index Index to get descriptor at.
   * \return Descriptor at index
   */
  Descriptor At(u32 index);

  /** Returns the descriptor in the heap at the specified index.
   * \brief Returns descriptor at index.
   * \param index Index to get descriptor at.
   * \return Descriptor at index
   */
  Descriptor operator[](u32 index);

  /** Set the name of the descriptor heap.
   * \brief Set name.
   * \param name Name to set.
   */
  void SetName(const String& name);
};

}