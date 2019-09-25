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
#include "olivine/core/common.hpp"
#include "olivine/core/file/path.hpp"
#include "olivine/core/file/result.hpp"

// ========================================================================== //
// FileIO Declaration
// ========================================================================== //

namespace olivine {

/** \class FileIO
 * \author Filip Björklund
 * \date 25 july 2019 - 14:09
 * \brief File IO handle.
 * \details
 *  Class that represents an interface to perform operations on a file in the
 * filesystem.
 */
class FileIO
{
public:
  /** Opaque file handle **/
  using OpaqueHandle = void*;
  /** Invalid handle value **/
  static constexpr OpaqueHandle INVALID_HANDLE = nullptr;

  /** File IO opening flags **/
  enum class Flag : u8
  {
    /** Open file with read access **/
    kRead = Bit(0),
    /** Open file with write access **/
    kWrite = Bit(1),
    /** Open file with read and write access **/
    kReadWrite = kRead | kWrite,
    /** Open file with shared read access **/
    kShareRead = Bit(2),
    /** Open file with shared write access **/
    kShareWrite = Bit(3),
    /** Open file with shared read and write access **/
    kShareReadWrite = kShareRead | kShareWrite,
    /** Create the file if it does not already exists **/
    kCreate = Bit(4),
    /** Overwrite file if it already exists **/
    kOverwrite = Bit(5),
    /** Open file with cursor at the end for appending **/
    kAppend = Bit(6)
  };
  OL_ENUM_CLASS_OPERATORS(friend, Flag, u8);

private:
  /** Path to file **/
  Path mPath;
  /** File handle **/
  OpaqueHandle mHandle = INVALID_HANDLE;
  /** Whether file is open **/
  bool mIsOpen = false;

public:
  /** Create a file IO handle for performing operations on a file in the
   * filesystem. This function does not open the file, instead it only sets up
   * the handle. Call 'FileIO::Open' to open the file.
   * \brief Create file IO handle.
   * \param[in] path Path to file.
   */
  explicit FileIO(Path path);

  /** Destruct file IO handle. This also closes the file if it's still open.
   * \brief Destruct file IO handle.
   */
  ~FileIO();

  /** Open the file IO handle to start performing operations on the file. The
   * flags determine how the file is opened, and whether non-existent files
   * should be created, and existing files be truncated.
   * \brief Open handle.
   * \param[in] flags Flags specifying how the file should be opened.
   * \return Result.
   * - FileResult::kSuccess: Successfully opened file IO.
   */
  FileResult Open(Flag flags);

  /** Close a file IO handle. No more operations may be performed on the handle
   * after this function has been called.
   * \brief Close handle.
   * \return Result.
   */
  FileResult Close();

  /** Read data from the file into a buffer. The buffer and number of bytes must
   * be specified by the user. The number of read bytes are also returned as an
   * output parameter.
   * \brief Read from file.
   * \param[in] buffer Buffer to read into.
   * \param[in] toRead Number of bytes to read.
   * \param[out] read Number of bytes read.
   * \return Result.
   */
  FileResult Read(u8* buffer, u64 toRead, u64& read);

  /** Read the entire contents of the file into a string.
   * \brief Read file into string.
   * \param string String to read file into.
   * \return Result.
   */
  FileResult Read(String& string);

  /** Write data from a buffer into the file. The buffer and number of bytes to
   * write must be specified by the user. The number of bytes that was
   * successfully written is returned to the user in an output parameter.
   * \brief Write to file.
   * \param[in] buffer Buffer to write data from.
   * \param[in] toWrite Number of bytes to write.
   * \param[out] written Number of bytes written.
   * \return Result.
   */
  FileResult Write(const u8* buffer, u64 toWrite, u64& written) const;

  /** Write the contents of a string to the file.
   * \brief Write string.
   * \param string String to write.
   * \param written Number of bytes written.
   * \return Result.
   */
  FileResult Write(const String& string, u64& written) const;

  /** Write the contents of a string to the file.
   * \brief Write string.
   * \param string String to write.
   * \return Result.
   */
  FileResult Write(const String& string) const;

  /** Flush and buffered writes that has not yet been written to file.
   * \brief Flush file.
   * \return Result.
   */
  FileResult Flush() const;

  /** Seek to the specified offset in the file.
   * \brief Seek in file.
   * \param position Position to seek to.
   */
  void Seek(u64 position) const;

  /** Seek to the end of the file.
   * \brief Seek to end.
   */
  void SeekEnd() const;

  /** Returns whether or not the handle is currently open.
   * \brief Returns whether handle is open.
   * \return True if the handle is open otherwise false.
   */
  OL_NODISCARD bool IsOpen() const { return mIsOpen; }

  /** Returns the path to the file that the IO is created for.
   * \brief Returns path.
   * \return Path.
   */
  OL_NODISCARD const Path& GetPath() const { return mPath; }

  /** Returns the current position of the cursor in the file.
   * \brief Returns cursor position.
   * \return Cursor position.
   */
  OL_NODISCARD u64 GetCursorPosition() const;
};

}