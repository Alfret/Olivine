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

#include "olivine/app/app.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/core/assert.hpp"
#include "olivine/core/console.hpp"
#include "olivine/render/api/context.hpp"
#include "olivine/render/api/device.hpp"
#include "olivine/render/api/queue.hpp"
#include "olivine/render/api/swap_chain.hpp"

// Thirdparty headers
#define GLFW_EXPOSE_NATIVE_WIN32
#include "thirdparty/glfw/glfw3native.h"

// ========================================================================== //
// App Implementation
// ========================================================================== //

namespace olivine {

App* App::sInstance = nullptr;

// -------------------------------------------------------------------------- //

App::App(const CreateInfo& createInfo)
  : mTitle(createInfo.title)
  , mFlags(createInfo.flags)
  , mWindow{ nullptr, createInfo.window.width, createInfo.window.height }
{
  Assert(sInstance == nullptr, "Only one application can exist at one time");
  sInstance = this;

  // Init GLFW
  const int success = glfwInit();
  Assert(success, "Failed to initialize GLFW");
  glfwSetErrorCallback(ErrorCallbackGLFW);

  // Create render context
  const Context::CreateInfo contextInfo{};
  mContext = new Context(contextInfo);

  // Create device
  const Device::CreateInfo deviceInfo{};
  mDevice = new Device(deviceInfo);

  // Create command queues
  mGraphicsQueue = new CommandQueue(CommandQueue::Type::kGraphics);
  mGraphicsQueue->SetName("GraphicsQueue");
  mComputeQueue = new CommandQueue(CommandQueue::Type::kCompute);
  mComputeQueue->SetName("ComputeQueue");
  mCopyQueue = new CommandQueue(CommandQueue::Type::kCopy);
  mCopyQueue->SetName("CopyQueue");

  // Create window
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  glfwWindowHint(GLFW_RESIZABLE,
                 bool(createInfo.flags & Flag::kResizable) ? GLFW_TRUE
                                                           : GLFW_FALSE);
  mWindow.handle = glfwCreateWindow(
    mWindow.width, mWindow.height, mTitle.GetUTF8(), nullptr, nullptr);
  Assert(mWindow.handle != nullptr, "Failed to create app window");
  glfwSetWindowUserPointer(mWindow.handle, this);
  glfwSetFramebufferSizeCallback(mWindow.handle, WindowResizeCallbackGLFW);
  glfwSetWindowPosCallback(mWindow.handle, WindowMoveCallbackGLFW);
  glfwSetKeyCallback(mWindow.handle, KeyCallbackGLFW);
  glfwSetCursorPosCallback(mWindow.handle, MouseMotionCallbackGLFW);

  // Create swap chain
  SwapChain::CreateInfo swapChainInfo{};
  swapChainInfo.window = glfwGetWin32Window(mWindow.handle);
  swapChainInfo.bufferWidth = mWindow.width;
  swapChainInfo.bufferHeight = mWindow.height;
  swapChainInfo.format = Format::kB8G8R8A8Unorm;
  swapChainInfo.presentQueue = mGraphicsQueue;
  mSwapChain = new SwapChain(swapChainInfo);
  Assert(mSwapChain != nullptr, "Failed to create swap chain");
  mSwapChain->SetName("SwapChain");
}

// -------------------------------------------------------------------------- //

App::~App()
{
  // Flush all queues
  mGraphicsQueue->Flush();
  mComputeQueue->Flush();
  mCopyQueue->Flush();

  // Delete swap chain
  delete mSwapChain;

  // Destroy window
  glfwDestroyWindow(mWindow.handle);

  // Destroy command queues
  delete mCopyQueue;
  delete mComputeQueue;
  delete mGraphicsQueue;

  // Delete device
  delete mDevice;

  // Delete render context
  delete mContext;

  // Clear global object
  Assert(sInstance == this, "Destroying invalid application object");
  sInstance = nullptr;
}

// -------------------------------------------------------------------------- //

void
App::Run()
{
  // Show window
  CenterWindow();
  Show();

  // Run app loop
  mRunning = true;
  while (mRunning) {
    // GLFW updating
    glfwPollEvents();
    if (glfwWindowShouldClose(mWindow.handle)) {
      Exit();
    }

    // Update

    // Update fixed

    // Render
  }

  // Hide window
  Hide();
}

// -------------------------------------------------------------------------- //

void
App::Exit()
{
  mRunning = false;
}

// -------------------------------------------------------------------------- //

void
App::Show()
{
  glfwShowWindow(mWindow.handle);
}

// -------------------------------------------------------------------------- //

void
App::Hide()
{
  glfwHideWindow(mWindow.handle);
}

// -------------------------------------------------------------------------- //

void
App::EnterFullscreen(VideoMode* videoMode)
{
  // Window is already fullscreen
  if (mWindow.isFullscreen) {
    return;
  }

  // Set old window data
  mWindow.old.width = mWindow.width;
  mWindow.old.height = mWindow.height;
  mWindow.old.posX = mWindow.posX;
  mWindow.old.posY = mWindow.posY;

  // Retrieve video mode from window
  GLFWmonitor* monitor = MonitorForWindowGLFW(mWindow.handle);
  s32 monitorX, monitorY;
  glfwGetMonitorPos(monitor, &monitorX, &monitorY);
  VideoMode mode;
  if (!videoMode) {
    const GLFWvidmode* _mode = glfwGetVideoMode(monitor);
    mode.width = _mode->width;
    mode.height = _mode->height;
    mode.refresh = _mode->refreshRate;
  } else {
    mode = *videoMode;
  }

  // Set fullscreen
  glfwSetWindowMonitor(mWindow.handle,
                       monitor,
                       monitorX,
                       monitorY,
                       mode.width,
                       mode.height,
                       mode.refresh ? mode.refresh : GLFW_DONT_CARE);

  // Set fullscreen status
  mWindow.isFullscreen = true;
}

// -------------------------------------------------------------------------- //

void
App::ExitFullscreen()
{
  // Window is not fullscreen
  if (!mWindow.isFullscreen) {
    return;
  }

  // Exit fullscreen
  glfwSetWindowMonitor(mWindow.handle,
                       nullptr,
                       mWindow.old.posX,
                       mWindow.old.posY,
                       mWindow.old.width,
                       mWindow.old.height,
                       GLFW_DONT_CARE);

  // Set fullscreen status
  mWindow.isFullscreen = false;
}

// -------------------------------------------------------------------------- //

void
App::CenterWindow()
{
  // Retrieve monitor and video mode
  GLFWmonitor* monitor = MonitorForWindowGLFW(mWindow.handle);
  Assert(monitor != nullptr,
         "Failed to find monitor that window was located on");
  const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
  Assert(videoMode != nullptr, "Failed to query monitor video mode");

  // Update pos
  s32 monX, monY;
  glfwGetMonitorPos(monitor, &monX, &monY);
  s32 winW, winH;
  glfwGetWindowSize(mWindow.handle, &winW, &winH);
  glfwSetWindowPos(mWindow.handle,
                   monX + (videoMode->width - winW) / 2,
                   monY + (videoMode->height - winH) / 2);
}

// -------------------------------------------------------------------------- //

GLFWmonitor*
App::MonitorForWindowGLFW(GLFWwindow* window)
{
  // Credits: https://vallentin.io/2014/02/07/glfw-center-window

  // Retrieve list of monitors
  int count;
  GLFWmonitor** monitors = glfwGetMonitors(&count);
  if (!monitors) {
    return nullptr;
  }

  // Retrieve window size and pos
  int windowX, windowY, windowWidth, windowHeight;
  glfwGetWindowSize(window, &windowWidth, &windowHeight);
  glfwGetWindowPos(window, &windowX, &windowY);

  // Find monitor that contains window center
  GLFWmonitor* bestMonitor = NULL;
  int bestArea = 0;
  for (int i = 0; i < count; i++) {
    GLFWmonitor* monitor = monitors[i];

    // Retrieve monitor pos and video-mode
    int monitorX, monitorY;
    glfwGetMonitorPos(monitor, &monitorX, &monitorY);
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (!mode) {
      continue;
    }

    // Check if monitor contains more of window
    const int areaMinX = Max(windowX, monitorX);
    const int areaMinY = Max(windowY, monitorY);
    const int areaMaxX = Min(windowX + windowWidth, monitorX + mode->width);
    const int areaMaxY = Min(windowY + windowHeight, monitorY + mode->height);
    const int area = (areaMaxX - areaMinX) * (areaMaxY - areaMinY);
    if (area > bestArea) {
      bestArea = area;
      bestMonitor = monitor;
    }
  }
  return bestMonitor;
}

// -------------------------------------------------------------------------- //

void
App::ErrorCallbackGLFW(int error, const char8* description)
{
  Console::WriteErrLine("[GLFW] Error({}): {}", error, description);
}

// -------------------------------------------------------------------------- //

void
App::WindowResizeCallbackGLFW(GLFWwindow* window, int width, int height)
{
  auto app = static_cast<App*>(glfwGetWindowUserPointer(window));
  app->mWindow.width = u32(width);
  app->mWindow.height = u32(height);
  app->OnResize(app->mWindow.width, app->mWindow.height);
}

// -------------------------------------------------------------------------- //

void
App::WindowMoveCallbackGLFW(GLFWwindow* window, int xpos, int ypos)
{
  const auto app = static_cast<App*>(glfwGetWindowUserPointer(window));
  app->mWindow.posX = s32(xpos);
  app->mWindow.posY = s32(ypos);
}

// -------------------------------------------------------------------------- //

void
App::KeyCallbackGLFW(GLFWwindow* window,
                     int key,
                     int scancode,
                     int action,
                     int mods)
{
  auto app = static_cast<App*>(glfwGetWindowUserPointer(window));
  Key _key = static_cast<Key>(key);
  if (action == GLFW_RELEASE) {
    app->OnKeyRelease(_key);
  } else {
    if (bool(app->mFlags & Flag::kExitOnEscape) && _key == Key::kEscape) {
      app->Exit();
    }
    app->OnKeyPress(_key, action == GLFW_REPEAT);
  }
}

// -------------------------------------------------------------------------- //

void
App::MouseMotionCallbackGLFW(GLFWwindow* window, f64 xpos, f64 ypos)
{
  auto app = static_cast<App*>(glfwGetWindowUserPointer(window));
  app->OnMouseMove(xpos, ypos);
}

}
