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
#include "olivine/core/collection/array_list.hpp"
#include "olivine/core/types.hpp"
#include "olivine/render/api/d3d12_util.hpp"

// ========================================================================== //
// Device Declaration
// ========================================================================== //

namespace olivine {

OL_FORWARD_DECLARE(DescriptorAllocator);

/** \class Device
 * \author Filip Björklund
 * \date 25 september 2019 - 17:13
 * \brief Graphics device.
 * \details
 * Represents a graphics device (GPU).
 */
class Device
{
public:
  /* Criterias for selecting the physical device */
  enum class SelectionCriterion : u32
  {
    /** Default device **/
    kDefault = 0,
    /* Select the device that seems to have the best performance */
    kHighPerf,
    /* Select the device that seems to have the lowest power consumption */
    kLowPower,
    /* Select the device with the most amount of dedicated device memory */
    kHighMem
  };

  /** Creation information **/
  struct CreateInfo
  {
    /* Criterion for selecting the physical device that the device will
     * represent. By default the default device is choosen */
    SelectionCriterion selectionCriterion = SelectionCriterion::kDefault;
  };

  /* VRS feature levels */
  enum class TierVRS
  {
    /* No support for variable rate shading */
    kNoSupport,
    /* Support tier 1 */
    kTier1,
    /* Support tier 2 */
    kTier2,
  };

  /* Features structure */
  struct Features
  {
    /* Ray-tracing support */
    bool rayTracing;
    /* Variable-rate shading support */
    struct
    {
      /* Support tier */
      TierVRS tier;
    } vrs;
  };

private:
  /* Device handle */
  ID3D12Device4* mHandle = nullptr;
  /* Adapter */
  IDXGIAdapter3* mAdapter = nullptr;

  /* Allocator */
  D3D12MA::Allocator* mAllocator;

  /* Device features */
  Features mFeatures;

public:
  /** Create a device from a set of creation information.
   * \brief Create device.
   */
  explicit Device(const CreateInfo& createInfo);

  /** Create a device with delayed initialization by calling 'Init'.
   * \brief Create device
   */
  Device() = default;

  /** Initialize a device that was created with the default constructor.
   * \brief Init device
   */
  void Init(const CreateInfo& createInfo);

  /** Destroy a device.
   * \brief Destroy device.
   */
  ~Device();

  /** Returns a structure that can be used to determine what feature support the
   * device has.
   * \brief Returns device features.
   * \return Device features.
   */
  const Features& GetFeatures() const { return mFeatures; }

  /** Returns the current memory usage of the application in bytes.
   * \brief Returns memory usage.
   * \return Memory usage in bytes.
   */
  u64 GetMemoryUsage();

  /** Returns the memory budget for the application in bytes.
   * \brief Returns memory budget.
   * \return Memory budget in bytes.
   */
  u64 GetMemoryBudget();

  /** Returns the handle of the D3D12 device.
   * \brief Returns handle.
   * \return Device handle.
   */
  ID3D12Device4* GetHandle() const { return mHandle; }

  /** Returns the handle of the D3D12 device.
   * \brief Returns handle.
   * \return Device handle.
   */
  ID3D12Device4* operator->() const { return mHandle; }

  /** Returns the memory allocator for the device. This is used to allocate
   * memory from the device.
   * \brief Returns memory allocator.
   * \return Memory allocator.
   */
  D3D12MA::Allocator* GetAllocator() const { return mAllocator; }

private:
  /** Enumerate all the available adapters.
   * \brief Enumerate adapters.
   */
  static ArrayList<IDXGIAdapter3*> EnumerateAdapters();
};

}