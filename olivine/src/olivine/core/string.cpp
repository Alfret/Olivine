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

#include "olivine/core/string.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/core/assert.hpp"

// Thirdparty headers
#include "thirdparty/alflib/alf_unicode.h"

// ========================================================================== //
// String Implementation
// ========================================================================== //

namespace olivine {

String::Iterator::Iterator(const String& string, SizeType offset)
  : mString(string)
  , mOffset(offset)
  , mCodepoint(0)
{
  u32 numBytes;
  alfUTF8Decode(string.GetUTF8(), offset, &mCodepoint, &numBytes);
}

// -------------------------------------------------------------------------- //

void
String::Iterator::operator++()
{
  mOffset += alfUTF8CodepointWidth(mCodepoint);
  u32 numBytes;
  alfUTF8Decode(mString.GetUTF8(), mOffset, &mCodepoint, &numBytes);
}

// -------------------------------------------------------------------------- //

bool
String::Iterator::operator!=(const String::Iterator& other)
{
  return &mString != &other.mString || mOffset != other.mOffset;
}

// -------------------------------------------------------------------------- //

typename String::Codepoint String::Iterator::operator*()
{
  return mCodepoint;
}

// -------------------------------------------------------------------------- //

String::String(const char8* string)
  : mBuffer(string)
  , mLength(static_cast<LengthType>(alfUTF8StringLength(string)))
{}

// -------------------------------------------------------------------------- //

String::String(const char16* string)
{
  u32 numBytes;
  AlfBool success = alfUTF16ToUTF8(
    reinterpret_cast<const AlfChar16*>(string), &numBytes, nullptr);
  OL_ASSERT(success, "Failed to convert UTF-16 to UTF-8");
  char* buffer = new char[numBytes + 1ull];
  buffer[numBytes] = 0;
  success = alfUTF16ToUTF8(
    reinterpret_cast<const AlfChar16*>(string), &numBytes, buffer);
  OL_ASSERT(success, "Failed to convert UTF-16 to UTF-8");
  mBuffer = BufferType(buffer);
  mLength = static_cast<u32>(alfUTF8StringLength(buffer));
  delete[] buffer;
}

// -------------------------------------------------------------------------- //

String::String(String::Codepoint codepoint)
{
  char8 encoded[5];
  u32 numBytes;
  const AlfBool success = alfUTF8Encode(encoded, 0, codepoint, &numBytes);
  OL_ASSERT(success, "Failed to construct string from codepoint");
  encoded[numBytes] = 0;
  mBuffer = BufferType(encoded);
  mLength = 1;
}

// -------------------------------------------------------------------------- //

String::Codepoint
String::AtByteOffset(u32 offset, u32& width) const
{
  // Decode at offset
  u32 codepoint, numBytes;
  const AlfBool success =
    alfUTF8Decode(GetUTF8(), offset, &codepoint, &numBytes);
  if (!success) {
    return -1;
  }

  // Set width and return codepoint
  width = numBytes;
  return codepoint;
}

// -------------------------------------------------------------------------- //

s64
String::Find(const String& substring) const
{
  const std::string::size_type pos = mBuffer.find(substring.mBuffer);
  if (pos == std::string::npos) {
    return -1;
  }
  return pos;
}

// -------------------------------------------------------------------------- //

s64
String::IndexOf(String::Codepoint codepoint) const
{
  return alfUTF8IndexOf(GetUTF8(), codepoint);
}

// -------------------------------------------------------------------------- //

s64
String::LastIndexOf(String::Codepoint codepoint) const
{
  return alfUTF8LastIndexOf(GetUTF8(), codepoint);
}

// -------------------------------------------------------------------------- //

bool
String::StartsWith(const String& string) const
{
  if (GetSize() < string.GetSize()) {
    return false;
  }
  return 0 == memcmp(GetUTF8(), string.GetUTF8(), string.GetSize());
}

// -------------------------------------------------------------------------- //

bool
String::StartsWith(String::Codepoint codepoint) const
{
  return mLength > 0 && At(0) == codepoint;
}

// -------------------------------------------------------------------------- //

bool
String::EndsWith(const String& string) const
{
  if (GetSize() < string.GetSize()) {
    return false;
  }
  return 0 == memcmp(GetUTF8() + GetSize() - string.GetSize(),
                     string.GetUTF8(),
                     string.GetSize());
}

// -------------------------------------------------------------------------- //

bool
String::EndsWith(String::Codepoint codepoint) const
{
  // Check that the codepoint is even possible to be at the end
  const char8* str = GetUTF8();
  const u32 width = CodepointWidth(codepoint);
  if (width > GetSize()) {
    return false;
  }

  // Decode at expected location from end
  u32 _codepoint, numBytes;
  if (alfUTF8Decode(str, GetSize() - width, &_codepoint, &numBytes)) {
    return _codepoint == codepoint;
  }
  return false;
}

// -------------------------------------------------------------------------- //

u32
String::Replace(const String& from, const String& to)
{
  s64 index;
  u32 count = 0;
  while ((index = Find(from)) >= 0) {
    String str0 = mBuffer.substr(0, index).c_str();
    String str1 = mBuffer.substr(index + from.GetSize()).c_str();
    operator=(str0 + to + str1);
    count++;
  }
  return count;
}

// -------------------------------------------------------------------------- //

u32
String::Remove(u32 codepoint)
{
  // Encode the codepoint into a string
  char8 encoded[5];
  u32 numBytes;
  const AlfBool success = alfUTF8Encode(encoded, 0, codepoint, &numBytes);
  if (!success) {
    return 0;
  }
  encoded[numBytes] = 0;

  // Use replace
  return Replace(encoded, "");
}

// -------------------------------------------------------------------------- //

String
String::Substring(u64 from, s64 count) const
{
  // Retrieve substring
  char8* substring = alfUTF8Substring(GetUTF8(), from, count);
  String output;
  output.mBuffer = BufferType{ substring, mBuffer.get_allocator() };
  output.mLength = LengthType(alfUTF8StringLength(output.mBuffer.c_str()));
  free(substring);
  return output;
}

// -------------------------------------------------------------------------- //

void
String::operator+=(const String& string)
{
  mBuffer += string.mBuffer;
  mLength = LengthType(alfUTF8StringLength(mBuffer.c_str()));
}

// -------------------------------------------------------------------------- //

void
String::operator+=(const char8* string)
{
  mBuffer += string;
  mLength = LengthType(alfUTF8StringLength(mBuffer.c_str()));
}

// -------------------------------------------------------------------------- //

u32
String::At(u32 index) const
{
  return alfUTF8AtIndex(GetUTF8(), static_cast<u64>(index));
}

// -------------------------------------------------------------------------- //

u32 String::operator[](u32 index) const
{
  return At(index);
}

// -------------------------------------------------------------------------- //

template<>
u32
String::As()
{
  return u32(strtoul(mBuffer.c_str(), nullptr, 10));
}

// -------------------------------------------------------------------------- //

char16*
String::GetUTF16() const
{
  const char8* _string = GetUTF8();
  u32 numCodeUnits;
  AlfBool success = alfUTF8ToUTF16(_string, &numCodeUnits, nullptr);
  OL_ASSERT(success, "Failed to convert UTF-16 to UTF-8");

  char16* buffer = new char16[numCodeUnits + 1ull];
  buffer[numCodeUnits] = 0;
  success = alfUTF8ToUTF16(
    _string, &numCodeUnits, reinterpret_cast<AlfChar16*>(buffer));
  OL_ASSERT(success, "Failed to convert UTF-16 to UTF-8");
  return buffer;
}

// -------------------------------------------------------------------------- //

String::LengthType
String::CodepointWidth(Codepoint codepoint)
{
  return alfUTF8CodepointWidth(codepoint);
}

}