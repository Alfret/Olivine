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
#include <string>

// Project headers
#include "olivine/core/types.hpp"
#include "olivine/core/macros.hpp"

// Thirdparty headers
#include "fmt/ostream.h"

// ========================================================================== //
// String Declaration
// ========================================================================== //

namespace olivine {

/** String **/
class String
{
public:
  /** Type of the underlying buffer (std::string) **/
  using BufferType = std::string;
  /** Length type **/
  using LengthType = u32;
  /** Size type **/
  using SizeType = u32;

  /** Codepoint type **/
  using Codepoint = u32;

  /** String iterator **/
  class Iterator
  {
  private:
    /** Underlying string reference **/
    const String& mString;
    /** Offset in string buffer **/
    SizeType mOffset;
    /** Current codepoint **/
    Codepoint mCodepoint;

  public:
    /** Construct iterator for String **/
    Iterator(const String& string, SizeType offset);

    /** Proceed in string **/
    void operator++();

    /** Check inequality **/
    bool operator!=(const Iterator& other);

    /** Retrieve reference **/
    SizeType operator*();
  };

  /** Value of an invalid codepoint **/
  static constexpr Codepoint InvalidCodepoint = Codepoint(-1);

private:
  /** Underlying string buffer **/
  BufferType mBuffer;
  /** Length in codepoints  **/
  LengthType mLength = 0;

public:
  /** Construct a string from a UTF-8 encoded c-string.
   * \brief Construct from UTF-8 string.
   * \param string String to construct from.
   */
  String(const char8* string);

  /** Construct a string from a UTF-16 encoded c-string.
   * \brief Construct from UTF-16 string.
   * \param string String to construct from.
   */
  String(const char16* string);

  /** Construct a string from a single codepoint.
   * \brief Construct string from codepoint.
   * \param codepoint Codepoint to construct from.
   * \param allocator Allocator to allocate memory with.
   */
  String(Codepoint codepoint);

  /** Default constructor **/
  String() = default;

  /** Default copy-constructor **/
  String(const String&) = default;

  /** Default move-constructor **/
  String(String&&) = default;

  /** Default copy-assignment **/
  String& operator=(const String&) = default;

  /** Default move-assignment **/
  String& operator=(String&&) noexcept = default;

  /** Returns the codepoint at the specified byte-offset in the string. If the
   * offset is not a start of a valid codepoint then -1 is returned.
   * \brief Returns codepoint at offset.
   * \param offset Offset to return codepoint at.
   * \param width Width of the codepoint.
   * \return Codepoint at offset or 'String::InvalidCodepoint' if it's not a
   * valid codepoint.
   */
  Codepoint AtByteOffset(u32 offset, u32& width) const;

  /** Find the index of the first occurance of a specified substring in the
   * string.
   * \brief Find occurance of substring.
   * \param substring Substring to find first occurance of.
   * \return Index of first occurance or -1 if the substring never occurs in the
   * string.
   */
  s64 Find(const String& substring) const;

  /** Returns the index of the first occurance of the specified codepoint.
   * \brief Returns index of first occurance.
   * \param codepoint Codepoint to return first occurance of.
   * \return Index of the first occurance of the codepoint or -1 if the
   * codepoint never occurs in the string.
   */
  s64 IndexOf(Codepoint codepoint) const;

  /** Returns the index of the last occurance of the specified codepoint.
   * \brief Returns index of last occurance.
   * \param codepoint Codepoint to return last occurance of.
   * \return Index of the last occurance of the codepoint or -1 if the
   * codepoint never occurs in the string.
   */
  s64 LastIndexOf(Codepoint codepoint) const;

  /** Returns whether or not the string begins with the specified other string.
   * \brief Returns whether string begins with other string.
   * \param string String to check if this string begins with.
   * \return True if this string begins with the other string.
   */
  bool StartsWith(const String& string) const;

  /** Returns whether or not the string begins with the specified codepoint.
   * \brief Returns whether string begins with codepoint.
   * \param codepoint Codepoint to check if string begins with.
   * \return True if the string begins with the specified codepoint otherwise
   * false.
   */
  bool StartsWith(Codepoint codepoint) const;

