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
#include "olivine/core/collection/array_list.hpp"
#include "olivine/core/platform/headers.hpp"
#include "olivine/core/macros.hpp"
#include "olivine/core/string.hpp"

// ========================================================================== //
// Path Declaration
// ========================================================================== //

namespace olivine {

/** \class Path
 * \author Filip Björklund
 * \date 19 july 2019 - 19:29
 * \brief Path.
 * \details
 * Represents a path in the filesystem.
 */
class Path
{
public:
  /**
   * List of known file extensions. The function Path::GetExtensionString() can
   * be used to determine the string of the extension if it's not part of the
   * enumeration.
   *
   * Any values added to this enumeration must also be added to the check in the
   * function Path::GetExtension(). So that they are correctly determined from
   * their strings.
   */
  enum class Extension
  {
    /** No extension **/
    kNone,
    /** Unknown extension **/
    kUnknown,

    /** Temporary file **/
    kTmp,

    /** Txt (text) data file **/
    kTxt,
    /** CSV (comma separated values) data file **/
    kCsv,
    /** Dat (data) data file **/
    kDat,
    /** JSON (javascript object notation) data file **/
    kJson,
    /** XML (extensible markup language) data file **/
    kXml,
    /** Yaml (YAML Ain't Markup Language) data file **/
    kYaml,
    /** Toml (Tom's obvious, minimal language) data file **/
    kToml,
    /** md (Markdown) data file **/
    kMd,
    /** cfg (configuration) data file **/
    kCfg,
    /** ini (initialization) data file **/
    kIni,
    /** log (Log file) data file **/
    kLog,

    /** PNG (portable network graphics) image file **/
    kPng,
    /** TGA (targa) image file **/
    kTga,
    /** JPG (joint photographic expert group) image file **/
    kJpeg,
    /** PSD (photoshop document) image file **/
    kPsd,
    /** bmp (Bitmap) image file **/
    kBmp,
    /** gif (graphics interchange format) image file **/
    kGif,
    /** ico (icon) image file **/
    kIco,
    /** svg (scalable vector graphics) image file **/
    kSvg,
    /** tiff (tagged image file format) image file **/
    kTiff,

    /** Ogg (ogg vorbis) audio file **/
    kOgg,
    /** WAV (waveform) audio file **/
    kWav,
    /** MP3 audio file **/
    kMp3,

    /** Obj model file **/
    kObj,
    /** Gltf model/scene file **/
    kGltf,

    /** AVI (audio video interleaved) video file **/
    kAvi,
    /** MP4 video file **/
    kMp4,

    /** Tar (tarball) archive **/
    kTar,
    /** Zip (zip) archive **/
    kZip,
    /** Gz (gzip) archive **/
    kGz,
    /** 7z (7-zip) archive **/
    k7z,

    /** TTF (truetype font) font file **/
    kTtf,
    /** OTF (open type font) font file **/
    kOtf,

    /** C source file **/
    kC,
    /** C header file **/
    kH,
    /** C++ source file **/
    kCpp,
    /** C++ header file **/
    kHpp,
    /** Python source file **/
    kPy,
    /** JavaScript source file **/
    kJs,
    /** Java source file **/
    kJava,
    /** Rust source file **/
    kRs,

    /** HLSL shader file **/
    kHLSL,
    /** GLSL shader file **/
    kGLSL,
    /** Metal shader file **/
    kMetal,

    /** exe (Windows executable) executable file **/
    kExe,
    /** app (MacOS application) executable file **/
    kApp,
    /** apk (Android package) executable file **/
    kApk,
    /** dll (dynamic link library) shared library **/
    kDll,
    /** so (shared object) shared library **/
    kSo,
    /** dynlib (dynamic library) shared library **/
    kDynlib,
    /** lib (library) static library **/
    kLib,
    /** a (library) static library **/
    kA
  };

public:
  /** Path (relative) to current directory **/
  static constexpr char8 CURRENT[] = ".";
  /** Path (relative) to parent directory **/
  static constexpr char8 PARENT[] = "..";

  /** Path separator (Windows) **/
  static constexpr char8 SEPARATOR[] = "\\";
  /** Path separator character (Windows) **/
  static constexpr u32 SEPARATOR_CHAR = '\\';

private:
  /** Path string **/
  String mPath;

public:
  /** Construct a path from a string.
   * \brief Construct path.
   * \param path
   */
  Path(String path = "");

  /** Join another path at the end of this path. This correctly insert a
   * separator between the path components.
   * \brief Join with another path.
   * \param other Path to join.
   * \return Reference to this.
   */
  Path& Join(const Path& other);

