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

#include "olivine/core/file/file.hpp"

// ========================================================================== //
// File Implementation
// ========================================================================== //

namespace olivine {

File::File(Path path)
  : mPath(std::move(path))
{}

// -------------------------------------------------------------------------- //

File::File(String path)
  : mPath(Path{ std::move(path) })
{}

// -------------------------------------------------------------------------- //

File
File::Open(const Path& path) const
{
  return File{ mPath.Joined(path) };
}

// -------------------------------------------------------------------------- //

File
File::Open(const String& path) const
{
  return File{ mPath.Joined(path) };
}

// -------------------------------------------------------------------------- //

File
File::Sibling(const String& path)
{
  return File{ mPath.GetDirectory().Join(path) };
}

// -------------------------------------------------------------------------- //

FileResult
File::Create(File::Type type, bool overwrite)
{
  FileSystem::CreateFlag flags = FileSystem::CreateFlag::kNone;
  if (overwrite) {
    flags |= FileSystem::CreateFlag::kOverwrite;
  }
  return FileSystem::Create(mPath, type, flags);
}

// -------------------------------------------------------------------------- //

FileResult
File::Delete(bool recursive)
{
  FileSystem::DeleteFlag flags = FileSystem::DeleteFlag::kNone;
  if (recursive) {
    flags |= FileSystem::DeleteFlag::kRecursive;
  }
  return FileSystem::Delete(mPath, flags);
}

// -------------------------------------------------------------------------- //

FileResult
File::Rename(const String& name)
{
  return FileSystem::Rename(mPath, name);
}

// -------------------------------------------------------------------------- //

FileResult
File::Copy(const Path& to)
{
  return FileSystem::Copy(mPath, to);
}

// -------------------------------------------------------------------------- //

FileResult
File::Move(const Path& to)
{
  return FileSystem::Move(mPath, to);
}

// -------------------------------------------------------------------------- //

bool
File::Exists() const
{
  return FileSystem::Exists(mPath);
}

// -------------------------------------------------------------------------- //

ArrayList<File>
File::Enumerate(bool includeSpecial) const
{
  // Enumerate files
  FileSystem::EnumerateFlag flags = FileSystem::EnumerateFlag::kNone;
  if (includeSpecial) {
    flags |= FileSystem::EnumerateFlag::kIncludeSpecial;
  }
  ArrayList<Path> paths = FileSystem::Enumerate(mPath);

  // Convert to 'andesite::File'
  ArrayList<File> files;
  for (Path& path : paths) {
    files.AppendEmplace(path);
  }
  return files;
}

// -------------------------------------------------------------------------- //

File::Type
File::GetType() const
{
  return FileSystem::GetType(mPath);
}

// -------------------------------------------------------------------------- //

u64
File::GetSize() const
{
  return FileSystem::GetSize(mPath);
}

// -------------------------------------------------------------------------- //

bool
operator==(const File& file0, const File& file1)
{
  return file0.mPath == file1.mPath;
}

// -------------------------------------------------------------------------- //

bool
operator!=(const File& file0, const File& file1)
{
  return file0.mPath != file1.mPath;
}

}