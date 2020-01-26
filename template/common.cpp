#include "common.h"

//! This is a list of global variables (across source files)
/*!

  In this file we need to actually declare and/or initialize the variables
  Even if the default value is OK, they need to be present here

  This is a bad programming habit, but remember we do not have a main class
  and we split the main application code across several source files
  To mitigate this fact, we place them inside the namespace common
*/

namespace common {

bool ogl_debug_log = false;

//! Object used to take screen-shoots
image::ScreenGrabber sg;

//! To store the state of window (before goiong into full-screen) so it can return to it later
WindowState window_state;

//! Main GLFW window and context handle
GLFWwindow* window = nullptr;

//! Object used to cotroll the trackball camera's rotation
ui::Trackball ball;

//! Keep track if we are in camera edit mode
bool mouse_drag = false;

//! The current zoom level of the camera
int zoom_level = 0;

//! Keep track if we are in roatation mode
bool rotating = false;

//! Keep track of the current angle (in degree)
float current_angle = 0.0f;

//! Contains the information about the current OpenGL context and the libraries version
std::string context_info;

//! Specular exponent (for the Blinn-Phong shading model)
float alpha = 0.0f;

//! Keep track if we render or not the user menu
bool show_menu = true;

//! Show hide DearImgui demo (if you need to quick look how to use a control)
bool show_demo_menu = false;

} //namespace common