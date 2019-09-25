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
#include "olivine/core/types.hpp"
#include "olivine/core/string.hpp"
#include "olivine/core/common.hpp"

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
    kResizable = Bit(1u)
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

  /** Whether app is running **/
  bool mRunning = false;

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
};

}