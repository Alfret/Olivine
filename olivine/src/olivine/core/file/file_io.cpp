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

#include "olivine/core/file/file_io.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/core/file/file_system.hpp"

// ========================================================================== //
// Functions
// ========================================================================== //

namespace olivine {

FileResult
FileIOErrorFromErrorWin32(DWORD error)
{
  switch (error) {
    case ERROR_SUCCESS:
      return FileResult::kSuccess;
    case ERROR_FILE_EXISTS:
      return FileResult::kAlreadyExists;
    case ERROR_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
      return FileResult::kNotFound;
    case ERROR_ACCESS_DENIED:
      return FileResult::kAccessDenied;
    default:
      return FileResult::kUnknownError;
  }
}

}

// ========================================================================== //
// FileIO Implementation
// ========================================================================== //

namespace olivine {

FileIO::FileIO(Path path)
  : mPath(std::move(path))
{}

// -------------------------------------------------------------------------- //

FileIO::~FileIO()
{
  if (mIsOpen) {
    Close();
  }
}

// -------------------------------------------------------------------------- //

FileResult
FileIO::Open(FileIO::Flag flags)
{
  // Check that file is not already open
  if (mIsOpen) {
    return FileResult::kAlreadyOpen;
  }

  // Access flags
  DWORD access = 0;
  if (bool(flags & Flag::kRead)) {
    access |= GENERIC_READ;
  }
  if (bool(flags & Flag::kWrite)) {
    access |= GENERIC_WRITE;
  }

  // Sharing mode
  DWORD sharing = 0;
  if (bool(flags & Flag::kShareRead)) {
    sharing |= FILE_SHARE_READ;
  }
  if (bool(flags & Flag::kShareWrite)) {
    sharing |= FILE_SHARE_WRITE;
  }

  // Disposition
  const bool overwrite = bool(flags & Flag::kOverwrite);
  DWORD disposition = overwrite ? TRUNCATE_EXISTING : OPEN_EXISTING;
  if (bool(flags & Flag::kCreate)) {
    disposition = overwrite ? CREATE_ALWAYS : OPEN_ALWAYS;
  }

  // Attributes
  const DWORD attributes = FILE_ATTRIBUTE_NORMAL;

  // Open handle
  char16* wpath = mPath.GetPathString().GetUTF16();
  mHandle = CreateFileW(
    wpath, access, sharing, nullptr, disposition, attributes, nullptr);
  delete[] wpath;
  if (mHandle == INVALID_HANDLE_VALUE) {
    const DWORD error = GetLastError();
    return FileIOErrorFromErrorWin32(error);
  }

  // Finalize
  mIsOpen = true;
  return FileResult::kSuccess;
}

// -------------------------------------------------------------------------- //

FileResult
FileIO::Close()
{
  // Check that file is open
  if (!mIsOpen) {
    return FileResult::kNotOpen;
  }

  // Flush any content
  const FileResult result = Flush();
  if (result != FileResult::kSuccess) {
    return result;
  }

  // Close file
  CloseHandle(mHandle);

  // Finalize
  mIsOpen = false;
  return FileResult::kSuccess;
}

// -------------------------------------------------------------------------- //

FileResult
FileIO::Read(u8* buffer, u64 toRead, u64& read)
{
  DWORD _read;
  const BOOL result =
    ReadFile(mHandle, buffer, static_cast<DWORD>(toRead), &_read, nullptr);
  if (result != TRUE) {
    const DWORD error = GetLastError();
    return FileIOErrorFromErrorWin32(error);
  }
  read = static_cast<u64>(_read);
  return FileResult::kSuccess;
}

// -------------------------------------------------------------------------- //

FileResult
FileIO::Read(u8* buffer, u64 toRead)
{
  u64 read;
  return Read(buffer, toRead, read);
}

// -------------------------------------------------------------------------- //

FileResult
FileIO::Read(String& string)
{
  // Allocate buffer
  const u64 size = FileSystem::GetSize(mPath);
  char8* buffer = static_cast<char8*>(
    Memory::Allocate(sizeof(char8) * (size + 1), Memory::MIN_ALIGN));
  if (!buffer) {
    return FileResult::kOutOfMemory;
  }
  buffer[size] = 0;

  // Read file
  u64 read;
  const FileResult result = Read(reinterpret_cast<u8*>(buffer), size, read);
  if (result != FileResult::kSuccess) {
    Memory::Free(buffer);
    return result;
  }

  // Create string and return
  string = String(buffer);
  Memory::Free(buffer);
  return FileResult::kSuccess;
}

// -------------------------------------------------------------------------- //

FileResult
FileIO::Write(const u8* buffer, u64 toWrite, u64& written) const
{
  DWORD _written;
  const BOOL success =
    WriteFile(mHandle, buffer, static_cast<DWORD>(toWrite), &_written, nullptr);
  if (success != TRUE) {
    return FileIOErrorFromErrorWin32(GetLastError());
  }
  written = _written;
  return FileResult::kSuccess;
}

// -------------------------------------------------------------------------- //

FileResult
FileIO::Write(const u8* buffer, u64 toWrite) const
{
  u64 written;
  return Write(buffer, toWrite, written);
}

// -------------------------------------------------------------------------- //

FileResult
FileIO::Write(const String& string, u64& written) const
{
  return Write(
    reinterpret_cast<const u8*>(string.GetUTF8()), string.GetSize(), written);
}

// -------------------------------------------------------------------------- //

FileResult
FileIO::Write(const String& string) const
{
  u64 written;
  return Write(
    reinterpret_cast<const u8*>(string.GetUTF8()), string.GetSize(), written);
}

// -------------------------------------------------------------------------- //

FileResult
FileIO::Flush() const
{
  const BOOL success = FlushFileBuffers(mHandle);
  if (!success) {
    return FileIOErrorFromErrorWin32(GetLastError());
  }
  return FileResult::kSuccess;
}

// -------------------------------------------------------------------------- //

void
FileIO::Seek(u64 position) const
{
  LARGE_INTEGER seekOffset;
  seekOffset.QuadPart = static_cast<LONGLONG>(position);
  SetFilePointerEx(mHandle, seekOffset, nullptr, FILE_BEGIN);
}

// -------------------------------------------------------------------------- //

void
FileIO::SeekEnd() const
{
  LARGE_INTEGER seekOffset;
  seekOffset.QuadPart = 0ull;
  SetFilePointerEx(mHandle, seekOffset, nullptr, FILE_END);
}

// -------------------------------------------------------------------------- //

u64
FileIO::GetCursorPosition() const
{
  LARGE_INTEGER seekOffset;
  seekOffset.QuadPart = 0ull;
  return SetFilePointerEx(mHandle, seekOffset, nullptr, FILE_CURRENT);
}

}