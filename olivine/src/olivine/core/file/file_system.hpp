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
// Filesystem Declaration
// ========================================================================== //

namespace olivine {

/** \class FileSystem
 * \author Filip Björklund
 * \date 26 july 2019 - 11:33
 * \brief File system utilities.
 * \details
 * Class that contains function for working with the file system.
 */
class FileSystem
{
  OL_NAMESPACE_CLASS(FileSystem);

public:
  /** Enumeration of file system object types **/
  enum class ObjectType : u8
  {
    /** Invalid. This is the type of non-existing objects **/
    kInvalid,
    /** Standard file  **/
    kFile,
    /** Directory **/
    kDirectory
  };
  OL_ENUM_CLASS_OPERATORS(friend, ObjectType, u8);

  /** Creation flags **/
  enum class CreateFlag : u8
  {
    /** No specific flags **/
    kNone = 0,
    /** Overwrite existing object **/
    kOverwrite = Bit(0)
  };
  OL_ENUM_CLASS_OPERATORS(friend, CreateFlag, u8);

  /** Deletion flags **/
  enum class DeleteFlag : u8
  {
    /** No specific flags **/
    kNone = 0,
    /** Delete objects recursively. This means that if the object is a directory
     * then everything in the directory is also deleted. Directories with other
     * objects in them cannot be deleted without specifying this flag **/
    kRecursive = Bit(0)
  };
  OL_ENUM_CLASS_OPERATORS(friend, DeleteFlag, u8);

  /** Enumeration flags **/
  enum class EnumerateFlag : u8
  {
    /** No specific flags **/
    kNone = 0,
    /** Flag that specifies if the any subdirectories should also be
     * enumerated **/
    kRecursive = Bit(0),
    /** Flag that specifies that special paths, such as '.' and '..', should be
     * included in the enumerated files list **/
    kIncludeSpecial = Bit(1),
  };
  OL_ENUM_CLASS_OPERATORS(friend, EnumerateFlag, u8);

public:
  /** Create an object in the file system. The path to the object and the type
   * is specified. A flag can be passed to determine how the object will be
   * created.
   * \brief Create object.
   * \param path Path to object.
   * \param type Type of file system object to create.
   * \param flags Flags for creation.
   * \return Result.
   */
  static FileResult Create(const Path& path,
                           ObjectType type,
                           CreateFlag flags = CreateFlag::kNone);

  /** Delete an object in the file system pointer to by the specified path.
   * \brief Delete object.
   * \param path Path to object.
   * \param recursive Whether to recursively delete subdirectories.
   * \return Result.
   */
  static FileResult Delete(const Path& path,
                           DeleteFlag flags = DeleteFlag::kNone);

  /** Rename an object in the file system pointed to by the specified path.
   * \note Only the name of the object itself and not the path is changed.
   * \brief Rename object.
   * \param path Path to object.
   * \param name Name to rename to.
   * \return Result.
   */
  static FileResult Rename(const Path& path, const String& name);

  /** Copy an object in the filesystem to another path.
   * \brief Copy object.
   * \param from Source of copy.
   * \param to Destination of copy.
   * \return Result.
   */
  static FileResult Copy(const Path& from, const Path& to);

  /** Move an object in the filesystem to another path.
   * \brief Move object.
   * \param from Source of move.
   * \param to Destination of move.
   * \return Result.
   */
  static FileResult Move(const Path& from, const Path& to);

  /** Returns whether or not an object exists in the file system at the
   * specified path.
   * \brief Returns whether object exists.
   * \return True if an object exists at the path otherwise false.
   */
  OL_NODISCARD static bool Exists(const Path& path);

  /** Enumerate all the files in the directory pointed to by the specified path.
   * \pre Object pointed to by path must be a directory.
   * \brief Enumerate files in directory.
   * \param path Path to the directory to enumerate.
   * \param flags Flags for enumerating.
   * \return List of file paths.
   */
  OL_NODISCARD static ArrayList<Path> Enumerate(
    const Path& path,
    EnumerateFlag flags = EnumerateFlag::kNone);

  /** Returns the type of an object in the file system pointed to by the
   * specified path.
   * \brief Returns object type.
   * \param path Path to object.
   * \return Type of the object.
   */
  OL_NODISCARD static ObjectType GetType(const Path& path);

  /** Returns the size of an object in the filesystem pointed to by the
   * specified path.
   * \note For files this is the size of the file itself.
   * \note For directories this is the combined size of all objects in the
   * directory.
   * \return Object size
   */
  OL_NODISCARD static u64 GetSize(const Path& path);
};

}