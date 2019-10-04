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
#include "olivine/app/key.hpp"
#include "olivine/app/gamepad.hpp"
#include "olivine/core/types.hpp"
#include "olivine/core/string.hpp"
#include "olivine/core/common.hpp"
#include "olivine/core/platform/headers.hpp"

// Thirdparty headers
#include "thirdparty/glfw/glfw3.h"

// ========================================================================== //
// App Declaration
// ========================================================================== //

namespace olivine {

OL_FORWARD_DECLARE(Context);
OL_FORWARD_DECLARE(Device);
OL_FORWARD_DECLARE(CommandQueue);
OL_FORWARD_DECLARE(SwapChain);
OL_FORWARD_DECLARE(Viewport);
OL_FORWARD_DECLARE(Rectangle);

/** \class App
 * \author Filip Björklund
 * \date 25 september 2019 - 12:18
 * \brief Application.
 * \details
 * Represents the main application.
 */
class App
{
public:
  /* Application flags */
  enum class Flag : u32
  {
    /* No specific flags */
    kNone = 0,
    /* Specifies that the application should exit when the user presses the
     * escape key */
    kExitOnEscape = Bit(0u),
    /* Specifies that the window should be resizable */
    kResizable = Bit(1u),
    /* Specifies that vertical blank synchronization should be enabled */
    kVerticalSync = Bit(2u)
  };
  OL_ENUM_CLASS_OPERATORS(friend, Flag, u32);

  /* Creation information */
  struct CreateInfo
  {
    /** App title **/
    String title = "Untitled";
    /** Window settings **/
    struct
    {
      /** Width **/
      u32 width = 400;
      /** Height **/
      u32 height = 400;
    } window;

    /* Target updates per second of the application. This will determine how
     * often the fixed update is called. It will however have no effect on the
     * standard 'Update' and 'Render' functions */
    u64 ups = 60;

    /* Key for toggling fullscreen. Default to 'invalid' which means that the
     * toggle feature is disabled and must be handled manually by the user */
    Key toggleFullscreenKey = Key::kInvalid;

    /* Creation flags */
    Flag flags = Flag::kNone;
  };

  /* Video mode */
  struct VideoMode
  {
    /* Width */
    u32 width;
    /* Height */
    u32 height;
    /* Refresh rate. Set to 0 to use the default */
    u32 refresh = 0;
  };

private:
  /* Global app instance */
  static App* sInstance;

private:
  /* App title */
  String mTitle;

  /* Flags specified on creation */
  Flag mFlags;

  /* Updates per second */
  u64 mUPS;

  /* Key for toggling fullscreen */
  Key mKeyToggleFullscreen;

  /* Render context */
  Context* mContext;
  /* Device  */
  Device* mDevice;
  /* Graphics command queue */
  CommandQueue* mGraphicsQueue;
  /* Compute command queue */
  CommandQueue* mComputeQueue;
  /* Copy command queue */
  CommandQueue* mCopyQueue;
  /* Swap chain */
  SwapChain* mSwapChain;

  /* Window */
  struct
  {
    /* GLFW handle */
    GLFWwindow* handle;
    /* Title */
    String title;
    /* Width */
    u32 width;
    /* Height */
    u32 height;
    /* X position */
    s32 posX;
    /* X position */
    s32 posY;

    /** Whether window is fullscreen **/
    bool isFullscreen = false;
    /* Data from before fullscreen. Only valid if fullscreen */
    struct
    {
      /* Width */
      u32 width;
      /* Height */
      u32 height;
      /* Pos X */
      s32 posX;
      /* Pos Y */
      s32 posY;
    } old;
  } mWindow;

  /* Whether app is running */
  bool mRunning = false;
  /* Whether cursor is grabbed */
  bool mCursorGrabbed = false;

public:
  /** Create an application object from a creation information structure.
   * \brief Create app.
   * \param createInfo Creation information.
   */
  explicit App(const CreateInfo& createInfo);

