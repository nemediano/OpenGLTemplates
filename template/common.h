#ifndef COMMON_H_
#define COMMON_H_

#include <string>

#include <GLFW/glfw3.h>

#include "ui/trackball.h"
#include "image/screengrabber.h"

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

extern image::ScreenGrabber sg;

extern WindowState window_state;

// Window and context handle
extern GLFWwindow* window;

//
extern ui::Trackball ball;


extern bool mouse_drag;
extern int zoom_level;

extern bool rotating;
extern float current_angle;

extern std::string context_info;

extern float alpha;

extern bool show_menu;

} //namsepace common

#endif