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

#include "olivine/core/shared_lib.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/core/platform/headers.hpp"
#include "olivine/core/assert.hpp"
#include "olivine/core/console.hpp"

// ========================================================================== //
// SharedLib Implementation
// ========================================================================== //

namespace olivine {

SharedLib::SharedLib(const String& file, const String& version)
  : mName(kPrefix + file + kExt)
{}

// -------------------------------------------------------------------------- //

SharedLib::~SharedLib()
{
  if (IsOpen()) {
    Close();
  }
}

// -------------------------------------------------------------------------- //

bool
SharedLib::Open()
{
  char16* _name = mName.GetUTF16();
  mHandle = static_cast<void*>(LoadLibraryW(_name));
  delete _name;
  return true;
}

// -------------------------------------------------------------------------- //

void
SharedLib::Close()
{
  FreeLibrary(static_cast<HMODULE>(mHandle));
  mHandle = nullptr;
}

// -------------------------------------------------------------------------- //

bool
SharedLib::IsOpen()
{
  return mHandle != nullptr;
}

// -------------------------------------------------------------------------- //

void*
SharedLib::GetSymbol(const String& name)
{
  Assert(mHandle, "Cannot load symbol from shared library that is not open");

  HMODULE module = static_cast<HMODULE>(mHandle);
  void* sym = static_cast<void*>(GetProcAddress(module, name.GetUTF8()));
  return sym;
}

}