#ifndef TEMPLATE_APP_H_
#define TEMPLATE_APP_H_

// ANSI C++ includes
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
// Third party libraries includes
// Dear imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
// GLEW
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GLM
#define GLM_FORCE_PURE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Includes from this template
#include "image/texture.h"
#include "image/proceduraltextures.h"
#include "mesh/model.h"
#include "ogl/oglhelpers.h"
#include "ogl/oglprogram.h"
#include "callbacks.h"
#include "common.h"

struct ProgramLocations {
  // Location for shader variables
  GLint uPVM;
  GLint uNormalMat;
  GLint uDiffuseMap;
  GLint uSpecularMap;
  GLint uAlpha;
  GLint aPosition;
  GLint aNormal;
  GLint aTextureCoord;
  
  ProgramLocations() {
    reset();
  }

  void reset () {
    uPVM = -1;
    uNormalMat = -1;
    uDiffuseMap = -1;
    uSpecularMap = -1;
    uAlpha = -1;
    aPosition = -1;
    aNormal = -1;
    aTextureCoord = -1;
  }
};


struct WindowState {
  GLFWmonitor* monitorPtr;
  int x_pos;
  int y_pos;
  int width;
  int height;
};

class TemplateApplication {
  public:
    void run();
    //! To store the state of window (before goiong into full-screen) so it can return to it later
    WindowState mWinState;
    //! Main GLFW window and context handle
    GLFWwindow* mWinPtr;
    //! Switches between full-screen and windowed mode
    /*!
      Queries if the window is in windowed mode. If it is, then stores the current window state
      and switches to full-screen mode. If it is not (hence we are full-screen) then reatrives
      the previous window state (position and size) and returns to windowed mode
    */
    void change_window_mode();
    //! Object used to take screen-shoots
    image::ScreenGrabber mSg;
  private:
    //! Keep track of the current angle (in degree)
    float mCurrentAngle;
    //! Specular exponent (for the Blinn-Phong shading model)
    float mAlpha;
    // See if we have a debug logger
    bool mHasDebug;
    // OGL shader porgram locations storage
    ProgramLocations mLoc;
    // OpenGL program handler
    ogl::OGLProgram* ogl_program_ptr = nullptr;
    // Two buffers to interact with the Model class
    std::vector<image::Texture*> textures;
    std::vector<mesh::MeshData> separators;
    // To keep track the elapsed time between frames
    double last_time = 0.0;
    // Vertex Array Object used to manage the Vertex Buffer Objects
    GLuint vao;
    void init_glfw();
    void load_OpenGL();
    void init_program();
    void load_model_data_and_send_to_gpu();
    void render();
    void update();
    void free_resources();
    //! Setup the IMGUI library to work with current OpenGL and GLFW, also general look and feel
    void setup_menu();
    //! Create the actual user menu of the application and connect the application with it.
    /*!
      Needs to be called every frame.
      If you want to add/remove input controls, this is the place
    */
    void create_menu();
    // Related to callbacks
    //! Register all the GLFW callbacks with the only window
    /*!
      Uses the gloal window handler, to register all the functions with respective events
    */
    void register_glfw_callbacks();
};

#endif