  /** Returns whether or not the string ends with the specified other string.
   * \brief Returns whether string ends with other string.
   * \param string String to check if this string ends with.
   * \return True if this string ends with the other string.
   */
  bool EndsWith(const String& string) const;

  /** Returns whether or not the string ends with the specified codepoint.
   * \brief Returns whether string ends with codepoint.
   * \param codepoint Codepoint to check if string ends with.
   * \return True if the string ends with the specified codepoint otherwise
   * false.
   */
  bool EndsWith(Codepoint codepoint) const;

  /** Replace all occurrences of the
   * \todo Currently the implementation is naïve and not very efficient. A
   * better substring searching algorithm should be used.
   * \brief Replace all occurrences of the string 'from' with the string 'to'.
   * \param from String to replace all occurrences of.
   * \param to String to replace the occurrences with.
   * \return Number of replaced occurrences.
   */
  u32 Replace(const String& from, const String& to);

  /** Remove all occurrences of the specified codepoint from the string.
   * \brief Remove all occurrences of codepoint.
   * \param codepoint Codepoint to remove occurrences of.
   * \return Number of removed codepoints.
   */
  u32 Remove(u32 codepoint);

  /** Returns a substring of the string from the 'from' index and 'count'
   * codepoints ahead.
   * \brief Returns substring.
   * \param from Index to get substring from.
   * \param count Number of codepoint in substring. -1 Means that the entire
   * string from the starting index should be returned.
   * \return Substring.
   */
  OL_NODISCARD String Substring(u64 from, s64 count = -1) const;

  /** Concatenate another string at the end of this string.
   * \brief Concatenate string.
   * \param string String to concatenate with.
   */
  void operator+=(const String& string);

  /** Concatenate another string at the end of this string.
   * \brief Concatenate string.
   * \param string String to concatenate with.
   */
  void operator+=(const char8* string);

  /** Returns the codepoint at the specified index in the string.
   * \note This function will traverse the entire string due to the
   * variable-length encoding nature of UTF-8.
   * \brief Returns codepoint at index.
   * \param index Index to get codepoint at.
   * \return Codepoint at index.
   */
  OL_NODISCARD u32 At(u32 index) const;

  /** Returns the codepoint at the specified index in the string.
   * \note This function will traverse the entire string due to the
   * variable-length encoding nature of UTF-8.
   * \brief Returns codepoint at index.
   * \param index Index to get codepoint at.
   * \return Codepoint at index.
   */
  OL_NODISCARD u32 operator[](u32 index) const;

  template<typename T>
  T As();

  /** Returns the iterator to the beginning of the string.
   * \brief Returns beginning iterator.
   * \return Begin iterator.
   */
  OL_NODISCARD Iterator Begin() const { return Iterator(*this, 0); }

  /** \copydoc ArrayList::Begin **/
  OL_NODISCARD Iterator begin() const { return Begin(); }

  /** Returns the iterator to the end of the string.
   * \brief Returns ending iterator.
   * \return End iterator.
   */
  OL_NODISCARD Iterator End() const { return Iterator(*this, GetSize()); }

  /** \copydoc ArrayList::End **/
  OL_NODISCARD Iterator end() const { return End(); }

  /** Returns the raw underlying UTF-8 data.
   * \brief Returns UTF-8 data.
   * \return UTF-8 string.
   */
  OL_NODISCARD const char8* GetUTF8() const { return mBuffer.c_str(); }

  /** Returns a UTF-16 encoded string converted from this string.
   * \brief Returns UTF-16 string.
   * \return UTF-16 string.
   */
  OL_NODISCARD char16* GetUTF16() const;

  /** Returns a reference to the internal 'std::string' buffer of the String
   * class
   * \brief Returns buffer.
   * \return Buffer.
   */
  OL_NODISCARD const std::string& GetBuffer() const { return mBuffer; }

  /** Returns whether or not the string is empty. This is the same as checking
   * if the length equals zero (0).
   * \brief Returns whether string is empty.
   * \return True if the string is empty otherwise false.
   */
  OL_NODISCARD bool IsEmpty() const { return GetLength() == 0; }

