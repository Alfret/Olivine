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

#include "olivine/core/file/file_system.hpp"

// ========================================================================== //
// Functions
// ========================================================================== //

namespace olivine {

/** Retrieve win32 file attributes **/
static void
RetrieveFileAttributeData(const Path& path,
                          WIN32_FILE_ATTRIBUTE_DATA& attributes)
{
  char16* wpath = path.GetPathString().GetUTF16();
  GetFileAttributesExW(wpath, GetFileExInfoStandard, &attributes);
  delete[] wpath;
}

// -------------------------------------------------------------------------- //

static void
EnumerateHelper(const Path& path,
                const Path& basePath,
                ArrayList<Path>& paths,
                FileSystem::EnumerateFlag flags)
{
  // TODO(Filip Björklund): Support recursive

  // Enumerate and add files to list from directory
  if (FileSystem::GetType(path) == FileSystem::ObjectType::kDirectory) {
    String wildcardPath = path.GetPathString() + "/*";
    WIN32_FIND_DATAW findData;
    char16* wpath = wildcardPath.GetUTF16();
    const HANDLE findHandle = FindFirstFileExW(
      wpath, FindExInfoBasic, &findData, FindExSearchNameMatch, nullptr, 0);
    delete[] wpath;
    if (findHandle != INVALID_HANDLE_VALUE) {
      do {
        String filePath(findData.cFileName);
        if (filePath != "." && filePath != "..") {
          paths.AppendEmplace(filePath);
        }
      } while (FindNextFileW(findHandle, &findData));
    }
    FindClose(findHandle);
  }
}

// -------------------------------------------------------------------------- //

FileResult
FileErrorFromErrorWin32(DWORD error)
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

// -------------------------------------------------------------------------- //

static s64
LastOffsetOfWStringWin32(const wchar_t* str, wchar_t chr)
{
  s64 lastIndex = -1;
  for (size_t i = 0; i < wcslen(str); i++) {
    if (str[i] == chr) {
      lastIndex = i;
    }
  }
  return lastIndex;
}

// -------------------------------------------------------------------------- //

static DWORD
CreateDirectoryRecursivelyWin32(char16* path)
{
  // Try to create directory
  BOOL result = CreateDirectoryW(path, NULL);
  if (result == 0) {
    // Retrieve error
    const DWORD error = GetLastError();

    // Check if invalid path or other error
    if (error == ERROR_PATH_NOT_FOUND) {
      // Create sub directory
      // Support both '\' and '/' file separators
      const s64 lastIndex0 = LastOffsetOfWStringWin32(path, '\\');
      const s64 lastIndex1 = LastOffsetOfWStringWin32(path, '/');
      const s64 lastIndex = Max(lastIndex0, lastIndex1);

      char16 subPath[MAX_PATH];
      if (lastIndex < 0) {
        return ERROR_PATH_NOT_FOUND;
      }
      memcpy(subPath, path, sizeof(char16) * lastIndex);
      subPath[lastIndex] = 0;

      // Recursively create sub-path
      const DWORD recursiveResult = CreateDirectoryRecursivelyWin32(subPath);
      if (recursiveResult != ERROR_SUCCESS) {
        return recursiveResult;
      }

      // Create this directory
      result = CreateDirectoryW(path, NULL);
      return result == 0 ? GetLastError() : ERROR_SUCCESS;
    }

    // Return other error
    return error;
  }

  // Return success
  return ERROR_SUCCESS;
}

}

// ========================================================================== //
// FileSystem Implementation
// ========================================================================== //