  /** Destruct the application.
   * \brief Destruct app.
   */
  virtual ~App();

  /** Run the main application loop.
   * \brief Run app.
   */
  void Run();

  /** Exit application. This will stop application loop if it's running.
   * \brief Exit app.
   */
  void Exit();

  /** Show the application window.
   * \brief Show app window.
   */
  void Show();

  /** Hide the application window.
   * \brief Hide app window.
   */
  void Hide();

  /** Enter fullscreen mode with the specified video mode.
   * \note If the video mode is different than the current of the window then
   * the resolution of the will change to accomodate this.
   * \note If the 'videoMode' parameter is NULL then the default video mode of
   * the monitor will be used.
   * \brief Enter fullscreen.
   * \param videoMode Video mode to enter fullscreen or NULL to use the video
   * mode of the monitor.
   */
  void EnterFullscreen(VideoMode* videoMode = nullptr);

  /** Exit fullscreen and restore the old window position and dimensions.
   * \brief Exit fullscreen.
   */
  void ExitFullscreen();

  /** Toggle between fullscreen and windowed mode.
   * \note This will use the 'NULL' video mode when entering fullscreen.
   * \brief Toggle fullscreen.
   */
  void ToggleFullscreen();

  /** Called each frame to update the application.
   * \brief Update app.
   * \param delta Delta time.
   */
  virtual void Update(f64 delta) {}

  /** Called at a fixed rate to update the application.
   * \brief Update app.
   */
  virtual void FixedUpdate() {}

  /** Called each frame to render the application.
   * \brief Render app.
   */
  virtual void Render() = 0;

  /** Called when a key has been pressed.
   * \brief Called on key presses.
   * \param key Key that was pressed.
   * \param repeat Whether the press was a repeat press.
   */
  virtual void OnKeyPress(Key key, bool repeat) {}

  /** Called when a key has been released.
   * \brief Called on key releases.
   * \param key Key that was released.
   */
  virtual void OnKeyRelease(Key key) {}

  /** Called when the mouse cursor has moved. The position is specified from the
   * top-left corner of the window.
   * \brief Called on mouse motion.
   * \param x X position.
   * \param y Y position.
   */
  virtual void OnMouseMove(f64 x, f64 y) {}

  /** Called when the mouse scroll wheel has scrolled (or trackpad gesture).
   * \brief Most mice only supports scrolling in the y axis.
   * \brief Called on mouse scroll.
   * \param deltaX Movement in x axis
   * \param deltaY Movement in y axis
   */
  virtual void OnMouseScroll(f64 deltaX, f64 deltaY) {}

  /** Returns whether or not a gamepad is connected with the specified index
   * in the gamepad list.
   * \brief Returns whether gamepad is connected.
   * \param index Index of the gamepad.
   * \return True if the gamepad is connected otherwise false.
   */
  bool IsGamepadConnected(u32 index = 0) const;

  /** Returns whether or not the specified gamepad button is pressed down on the
   * gamepad with the specified index.
   * \brief Returns whether gamepad button is pressed.
   * \param button Button to check if down.
   * \param index Index of the gamepad to check the button for.
   * \return True if the gamepad button is down otherwise false.
   */
  bool IsGamepadButtonDown(GamepadButton button, u32 index = 0) const;

  /** Returns the value of the specified gamepad axis for the gamepad at the
   * specified index.
   * \brief Returns gamepad axis value.
   * \param axis Axis to get value for.
   * \param index Index of the gamepad to get value for.
   * \return Axis value on the range [-1.0, 1.0]
   */
  f32 GetGamepadAxis(GamepadAxis axis, u32 index = 0) const;

  /** Called when the application window has been resized.
   * \note The width and height describe the client area of the window.
   * \brief Called on window resize.
   * \param width Width of resized window.
   * \param height Height of resized window.
   */
  virtual void OnResize(u32 width, u32 height) {}

