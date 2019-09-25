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
// Console Declaration
// ========================================================================== //

namespace olivine {

/** Represents a way to access and interact with the standard output **/
class Console
{
  OL_NAMESPACE_CLASS(Console);

public:
  /** Type for a 8-bit ANSI color **/
  using Color = u8;

  /** Black **/
  static constexpr Color BLACK = 0;
  /** Black **/
  static constexpr Color RED_DARK = 1;
  /** Black **/
  static constexpr Color GREEN_DARK = 2;
  /** Black **/
  static constexpr Color YELLOW_DARK = 3;
  /** Black **/
  static constexpr Color BLUE_DARK = 4;
  /** Black **/
  static constexpr Color MAGENTA_DARK = 5;
  /** Black **/
  static constexpr Color CYAN_DARK = 6;
  /** Black **/
  static constexpr Color GRAY_LIGHT = 7;
  /** Black **/
  static constexpr Color GRAY_DARK = 8;
  /** Black **/
  static constexpr Color RED = 9;
  /** Black **/
  static constexpr Color GREEN = 10;
  /** Black **/
  static constexpr Color YELLOW = 11;
  /** Black **/
  static constexpr Color BLUE = 12;
  /** Black **/
  static constexpr Color MAGENTA = 13;
  /** Black **/
  static constexpr Color CYAN = 14;
  /** Black **/
  static constexpr Color WHITE = 15;

public:
  /** Write a formatted message to the standard output.
   * \note Message is formatted according to the rules of the 'fmt' library.
   * \brief Write to stdout.
   * \tparam ARGS Argument types.
   * \param format Message format string.
   * \param arguments Arguments to format string with.
   */
  template<typename... ARGS>
  static void Write(const String& format, ARGS&&... arguments);

  /** Write a formatted error message to the error output.
   * \note Message is formatted according to the rules of the 'fmt' library.
   * \brief Write to stderr.
   * \tparam ARGS Argument types.
   * \param format Message format string.
   * \param arguments Arguments to format string with.
   */
  template<typename... ARGS>
  static void WriteErr(const String& format, ARGS&&... arguments);

  /** Write a formatted message to the standard output followed by a newline.
   * \note Message is formatted according to the rules of the 'fmt' library.
   * \brief Write to stdout.
   * \tparam ARGS Argument types.
   * \param format Message format string.
   * \param arguments Arguments to format string with.
   */
  template<typename... ARGS>
  static void WriteLine(const String& format, ARGS&&... arguments);

  /** Write a formatted error message to the error output followed by a newline.
   * \note Message is formatted according to the rules of the 'fmt' library.
   * \brief Write to stderr.
   * \tparam ARGS Argument types.
   * \param format Message format string.
   * \param arguments Arguments to format string with.
   */
  template<typename... ARGS>
  static void WriteErrLine(const String& format, ARGS&&... arguments);

  /** Construct a colored string from the input string and an 8-bit ANSI color
   * value. Values from 0 to 255 are supported.
   * \brief Construct colored string.
   * \param string String to color.
   * \param color Color.
   * \return Colored string.
   */
  static String Colored(const String& string, Color color);

  /** Flush all buffered data that has not yet been printed to the console.
   * \brief Flush console.
   */
  static void Flush();

private:
  /** Write to standard output **/
  static void Write_(const String& message);

  /** Write to error output **/
  static void WriteErr_(const String& message);

  /** Write a line to standard output (newline is appended) **/
  static void WriteLine_(const String& message);

  /** Write a line to error output **/
  static void WriteErrLine_(const String& message);
};

// -------------------------------------------------------------------------- //

template<typename... ARGS>
void
Console::Write(const String& format, ARGS&&... arguments)
{
  Write_(String::Format(format, std::forward<ARGS>(arguments)...));
}

// -------------------------------------------------------------------------- //

template<typename... ARGS>
void
Console::WriteErr(const String& format, ARGS&&... arguments)
{
  WriteErr_(String::Format(format, std::forward<ARGS>(arguments)...));
}

// -------------------------------------------------------------------------- //

template<typename... ARGS>
void
Console::WriteLine(const String& format, ARGS&&... arguments)
{
  WriteLine_(String::Format(format, std::forward<ARGS>(arguments)...));
}

// -------------------------------------------------------------------------- //

template<typename... ARGS>
void
Console::WriteErrLine(const String& format, ARGS&&... arguments)
{
  WriteErrLine_(String::Format(format, std::forward<ARGS>(arguments)...));
}

}