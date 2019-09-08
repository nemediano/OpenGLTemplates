#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include <GLFW/glfw3.h>

// GLFW related callbacks

//! Register all the GLFW callbacks with the only window
/*!
  Uses the gloal window handler, to register all the functions with respective events
*/
void register_glfw_callbacks();
//! Callback function for internal GLFW errors
/*!
  If this function is executed an internal GLFW error was generated and the program will likely
  crash. The most common failure, is that the application tries to create an OpenGL context superior
  to the one actually supported by your SW (driver) and HW (GPU)
*/
void glfw_error_callback(int error, const char* description);
//! Handles any keyboard interaction on the window
void key_callback(GLFWwindow* windowPtr, int key, int scancode, int action, int mods);
//! Called every time thewindow changes size
void resize_callback(GLFWwindow* windowPtr, int new_window_width, int new_window_height);
//! Called every time the window's framebuffer changes size
void framebuffer_size_callback(GLFWwindow* windowPtr, int width, int height);
//! Called every time there is a click (in or out) on the window
void mouse_button_callback(GLFWwindow* windowPtr, int button, int action, int mods);
//! When the mouse moves (hovers the wondow)
void cursor_position_callback(GLFWwindow* windowPtr, double xpos, double ypos);
//! When the mouse scroll wheel is activated
void scroll_callback(GLFWwindow* windowPtr, double x_offset, double y_offset);

// Helper functions
//! Switches between full-screen and windowed mode
/*!
  Queries if the window is in windowed mode. If it is, then stores the current window state
  and switches to full-screen mode. If it is not (hence we are full-screen) then reatrives
  the previous window state (position and size) and returns to windowed mode
*/
void change_window_mode();
//! Determines which is the best monitor to go fullscreen
/*!
  When the window is in windowed (normal) mode. Determines which monitor contains the
  the greater area of the window
*/
GLFWmonitor* find_best_monitor(GLFWwindow *window);

#endif