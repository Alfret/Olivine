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
#include "olivine/core/platform/headers.hpp"

// Library headers
#include <thirdparty/glfw/glfw3.h>

// ========================================================================== //
// Key Enumeration
// ========================================================================== //

namespace olivine {

/** \enum Key
 * \author Filip Björklund
 * \date 26 september 2019 - 14:19
 * \brief Keys.
 * \details
 * Enumeration of keys that are common to most keyboards. Some keys may not be
 * included in the enumeration and will be reported as 'unknown'.
 */
enum class Key
{
  /* Invalid key (will never be parameter to callback) */
  kInvalid = GLFW_KEY_UNKNOWN - 1,
  /* Unknown key  */
  kUnknown = GLFW_KEY_UNKNOWN,
  /* Space key */
  kSpace = GLFW_KEY_SPACE,
  kApostrophe = GLFW_KEY_APOSTROPHE,
  kComma = GLFW_KEY_COMMA,
  kMinus = GLFW_KEY_MINUS,
  kPeriod = GLFW_KEY_PERIOD,
  kSlash = GLFW_KEY_SLASH,
  k0 = GLFW_KEY_0,
  k1 = GLFW_KEY_1,
  k2 = GLFW_KEY_2,
  k3 = GLFW_KEY_3,
  k4 = GLFW_KEY_4,
  k5 = GLFW_KEY_5,
  k6 = GLFW_KEY_6,
  k7 = GLFW_KEY_7,
  k8 = GLFW_KEY_8,
  k9 = GLFW_KEY_9,
  kSemicolon = GLFW_KEY_SEMICOLON,
  kEqual = GLFW_KEY_EQUAL,
  kA = GLFW_KEY_A,
  kB = GLFW_KEY_B,
  kC = GLFW_KEY_C,
  kD = GLFW_KEY_D,
  kE = GLFW_KEY_E,
  kF = GLFW_KEY_F,
  kG = GLFW_KEY_G,
  kH = GLFW_KEY_H,
  kI = GLFW_KEY_I,
  kJ = GLFW_KEY_J,
  kK = GLFW_KEY_K,
  kL = GLFW_KEY_L,
  kM = GLFW_KEY_M,
  kN = GLFW_KEY_N,
  kO = GLFW_KEY_O,
  kP = GLFW_KEY_P,
  kQ = GLFW_KEY_Q,
  kR = GLFW_KEY_R,
  kS = GLFW_KEY_S,
  kT = GLFW_KEY_T,
  kU = GLFW_KEY_U,
  kV = GLFW_KEY_V,
  kW = GLFW_KEY_W,
  kX = GLFW_KEY_X,
  kY = GLFW_KEY_Y,
  kZ = GLFW_KEY_Z,
  kLeftBracket = GLFW_KEY_LEFT_BRACKET,
  kBackslash = GLFW_KEY_BACKSLASH,
  kRightBracket = GLFW_KEY_RIGHT_BRACKET,
  kGraveAccent = GLFW_KEY_GRAVE_ACCENT,
  kWorld1 = GLFW_KEY_WORLD_1,
  kWorld2 = GLFW_KEY_WORLD_2,
  kEscape = GLFW_KEY_ESCAPE,
  kEnter = GLFW_KEY_ENTER,
  kTab = GLFW_KEY_TAB,
  kBackspace = GLFW_KEY_BACKSPACE,
  kInsert = GLFW_KEY_INSERT,
  kDelete = GLFW_KEY_DELETE,
  kRight = GLFW_KEY_RIGHT,
  kLeft = GLFW_KEY_LEFT,
  kDown = GLFW_KEY_DOWN,
  kUp = GLFW_KEY_UP,
  kPageUp = GLFW_KEY_PAGE_UP,
  kPageDown = GLFW_KEY_PAGE_DOWN,
  kHome = GLFW_KEY_HOME,
  kEnd = GLFW_KEY_END,
  kCapsLock = GLFW_KEY_CAPS_LOCK,
  kScrollLock = GLFW_KEY_SCROLL_LOCK,
  kNumLock = GLFW_KEY_NUM_LOCK,
  kPrintScreen = GLFW_KEY_PRINT_SCREEN,
  kPause = GLFW_KEY_PAUSE,
  kF1 = GLFW_KEY_F1,
  kF2 = GLFW_KEY_F2,
  kF3 = GLFW_KEY_F3,
  kF4 = GLFW_KEY_F4,
  kF5 = GLFW_KEY_F5,
  kF6 = GLFW_KEY_F6,
  kF7 = GLFW_KEY_F7,
  kF8 = GLFW_KEY_F8,
  kF9 = GLFW_KEY_F9,
  kF10 = GLFW_KEY_F10,
  kF11 = GLFW_KEY_F11,
  kF12 = GLFW_KEY_F12,
  kF13 = GLFW_KEY_F13,
  kF14 = GLFW_KEY_F14,
  kF15 = GLFW_KEY_F15,
  kF16 = GLFW_KEY_F16,
  kF17 = GLFW_KEY_F17,
  kF18 = GLFW_KEY_F18,
  kF19 = GLFW_KEY_F19,
  kF20 = GLFW_KEY_F20,
  kF21 = GLFW_KEY_F21,
  kF22 = GLFW_KEY_F22,
  kF23 = GLFW_KEY_F23,
  kF24 = GLFW_KEY_F24,
  kF25 = GLFW_KEY_F25,
  kKeypad0 = GLFW_KEY_KP_0,
  kKeypad1 = GLFW_KEY_KP_1,
  kKeypad2 = GLFW_KEY_KP_2,
  kKeypad3 = GLFW_KEY_KP_3,
  kKeypad4 = GLFW_KEY_KP_4,
  kKeypad5 = GLFW_KEY_KP_5,
  kKeypad6 = GLFW_KEY_KP_6,
  kKeypad7 = GLFW_KEY_KP_7,
  kKeypad8 = GLFW_KEY_KP_8,
  kKeypad9 = GLFW_KEY_KP_9,
  kKeypadDecimal = GLFW_KEY_KP_DECIMAL,
  kKeypadDivide = GLFW_KEY_KP_DIVIDE,
  kKeypadMultiply = GLFW_KEY_KP_MULTIPLY,
  kKeypadSubtract = GLFW_KEY_KP_SUBTRACT,
  kKeypadAdd = GLFW_KEY_KP_ADD,
  kKeypadEnter = GLFW_KEY_KP_ENTER,
  kKeypadEqual = GLFW_KEY_KP_EQUAL,
  kShiftLeft = GLFW_KEY_LEFT_SHIFT,
  kCtrlLeft = GLFW_KEY_LEFT_CONTROL,
  kAltLeft = GLFW_KEY_LEFT_ALT,
  kSuperLeft = GLFW_KEY_LEFT_SUPER,
  kShiftRight = GLFW_KEY_RIGHT_SHIFT,
  kCtrlRight = GLFW_KEY_RIGHT_CONTROL,
  kAltRight = GLFW_KEY_RIGHT_ALT,
  kSuperRight = GLFW_KEY_RIGHT_SUPER,
  kMenu = GLFW_KEY_MENU
};

}