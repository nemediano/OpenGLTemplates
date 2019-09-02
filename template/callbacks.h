#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include <GLFW/glfw3.h>

// GLFW related callbacks
void register_glfw_callbacks();
void glfw_error_callback(int error, const char* description);
void key_callback(GLFWwindow* windowPtr, int key, int scancode, int action, int mods);
void resize_callback(GLFWwindow* windowPtr, int new_window_width, int new_window_height);
void framebuffer_size_callback(GLFWwindow* windowPtr, int width, int height);
void mouse_button_callback(GLFWwindow* windowPtr, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* windowPtr, double xpos, double ypos);
void scroll_callback(GLFWwindow* windowPtr, double x_offset, double y_offset);

void change_window_mode();

#endif