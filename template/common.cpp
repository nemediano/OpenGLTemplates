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

//! Show hide DearImgui demo (if you need to quick look how to use a control)
//bool show_demo_menu = false;

} //namespace common