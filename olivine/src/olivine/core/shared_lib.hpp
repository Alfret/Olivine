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
#include "olivine/core/string.hpp"

// ========================================================================== //
// SharedLib Declaration
// ========================================================================== //

namespace olivine {

/** Shared library **/
class SharedLib
{
public:
  /** Name prefix string **/
  static constexpr char8 kPrefix[] = "";
  /** Name extension string **/
  static constexpr char8 kExt[] = ".dll";

private:
  /** Library name **/
  String mName;

  /** Library handle **/
  void* mHandle = nullptr;

public:
  /** Create a shared library handle. This does not open the library **/
  explicit SharedLib(const String& file, const String& version = "");

  /** Destruct shared library handle **/
  ~SharedLib();

  /** Open library **/
  bool Open();

  /** Close library **/
  void Close();

  /** Returns whether or not library is open **/
  bool IsOpen();

  /** Returns a symbol from the shared library **/
  void* GetSymbol(const String& name);
};

}