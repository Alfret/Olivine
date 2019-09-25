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

#include "olivine/core/console.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/core/platform/headers.hpp"

// ========================================================================== //
// Console Declaration
// ========================================================================== //

namespace olivine {

String
Console::Colored(const String& string, Console::Color color)
{
  return String::Format("\033[38;5;{}m{}\033[0m", color, string);
}

// -------------------------------------------------------------------------- //

void
Console::Flush()
{
  fflush(stdout);
}

// -------------------------------------------------------------------------- //

void
Console::Write_(const String& message)
{
  char16* _message = message.GetUTF16();
  OutputDebugStringW(_message);
  delete[] _message;
}

// -------------------------------------------------------------------------- //

void
Console::WriteErr_(const String& message)
{
  char16* _message = message.GetUTF16();
  OutputDebugStringW(_message);
  delete[] _message;
}

// -------------------------------------------------------------------------- //

void
Console::WriteLine_(const String& message)
{
  char16* _message = message.GetUTF16();
  OutputDebugStringW(_message);
  delete[] _message;
}

// -------------------------------------------------------------------------- //

void
Console::WriteErrLine_(const String& message)
{
  char16* _message = message.GetUTF16();
  OutputDebugStringW(_message);
  delete[] _message;
}

}