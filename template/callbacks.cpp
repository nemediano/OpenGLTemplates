#include <iostream>

#include "imgui/imgui.h"

#include "common.h"
#include "callbacks.h"

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
    //glfwSetWindowMonitor(common::window, nullptr, common::window_state.x_pos, common::window_state.y_pos,
    //    common::window_state.width, common::window_state.height, 0);
  } else { // go to full screen
    glfwGetWindowPos(common::window, &common::window_state.x_pos, &common::window_state.y_pos);
    glfwGetWindowSize(common::window, &common::window_state.width, &common::window_state.height);
    //const GLFWvidmode* mode = glfwGetVideoMode(common::window_state.monitorPtr);
    //glfwSetWindowMonitor(common::window, common::window_state.monitorPtr, 0, 0, mode->width,
    //    mode->height, mode->refreshRate);
  }
}