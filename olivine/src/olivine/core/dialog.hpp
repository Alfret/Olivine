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
// Header
// ========================================================================== //

// Project headers
#include "olivine/core/types.hpp"
#include "olivine/core/string.hpp"

// ========================================================================== //
// Dialog Declaration
// ========================================================================== //

namespace olivine {

/** \class Dialog
 * \author Filip Björklund
 * \date 26 september 2019 - 11:58
 * \brief Dialog utilities.
 * \details
 * Utility class with static functions for showing dialog windows.
 */
class Dialog
{
  OL_NAMESPACE_CLASS(Dialog);

public:
  /** Show an error dialog with the specified title and message.
   * \brief Show error dialog.
   * \param title Dialog title.
   * \param message Message to show in dialog.
   */
  static void ShowError(const String& title, const String& message);
};

}