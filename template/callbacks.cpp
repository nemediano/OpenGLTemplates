#include <iostream>

#include "imgui/imgui.h"

#include "callbacks.h"
#include "common.h"

void register_glfw_callbacks() {
  glfwSetWindowSizeCallback(common::window, resize_callback);
  glfwSetFramebufferSizeCallback(common::window, framebuffer_size_callback);
  glfwSetKeyCallback(common::window, key_callback);
  glfwSetMouseButtonCallback(common::window, mouse_button_callback);
  glfwSetCursorPosCallback(common::window, cursor_position_callback);
  glfwSetScrollCallback(common::window, scroll_callback);
}

void key_callback(GLFWwindow* windowPtr, int key, int scancode, int action, int mods) {
  ImGuiIO& io = ImGui::GetIO();
  //Imgui wants this event, since it happen inside the GUI
  if (io.WantCaptureKeyboard) {
    return;
  }
  //The event happen outside the GUI, your application should try to handle it
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(windowPtr, 1);
  } else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
    common::rotating = !common::rotating;
    common::current_angle = 0.0f;
  } else if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
    change_window_mode();
  } else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
    common::sg.grab();
  } else if (key == GLFW_KEY_M && action == GLFW_PRESS) {
    common::show_menu = !common::show_menu;
  }
}

void mouse_button_callback(GLFWwindow* windowPtr, int button, int action, int mods) {
  ImGuiIO& io = ImGui::GetIO();
  //Imgui wants this event, since it happen inside the GUI
  if (io.WantCaptureMouse) {
    return;
  }
  //The event happen outside the GUI, your application should try to handle it
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    common::mouse_drag = true;
    double mouse_x;
    double mouse_y;
    glfwGetCursorPos(windowPtr, &mouse_x, &mouse_y);
    common::ball.startDrag(glm::ivec2(int(mouse_x), int(mouse_y)));
  } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    common::ball.endDrag();
    common::mouse_drag = false;
  }
}

void cursor_position_callback(GLFWwindow* windowPtr, double mouse_x, double mouse_y) {
  ImGuiIO& io = ImGui::GetIO();
  //Imgui wants this event, since it happen inside the GUI
  if (io.WantCaptureMouse) {
    return;
  }

  if (common::mouse_drag) {
    common::ball.drag(glm::ivec2(int(mouse_x), int(mouse_y)));
  }
}

void scroll_callback(GLFWwindow* windowPtr, double x_offset, double y_offset) {
  common::zoom_level += int(y_offset);
  common::zoom_level = glm::clamp(common::zoom_level, -5, 5);
}

void resize_callback(GLFWwindow* windowPtr, int new_window_width, int new_window_height) {
  glViewport(0, 0, new_window_width, new_window_height);
  common::ball.setWindowSize(new_window_width, new_window_height);
}

void framebuffer_size_callback(GLFWwindow* windowPtr, int width, int height) {
  common::sg.resize(width, height);
}

void glfw_error_callback(int error, const char* description) {
  using std::cerr;
  using std::endl;
  cerr << "GLFW Error: " << description << endl;
}

void change_window_mode() {
  //Windowed windows return null as their monitor
  GLFWmonitor* monitor = glfwGetWindowMonitor(common::window);

  if (monitor) { // Go to windowed mode
    common::window_state.monitorPtr = monitor;
    glfwSetWindowMonitor(common::window, nullptr, common::window_state.x_pos, common::window_state.y_pos,
        common::window_state.width, common::window_state.height, 0);
  } else { // go to full screen
    glfwGetWindowPos(common::window, &common::window_state.x_pos, &common::window_state.y_pos);
    glfwGetWindowSize(common::window, &common::window_state.width, &common::window_state.height);
    common::window_state.monitorPtr = find_best_monitor(common::window);
    const GLFWvidmode* mode = glfwGetVideoMode(common::window_state.monitorPtr);
    glfwSetWindowMonitor(common::window, common::window_state.monitorPtr, 0, 0, mode->width,
        mode->height, mode->refreshRate);
  }
}

/**
 * I took this algorithm (idea) from here:
 * http://stackoverflow.com/questions/21421074/
 *     how-to-create-a-full-screen-window-on-the-current-monitor-with-glfw
 */

GLFWmonitor* find_best_monitor(GLFWwindow *windowPtr) {
  // Get window's info: position and size
  glm::ivec2 win_pos;
  glm::ivec2 win_size;
  glfwGetWindowPos(windowPtr, &win_pos.x, &win_pos.y);
  glfwGetWindowSize(windowPtr, &win_size.x, &win_size.y);
  // Query the number of monitors and get handle of them
  int num_monitors = 0;
  GLFWmonitor **monitors = glfwGetMonitors(&num_monitors);
  // The initial best values is minimun possible: no overlaping
  int best_overlap = 0;
  GLFWmonitor *best_monitor = nullptr;
  // Loop checking all the available monitors to see
  // if one of them has better overlaping area.
  for (int i = 0; i < num_monitors; i++) {
    // Get current monitor's info
    glm::ivec2 mon_pos;
    glm::ivec2 mon_size;
    const GLFWvidmode *mode = glfwGetVideoMode(monitors[i]);
    mon_size.x = mode->width;
    mon_size.y = mode->height;
    glfwGetMonitorPos(monitors[i], &mon_pos.x, &mon_pos.y);
    // Calculate the area of overlap between this monitor and the window
    // The overlap is always a rectangle, so rect height time rect width
    int overlap =
      glm::max(0, glm::min(win_pos.x + win_size.x, mon_pos.x + mon_size.x) - glm::max(win_pos.x, mon_pos.x)) *  // width
      glm::max(0, glm::min(win_pos.y + win_size.y, mon_pos.y + mon_size.y) - glm::max(win_pos.y, mon_pos.y));   // height
    // If this area is better than our current best, then this is the new best
    if (best_overlap < overlap) {
      best_overlap = overlap;
      best_monitor = monitors[i];
    }
  }

  return best_monitor;
}