  /** Join another path at the end of this path. This correctly insert a
   * separator between the path components.
   * \brief Join with another path.
   * \param other Path to join.
   * \return Reference to this.
   */
  Path& operator+=(const Path& other);

  /** Returns the join of this path with another path. This correctly insert a
   * separator between the path components.
   * \brief Returns joined path.
   * \param other Path to join with.
   * \return Joined paths.
   */
  OL_NODISCARD Path Joined(const Path& other) const;

  /** Returns the string that represents the path.
   * \brief Returns path string.
   * \return Path string.
   */
  OL_NODISCARD const String& GetPathString() const { return mPath; }

  /** Returns the string that represents the path.
   * \brief Returns path string.
   * \return Path string.
   */
  OL_NODISCARD const char8* GetPathStringUTF8() const
  {
    return mPath.GetUTF8();
  }

  /** Returns the path as an absolute path. This function also resolves any '.'
   * (current directory) and '..' (parent directory) components that may be part
   * of the path. For this reason this function may be costly and should not be
   * called when the absolute path is not actually needed.
   * \brief Returns absolute resolved path.
   * \return Absolute and resolved path.
   */
  OL_NODISCARD Path GetAbsolutePath() const;

  /** Returns a canonicalized (normalized) version of the path.
   * \brief Returns canonicalized path.
   * \return Canonicalized path.
   */
  OL_NODISCARD Path GetCanonical() const;

  /** Returns the path to the directory containing the object at this path.
   * \brief Returns containing directory.
   * \return Containing directory.
   *
   * \example
   * Path{ "path/to/file.txt" }.GetDirectory(); // 'path/to'
   * Path{ "file.txt" }.GetDirectory(); // ''
   */
  OL_NODISCARD Path GetDirectory() const;

  /** Returns each of the path components that make up the path.
   * \brief Returns path components.
   * \return Path components.
   */
  OL_NODISCARD ArrayList<String> GetComponents() const;

  /** Returns the name of the object at the path. This includes the base name
   * and the extension. This works similar to Path::GetBaseName(), however it
   * does include the extension.
   * \brief Returns name.
   * \return Name.
   *
   * \example
   * auto name = Path{ "path/to/file.txt" }.GetName(); // 'file.txt'
   * auto name = Path{ "path/to/file" }.GetName(); // 'file'
   */
  OL_NODISCARD String GetName() const;

  /** Returns the base name of the object at the path. This works similar to
   * Path::GetName(), however without including the extension.
   * \brief Returns base name.
   * \return Base name.
   *
   * \example
   * auto name = Path{ "path/to/file.txt" }.GetName(); // 'file'
   * auto name = Path{ "path/to/file" }.GetName(); // 'file'
   */
  OL_NODISCARD String GetBaseName() const;

  /** Returns the extension of the path.
   * \brief Returns extension.
   * \return Extension.
   *
   * \example
   * auto name = Path{ "path/to/file.txt" }.GetName(); // Extension::kTxt
   * auto name = Path{ "path/to/file.tar" }.GetName(); // Extension::kTar
   * auto name = Path{ "path/to/file.custom" }.GetName(); // Extension::kUnknown
   * auto name = Path{ "path/to/file" }.GetName(); // Extension::kNone
   */
  OL_NODISCARD Extension GetExtension() const;

  /** Returns the extension of the path as a string.
   * \brief Returns extension string.
   * \return Extension string.
   */
  OL_NODISCARD String GetExtensionString() const;

  /** Output stream function **/
  friend std::ostream& operator<<(std::ostream& stream, const Path& path);

  /** Returns whether or not two paths are equal.
   * \brief Returns whether paths are equal.
   * \param path0 First path.
   * \param path1 Second path.
   * \return True if the paths are equal otherwise false.
   */
  friend bool operator==(const Path& path0, const Path& path1);

  /** Returns whether or not two paths are not equal.
   * \brief Returns whether paths are unequal.
   * \param path0 First path.
   * \param path1 Second path.
   * \return True if the paths are not equal otherwise false.
   */
  friend bool operator!=(const Path& path0, const Path& path1);

  /** Returns two paths joined together.
   * \brief Returns joined paths.
   * \param path0 First path.
   * \param path1 Second path.
   * \return Joined paths.
   */
  friend Path operator+(const Path& path0, const Path& path1);

  /** Returns two paths joined together, the second created from a string.
   * \brief Returns joined paths.
   * \param path0 First path.
   * \param path1 Second path.
   * \return Joined paths.
   */
  friend Path operator+(const Path& path0, const String& path1);
};

}