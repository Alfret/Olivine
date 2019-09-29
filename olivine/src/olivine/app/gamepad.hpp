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

// Thirdparty headers
#include "thirdparty/glfw/glfw3.h"

// ========================================================================== //
// GamepadButton Enumeration
// ========================================================================== //

namespace olivine {

/** \enum GamepadButton
 * \author Filip Björklund
 * \date 26 september 2019 - 14:38
 * \brief Gamepad buttons.
 * \details
 * Enumeration of gamepad buttons.
 */
enum class GamepadButton
{
  /* A & Cross button */
  kA = GLFW_GAMEPAD_BUTTON_A,
  /* B & Circle button */
  kB = GLFW_GAMEPAD_BUTTON_B,
  /* X & Square button */
  kX = GLFW_GAMEPAD_BUTTON_X,
  /* Y & Triangle button */
  kY = GLFW_GAMEPAD_BUTTON_Y,
  /* Left bumper button */
  kLeftBumper = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
  /* Right bumper button */
  kRightBumper = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
  /* Back button */
  kBack = GLFW_GAMEPAD_BUTTON_BACK,
  /* Start button */
  kStart = GLFW_GAMEPAD_BUTTON_START,
  /* Guide button */
  kGuide = GLFW_GAMEPAD_BUTTON_GUIDE,
  /* Left thumbstick button  */
  kLeftThumb = GLFW_GAMEPAD_BUTTON_LEFT_THUMB,
  /* Right thumbstick button  */
  kRightThumb = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,
  /* Directional pad up */
  kDPadUp = GLFW_GAMEPAD_BUTTON_DPAD_UP,
  /* Directional pad right */
  kDPadRight = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
  /* Directional pad down */
  kDPadDown = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
  /* Directional pad left */
  kDPadLeft = GLFW_GAMEPAD_BUTTON_DPAD_LEFT,
  /* A & Cross button */
  kCross = GLFW_GAMEPAD_BUTTON_CROSS,
  /* B & Circle button */
  kCircle = GLFW_GAMEPAD_BUTTON_CIRCLE,
  /* X & Square button */
  kSquare = GLFW_GAMEPAD_BUTTON_SQUARE,
  /* Y & Triangle button */
  kTriangle = GLFW_GAMEPAD_BUTTON_TRIANGLE,
};

}

// ========================================================================== //
// GamepadAxis Enumeration
// ========================================================================== //

namespace olivine {

/** \enum GamepadAxis
 * \author Filip Björklund
 * \date 26 september 2019 - 19:36
 * \brief Gamepad axes.
 * \details
 * Enumeration of gamepad axes.
 */
enum class GamepadAxis
{
  /* Left stick x-axis */
  kLeftX = GLFW_GAMEPAD_AXIS_LEFT_X,
  /* Left stick y-axis */
  kLeftY = GLFW_GAMEPAD_AXIS_LEFT_Y,
  /* Right stick x-axis */
  kRightX = GLFW_GAMEPAD_AXIS_RIGHT_X,
  /* Right stick y-axis */
  kRightY = GLFW_GAMEPAD_AXIS_RIGHT_Y,
  /* Left trigger */
  kLeftTrigger = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
  /* Left trigger */
  kRightTrigger = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,

};

}