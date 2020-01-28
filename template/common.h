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

//! Show hide DearImgui demo (if you need to quick look how to use a control)
//extern bool show_demo_menu;

} //namsepace common

#endif