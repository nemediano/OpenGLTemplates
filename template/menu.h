#ifndef MENU_H_
#define MENU_H_

//! Functions related to the GUI
/*!
  Uses Dear ImGUI, the suurce files of the librarie are in the imgui folder
*/

//! Setup the IMGUI library to work with current OpenGL and GLFW, also general lookk and feel
void setup_menu();
//! Create the actual user menu of the application and connect the application with it.
/*!
  Needs to be called every frame.
  If you want to add/remove input controls, this is the place
*/
void create_menu();

#endif