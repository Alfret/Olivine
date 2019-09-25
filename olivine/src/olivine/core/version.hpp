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
#include "olivine/core/types.hpp"
#include "olivine/core/macros.hpp"

// ========================================================================== //
// Version Declaration
// ========================================================================== //

namespace olivine {

/** \class Version.
 * \author Filip Björklund
 * \date 22 july 2019 - 14:22
 * \brief Semantic version.
 * \details
 * Class that represents a semantic versioning number.
 */
class Version
{
private:
  /** Major version **/
  u16 mMajor;
  /** Minor version **/
  u16 mMinor;
  /** Patch version **/
  u32 mPatch;

public:
  /** Construct a semantic versioning number from the three (3) separate version
   * numbers.
   * \brief Create version.
   * \param major Major version.
   * \param minor Minor version.
   * \param patch Patch version.
   */
  Version(u16 major, u16 minor, u32 patch = 0);

  /** Construct a default semantic versioning number (0.1.0).
   * \brief Construct default version.
   */
  Version();

  /** Returns the major version of the semantic versioning number.
   * \brief Returns major version.
   * \return Major version.
   */
  u16& Major() { return mMajor; }

  /** Returns the major version of the semantic versioning number.
   * \brief Returns major version.
   * \return Major version.
   */
  OL_NODISCARD const u16& Major() const { return mMajor; }

  /** Returns the minor version of the semantic versioning number.
   * \brief Returns minor version.
   * \return Minor version.
   */
  u16& Minor() { return mMinor; }

  /** Returns the minor version of the semantic versioning number.
   * \brief Returns minor version.
   * \return Minor version.
   */
  OL_NODISCARD const u16& Minor() const { return mMinor; }

  /** Returns the patch version of the semantic versioning number.
   * \brief Returns patch version.
   * \return Patch version.
   */
  u32& Patch() { return mPatch; }

  /** Returns the patch version of the semantic versioning number.
   * \brief Returns patch version.
   * \return Patch version.
   */
  OL_NODISCARD const u32& Patch() const { return mPatch; }

  /** Equality operator **/
  friend bool operator==(const Version& lhs, const Version& rhs);

  /** Inequality operator **/
  friend bool operator!=(const Version& lhs, const Version& rhs);

  /** Less-than operator **/
  friend bool operator<(const Version& lhs, const Version& rhs);

  /** Greater-than operator **/
  friend bool operator>(const Version& lhs, const Version& rhs);

  /** Less-than-or-equal operator **/
  friend bool operator<=(const Version& lhs, const Version& rhs);

  /** Greater-than-or-equal operator **/
  friend bool operator>=(const Version& lhs, const Version& rhs);
};

}