  /** Returns the length of the string in number of codepoints.
   * \note The value returned from this function may not be the same as from the
   * correspoind 'std::string'. For the reason that this string knows about
   * UTF-8.
   * \brief Returns length.
   * \return Length of the string in codepoints.
   */
  OL_NODISCARD LengthType GetLength() const { return mLength; }

  /** Returns the size of the string in number of bytes.
   * \note Unlike 'String::GetLength()' this function returns the same value as
   * the corresponding std::string.
   * \note Value returned are always equal to, or greater than, the values
   * returned from the 'String::GetLength()' function.
   * \brief Returns size.
   * \return Size of the string in bytes.
   */
  OL_NODISCARD SizeType GetSize() const
  {
    return static_cast<u32>(mBuffer.size());
  }

public:
  /** Output stream function **/
  friend std::ostream& operator<<(std::ostream& stream, const String& string)
  {
    return stream << string.mBuffer;
  }

  /** Concatenation **/
  friend String operator+(const String& str0, const String& str1)
  {
    return String((str0.mBuffer + str1.mBuffer).c_str());
  }

  /** Concatenation **/
  friend String operator+(const String& str0, const char8* str1)
  {
    return str0 + String(str1);
  }

  /** Concatenation **/
  friend String operator+(const char8* str0, const String& str1)
  {
    return String(str0) + str1;
  }

  /** Equality **/
  friend bool operator==(const String& str0, const String& str1)
  {
    return strcmp(str0.GetUTF8(), str1.GetUTF8()) == 0;
  }

  /** Equality **/
  friend bool operator==(const String& str0, const char8* str1)
  {
    return str0 == String(str1);
  }

  /** Equality **/
  friend bool operator==(const char8* str0, const String& str1)
  {
    return String(str0) == str1;
  }

  /** Inequality **/
  friend bool operator!=(const String& str0, const String& str1)
  {
    return !(str0 == str1);
  }

  /** Inequality **/
  friend bool operator!=(const String& str0, const char8* str1)
  {
    return str0 != String(str1);
  }

  /** Inequality **/
  friend bool operator!=(const char8* str0, const String& str1)
  {
    return String(str0) != str1;
  }

  /** Format a string according to the rules of the fmt library. The format
   * string is formatted with the arguments.
   * \brief Format a string.
   * \tparam ARGS Argument types.
   * \param format String that describes format.
   * \param arguments Arguments to format string with.
   * \return Formatted string.
   *
   * \example
   * // "Sum: 2 + 2 = 4"
   * String str = String::Format("Sum: {} + {} = {}", 2, "2", 4);
   */
  template<typename... ARGS>
  OL_NODISCARD static String Format(const String& format, ARGS&&... arguments);

  /** Convert the given value to string form.
   * \brief Convert value to string.
   * \tparam T Type of the value.
   * \param value Value to get type of.
   * \return Value in string form.
   */
  template<typename T>
  static String ToString(T value);

  /** Returns the width of a codepoint in bytes.
   * \brief Returns codepoint width.
   * \param codepoint Codepoint to get width of.
   * \return Width of codepoint in bytes.
   */
  static LengthType CodepointWidth(Codepoint codepoint);
};

}

// ========================================================================== //
// String Implementation
// ========================================================================== //

namespace olivine {

template<typename T>
T
String::As()
{
  return T{ *this };
}

// -------------------------------------------------------------------------- //

template<>
u32
String::As();

// -------------------------------------------------------------------------- //

template<typename T>
String
String::ToString(T value)
{
  return String(std::to_string(value).c_str());
}

// -------------------------------------------------------------------------- //

template<typename... ARGS>
String
String::Format(const String& format, ARGS&&... arguments)
{
  return String(
    fmt::format(format.mBuffer, std::forward<ARGS>(arguments)...).c_str());
}

}

// ========================================================================== //
// Functions
// ========================================================================== //

namespace std {
template<>
struct hash<olivine::String>
{
  std::size_t operator()(const olivine::String& string) const
  {
    return std::hash<std::string>{}(string.GetBuffer());
  }
};

}