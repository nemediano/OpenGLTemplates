#ifndef TEMPLATE_APP_H_
#define TEMPLATE_APP_H_

// ANSI C++ includes
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

// Includes from this template
#include "image/texture.h"
#include "image/screengrabber.h"
#include "mesh/model.h"

#include "ogl/oglprogram.h"
#include "ui/trackball.h"


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
    // All the things that need to be accees from the GLFW callbacks needs to be either public
    // or have accesor/mutator methods. Since I prefer simplicity I made them public

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
    //! Object used to controll the trackball camera's rotation
    ui::Trackball mBall;
    //! Keep track if we are in camera edit mode
    bool mMouseDrag = false;
    //! The current zoom level of the camera
    int mZoomLevel;
    //! Keep track if we are in roatation mode
    bool mRotating;
    //! Keep track of the current angle (in degree)
    float mCurrentAngle;
    //! Keep track if we render or not the user menu
    bool mShowMenu;
    //! Show hide DearImgui demo (if you need to quick look how to use a control)
    bool mShowDemoMenu;
  private:
    //! Specular exponent (for the Blinn-Phong shading model)
    float mAlpha;
    // See if we have a debug logger
    bool mHasDebug;
    // OGL shader porgram locations storage
    ProgramLocations mLoc;
    // OpenGL program handler
    ogl::OGLProgram* mGLProgramPtr = nullptr;
    // Two buffers to interact with the Model class
    std::vector<image::Texture*> mTextures;
    std::vector<mesh::MeshData> mSeparators;
    // To keep track the elapsed time between frames
    double mLastTime = 0.0;
    // Vertex Array Object used to manage the Vertex Buffer Objects
    GLuint mVao;
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
