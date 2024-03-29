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
#include <ostream>

// ========================================================================== //
// FileResult Enumeration
// ========================================================================== //

namespace olivine {

/** \enum FileResult
 * \author Filip Björklund
 * \date 25 july 2019 - 16:43
 * \brief File results.
 * \details
 * Enumeration of result values returned from the file APIs.
 */
enum class FileResult
{
  /** Success **/
  kSuccess,
  /** Unknown error **/
  kUnknownError,
  /** Invalid arguments **/
  kInvalidArgument,
  /** Failed to allocate memory **/
  kOutOfMemory,

  /** File is not open **/
  kNotOpen,
  /** File is already open **/
  kAlreadyOpen,

  /** File was not found at the specified path **/
  kNotFound,
  /** File already exists at the specified path **/
  kAlreadyExists,
  /** Access was denied trying to access file at the specified path **/
  kAccessDenied,
  /** Reached end-of-file (EOF) **/
  kEOF
};

}

// ========================================================================== //
// Functions
// ========================================================================== //

namespace olivine {

/** Function for writing a 'FileResult' to an output stream.
 * \brief Write 'FileResult' to output stream.
 * \param os Output stream.
 * \param result Result to write to stream.
 * \return Output stream.
 */
std::ostream&
operator<<(std::ostream& os, const FileResult& result);

}