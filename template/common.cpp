#include "common.h"

namespace common {

GLFWwindow* window = nullptr;

ui::Trackball ball;

WindowState window_state;

bool mouse_drag = false;
int zoom_level = 0;

bool rotating = false;
float current_angle = 0.0f;

std::string context_info;

} //namespace common