  /** Returns the render context of the application.
   * \brief Returns render context
   */
  Context* GetContext() const { return mContext; }

  /** Returns the graphics device of the application.
   * \brief Returns graphics device
   */
  Device* GetDevice() const { return mDevice; }

  /** Returns the graphics command queue.
   * \brief Return graphics queue.
   * \return Graphics command queue.
   */
  CommandQueue* GetGraphicsQueue() const { return mGraphicsQueue; }

  /** Returns the compute command queue.
   * \brief Return compute queue.
   * \return Compute command queue.
   */
  CommandQueue* GetComputeQueue() const { return mComputeQueue; }

  /** Returns the copy command queue.
   * \brief Return copy queue.
   * \return Copy command queue.
   */
  CommandQueue* GetCopyQueue() const { return mCopyQueue; }

  /** Returns the swap chain of the application window.
   * \brief Returns swap chain.
   * \return Swap chain.
   */
  SwapChain* GetSwapChain() const { return mSwapChain; }

  /** Enabled cursor grabbing. This will keep the cursor inside the window and
   * thus allow moving it infinitely. Mouse motion are still reported as normal.
   * \brief Enable cursor grab.
   */
  void EnableGrabCursor();

  /** Disable cursor grabbing.
   * \brief Disable cursor grab.
   */
  void DisableGrabCursor();

  /** Toggle cursor grabbing. When the cursor is grabbed it can be moved
   * infinitely without exiting the window.
   * \brief Toggle cursor grab.
   */
  void ToggleGrabCursor();

  /** Flush all the command queues that are owned by the application (default
   * queues).
   * \brief Flush queues.
   */
  void FlushQueues();

  /** Returns the width of the application window.
   * \brief Returns window width.
   * \return Window width.
   */
  u32 GetWidth() const { return mWindow.width; }

  /** Returns the height of the application window.
   * \brief Returns window height.
   * \return Window height.
   */
  u32 GetHeight() const { return mWindow.height; }

  /** Returns a viewport that represents the entire window.
   * \brief Returns full viewport.
   * \return Viewport.
   */
  Viewport EntireViewport() const;

  /** Returns a rectangle that represents the entire window.
   * \note This exists to make setting the scissor rectangle easier.
   * \brief Returns full rectangle.
   * \return Rectangle.
   */
  Rectangle EntireRectangle() const;

  /** Returns the current title of the application window.
   * \brief Returns app window title.
   * \return Window title.
   */
  const String& GetWindowTitle() const { return mWindow.title; }

  /** Set the title of the application window.
   *\note This is not necesarily the same as the title of the application
   * (only if never set after creation).
   * \brief Set app window title.
   * \param title Title to set.
   */
  void SetWindowTitle(const String& title);

private:
  /** Center the app window **/
  void CenterWindow();

public:
  /** Returns the global application instance.
   * \brief Returns global app instance.
   * \return Global app intance.
   */
  static App* Instance() { return sInstance; }

private:
  /** Returns the GLFWmonitor that a GLFWwindow is positioned on **/
  static GLFWmonitor* MonitorForWindowGLFW(GLFWwindow* window);

  /** GLFW error callback **/
  static void ErrorCallbackGLFW(int error, const char8* description);

  /** GLFW window resize callback **/
  static void WindowResizeCallbackGLFW(GLFWwindow* window,
                                       int width,
                                       int height);

  /** GLFW window move callback **/
  static void WindowMoveCallbackGLFW(GLFWwindow* window, int xpos, int ypos);

  /** GLFW key callback **/
  static void KeyCallbackGLFW(GLFWwindow* window,
                              int key,
                              int scancode,
                              int action,
                              int mods);

  /** GLFW mouse move callback **/
  static void MouseMotionCallbackGLFW(GLFWwindow* window, f64 xpos, f64 ypos);

  /** GLFW mouse scroll callback **/
  static void MouseScrollCallbackGLFW(GLFWwindow* window, double x, double y);
};

}