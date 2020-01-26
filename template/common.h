#ifndef COMMON_H_
#define COMMON_H_

#include <string>

#include <GLFW/glfw3.h>

#include "ui/trackball.h"
#include "image/screengrabber.h"

//! This is a list of global variables (across source files)
/*!
  In this file we list the variables to be included in other files.

  This is a bad programming habit, but remember we do not have a main class
  and we split the main application code across several source files
  To mitigate this fact, we place them inside the namespace common
*/
namespace common {

//! Represents the state of a window in windowed mode
class WindowState {
public:
  GLFWmonitor* monitorPtr;
  int x_pos;
  int y_pos;
  int width;
  int height;
};

extern bool ogl_debug_log;

//! Object used to take screen-shoots
extern image::ScreenGrabber sg;

//! To store the state of window (before goiong into full-screen) so it can return to it later
extern WindowState window_state;

//! Main GLFW window and context handle
extern GLFWwindow* window;

//! Object used to cotroll the trackball camera's rotation
extern ui::Trackball ball;

//! Keep track if we are in camera edit mode
extern bool mouse_drag;

//! The current zoom level of the camera
extern int zoom_level;

//! Keep track if we are in roataion mode
extern bool rotating;

//! Keep track of the current angle (in degree)
extern float current_angle;

//! Contains the information about the current OpenGL context and the libraries version
extern std::string context_info;

//! Specular exponent (for the Blinn-Phong shading model)
extern float alpha;

//! Keep track if we render or not the user menu
extern bool show_menu;

//! Show hide DearImgui demo (if you need to quick look how to use a control)
extern bool show_demo_menu;

} //namsepace common

#endif