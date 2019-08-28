#ifndef COMMON_H_
#define COMMON_H_

#include <GLFW/glfw3.h>

#include "ui/trackball.h"

namespace common {

// Keep track of the window state
class WindowState {
public:
  GLFWmonitor* monitorPtr;
  int x_pos;
  int y_pos;
  int width;
  int height;
};

extern WindowState window_state;

// Window and context handle
extern GLFWwindow* window;

//
extern ui::Trackball ball;


extern bool mouse_drag;
extern int zoom_level;

extern bool rotating;
extern float current_angle;

} //namsepace common

#endif