namespace olivine {

FileResult
FileSystem::Create(const Path& path, ObjectType type, CreateFlag flags)
{
  Assert(type != ObjectType::kInvalid,
         "Cannot create file system object of type 'kInvalid'");

  bool overwrite = bool(flags & CreateFlag::kOverwrite);

  if (type == ObjectType::kFile) {
    char16* wpath = path.GetPathString().GetUTF16();
    HANDLE file = CreateFileW(wpath,
                              0,
                              0,
                              nullptr,
                              overwrite ? OPEN_ALWAYS : CREATE_NEW,
                              FILE_ATTRIBUTE_NORMAL,
                              nullptr);
    delete[] wpath;
    if (file == INVALID_HANDLE_VALUE) {
      return FileErrorFromErrorWin32(GetLastError());
    }
    CloseHandle(file);
    return FileResult::kSuccess;
  }
  if (type == ObjectType::kDirectory) {
    char16* wpath = path.GetPathString().GetUTF16();
    const DWORD result = CreateDirectoryRecursivelyWin32(wpath);
    delete[] wpath;
    return FileErrorFromErrorWin32(result);
  }

  // Unknown error
  return FileResult::kUnknownError;
}

// -------------------------------------------------------------------------- //

FileResult
FileSystem::Delete(const Path& path, DeleteFlag flags)
{
  // File does not exist
  if (!Exists(path)) {
    return FileResult::kNotFound;
  }

  // Currently directories can't be removed
  if (GetType(path) == ObjectType::kDirectory) {
    Assert(false, "Directories does not support being deleted yet");
    return FileResult::kInvalidArgument;
  }

  // Delete file
  if (GetType(path) == ObjectType::kFile) {
    char16* wpath = path.GetPathString().GetUTF16();
    const BOOL success = DeleteFileW(wpath);
    delete[] wpath;
    if (!success) {
      return FileErrorFromErrorWin32(GetLastError());
    }
    return FileResult::kSuccess;
  }

  // Unknown error
  return FileResult::kUnknownError;
}

// -------------------------------------------------------------------------- //

FileResult
FileSystem::Rename(const Path& path, const String& name)
{
  // TODO(Filip Björklund): Implement
  (void)name;
  return FileResult::kUnknownError;
}

// -------------------------------------------------------------------------- //

FileResult
FileSystem::Copy(const Path& from, const Path& to)
{
  // TODO(Filip Björklund): Implement
  (void)to;
  return FileResult::kUnknownError;
}

// -------------------------------------------------------------------------- //

FileResult
FileSystem::Move(const Path& from, const Path& to)
{
  // TODO(Filip Björklund): Implement
  (void)to;
  return FileResult::kUnknownError;
}

// -------------------------------------------------------------------------- //

bool
FileSystem::Exists(const Path& path)
{
  WIN32_FILE_ATTRIBUTE_DATA attributes;
  RetrieveFileAttributeData(path, attributes);
  return attributes.dwFileAttributes != INVALID_FILE_ATTRIBUTES;
}

// -------------------------------------------------------------------------- //

ArrayList<Path>
FileSystem::Enumerate(const Path& path, EnumerateFlag flags)
{
  // Assert preconditions
  Assert(GetType(path) == ObjectType::kDirectory,
         "Only directories can be enumerated");

  // List of paths
  ArrayList<Path> paths;

  // Add special directories if they should be included
  bool includeSpecial = bool(flags & EnumerateFlag::kIncludeSpecial);
  if (includeSpecial && GetType(path) == ObjectType::kDirectory) {
    paths.AppendEmplace(".");
    paths.AppendEmplace("..");
  }

  // Enumerate directory entries
  EnumerateHelper(path, Path{ "" }, paths, flags);

  // Return list of paths
  return paths;
}

// -------------------------------------------------------------------------- //

FileSystem::ObjectType
FileSystem::GetType(const Path& path)
{
  // Only existing files has a type
  if (!Exists(path)) {
    return ObjectType::kInvalid;
  }

  WIN32_FILE_ATTRIBUTE_DATA attributes;
  RetrieveFileAttributeData(path, attributes);
  if (attributes.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
    return ObjectType::kDirectory;
  }
  return ObjectType::kFile;
}

// -------------------------------------------------------------------------- //

u64
FileSystem::GetSize(const Path& path)
{
  WIN32_FILE_ATTRIBUTE_DATA attributes;
  RetrieveFileAttributeData(path, attributes);
  const u64 sizeHigh = (uint64_t)attributes.nFileSizeHigh << sizeof(DWORD);
  const u64 sizeLow = attributes.nFileSizeLow;
  return sizeLow | sizeHigh;
}
}