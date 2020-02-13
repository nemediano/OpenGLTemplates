//Standar libraries includes
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
// Third party libraries includes
// GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Includes from this template
#include "ogl/oglhelpers.h"

// Includes from this project
#include "callbacks.h"
#include "templateApplication.h"

void TemplateApplication::init_glfw() {
  using std::cerr;
  using std::endl;
  // Set error log for GLFW
  glfwSetErrorCallback(glfw_error_callback);
  // Try to init libary
  if (!glfwInit()) {
    // Initialization failed
    cerr << "GLFW initialization failed!" << endl;
    throw std::runtime_error("GLFW initialization failed!");
  }
  // Library was initializated, now try window and context
  // This depends on the HW (GPU) and SW (Driver), use the best avialble
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // This lines require OpenGL 4.3 or above, comment them if you dont have it
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  mWinPtr = glfwCreateWindow(900, 600, "OpenGL Template", nullptr, nullptr);
  if (!mWinPtr) {
    // Window or context creation failed
    glfwTerminate();
    throw std::runtime_error("OpenGL context not available");
  }
  // Save window state
  mWinState.monitorPtr = glfwGetPrimaryMonitor();
  // Context setting needs to happen before OpenGL's extension loader
  glfwMakeContextCurrent(mWinPtr);
  glfwSwapInterval(1);
}

void TemplateApplication::load_OpenGL() {
  /************************************************************************/
  /*                    Init OpenGL context                               */
  /************************************************************************/
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    std::stringstream ss;
    ss << "Glew initialization failed: " << glewGetErrorString(err) << std::endl;
    throw std::runtime_error(ss.str());
  }
  // If our context allow us, ask for a debug callback
  mHasDebug = ogl::getErrorLog();
}

void TemplateApplication::init_program() {
  /************************************************************************/
  /*                   OpenGL program (pipeline) creation                 */
  /************************************************************************/
  mGLProgramPtr = new ogl::OGLProgram("shaders/vertex.glsl", "shaders/fragment.glsl");
  /* Now, that we have the program, query location of shader variables */
  mLoc.uPVM = mGLProgramPtr->uniformLoc("PVM");
  mLoc.uNormalMat = mGLProgramPtr->uniformLoc("NormalMat");
  mLoc.uAlpha = mGLProgramPtr->uniformLoc("uAlpha");
  mLoc.uDiffuseMap = mGLProgramPtr->uniformLoc("uDiffuseMap");
  mLoc.uSpecularMap = mGLProgramPtr->uniformLoc("uSpecularMap");
  mLoc.aPosition = mGLProgramPtr->attribLoc("posAttr");
  mLoc.aNormal = mGLProgramPtr->attribLoc("normalAttr");
  mLoc.aTextureCoord = mGLProgramPtr->attribLoc("textCoordAttr");
  /* Then, create primitives and send data to GPU */
  load_model_data_and_send_to_gpu();
  // Initialize some basic rendering state
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.15f, 0.15f, 0.15f, 1.0f); // Dark gray background color
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  // Initialize trackball camera
  int width;
  int height;
  glfwGetWindowSize(mWinPtr, &width, &height);
  mBall.setWindowSize(width, height);
  // Also, let the screen grabber know the current buffer size
  mSg.resize(width, height);
  // Initial values for program logic
  mAlpha = 4.0f;
  mShowMenu = true;
  mShowDemoMenu = false;
  mRotating = false;
  mZoomLevel = -1;
}

void TemplateApplication::load_model_data_and_send_to_gpu() {
  using namespace mesh;
  // Models location in filesystem
  const std::string model_folder = "models/Nyra/";
  const std::string model_path = model_folder + "Nyra_pose.obj";
  // Read model data
  Model model{model_path};
  model.toUnitCube(); // Rescale model
  // Query data
  std::vector<unsigned int> indices = model.getIndices();
  std::vector<Vertex> vertices = model.getVertices();
  // The separator will tell us how to render, since we destroy the model, we keep a copy
  mSeparators = model.getSeparators();
  // Since we use the model to get the paths for the textures, I need
  // fill the textures collection here
  for (auto t : model.getDiffuseTextures()) {
    std::string texture_file = model_folder + t.filePath;
    image::Texture* texture = new image::Texture(texture_file);
    texture->send_to_gpu();
    mTextures.push_back(texture);
  }
  // Create the vertex buffer objects and VAO
  GLuint vbo;
  GLuint indexBuffer;
  glGenVertexArrays(1, &mVao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &indexBuffer);
  // Bind the vao this need to be done before anything
  glBindVertexArray(mVao);
  // Send data to GPU: first send the vertices
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(mLoc.aPosition);
  glVertexAttribPointer(mLoc.aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        OFFSET_OF(Vertex, position));
  glEnableVertexAttribArray(mLoc.aNormal);
  glVertexAttribPointer(mLoc.aNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          OFFSET_OF(Vertex, normal));
  glEnableVertexAttribArray(mLoc.aTextureCoord);
  glVertexAttribPointer(mLoc.aTextureCoord, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          OFFSET_OF(Vertex, textCoords));
  // Now, the indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);
  // Unbind the vao we will use it for render
  glBindVertexArray(0);
  // Now that we have the data in the GPU and the reference in vao we do not need to keep it
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &indexBuffer);
  vertices.clear();
  indices.clear();
}

void TemplateApplication::render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  mGLProgramPtr->use();
  /************************************************************************/
  /* Calculate  Model, View and Projection Matrices                       */
  /************************************************************************/
  // Identity matrix, as start for some calculations
  glm::mat4 I(1.0f);
  // Model
  glm::mat4 M = glm::scale(I, 2.0f * glm::vec3(1.0f));
  if (mRotating) {
    M = glm::rotate(M, glm::radians(mCurrentAngle), glm::vec3(0.0f, 1.0f, 0.0f));
  }
  // View
  glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 3.5f);
  glm::vec3 camera_eye = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::mat4 Cam_Init_Pos = glm::lookAt(camera_position, camera_eye, camera_up);
  // Use trackball to get a rotation applied to the camera's initial pose
  glm::mat4 V = Cam_Init_Pos * mBall.getRotation();
  // Projection
  int width;
  int height;
  glfwGetWindowSize(mWinPtr, &width, &height);
  GLfloat aspect = float(width) / float(height);
  const float TAU = 6.28318f; // Math constant equal two PI (Remember, we are in radians)
  GLfloat fovy = TAU / 8.0f + mZoomLevel * (TAU / 50.0f);
  GLfloat zNear = 1.0f;
  GLfloat zFar = 5.0f;
  glm::mat4 P = glm::perspective(fovy, aspect, zNear, zFar);
  /************************************************************************/
  /* Send uniform values to shader                                        */
  /************************************************************************/
  if (mLoc.uPVM != -1) {
    glUniformMatrix4fv(mLoc.uPVM, 1, GL_FALSE, glm::value_ptr(P * V * M));
  }
  if (mLoc.uNormalMat != -1) {
    glUniformMatrix4fv(mLoc.uNormalMat, 1, GL_FALSE,
        glm::value_ptr(glm::inverse(glm::transpose(V * M))));
  }
  if (mLoc.uAlpha != -1) {
    glUniform1f(mLoc.uAlpha, mAlpha);
  }
  /************************************************************************/
  /* Bind buffer object and their corresponding attributes (use VAO)      */
  /************************************************************************/
  glBindVertexArray(mVao);
  /* Draw */
  for (size_t i = 0; i < mSeparators.size(); ++i) {
    mesh::MeshData sep = mSeparators[i];
    if (sep.diffuseIndex == -1 || sep.specIndex == -1) {
      // This mesh is missing some texture
      // Do not render (Not with these shaders at least)
      continue;
    }
    // Send diffuse texture in unit 0
    glActiveTexture(GL_TEXTURE0);
    mTextures[sep.diffuseIndex]->bind();
    glUniform1i(mLoc.uDiffuseMap, 0);
    // Send specular texture in unit 1
    glActiveTexture(GL_TEXTURE1);
    mTextures[sep.specIndex]->bind();
    glUniform1i(mLoc.uSpecularMap, 1);
    // Now draw this mesh indexes by using (by query) the separator
    glDrawElementsBaseVertex(GL_TRIANGLES, sep.howMany, GL_UNSIGNED_INT,
                             reinterpret_cast<void*>(sep.startIndex * int(sizeof(unsigned int))),
                             sep.startVertex);
  }
  // Clean the state for other render (could be the UI, other rendering pass, or the next frame)
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}

void TemplateApplication::update() {
  // We use GLFW (rather than OpenGL) to timer
  double time = glfwGetTime();
  double elapsed = time - mLastTime; // elapsed is time in seconds between frames
  mLastTime = time;
  /* If rotating, then update angle*/
  if (mRotating) {
    const float speed = 180.0f; // In degrees per second
    mCurrentAngle += float(elapsed) * speed;
    if (mCurrentAngle > 360.0f) {
      int quotient = int(mCurrentAngle / 360.0f);
      // Current angle is the float point modulo with 360.0f of previous current angle
      mCurrentAngle -= quotient * 360.0f;
    }
  }
}

void TemplateApplication::free_resources() {
  /* Release imgui resources */
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  /* Delete texture pointers */
  for (size_t i = 0; i < mTextures.size(); ++i) {
    delete mTextures[i];
  }
  /* Delete OpenGL program */
  delete mGLProgramPtr;
  // Window and context destruction
  glfwDestroyWindow(mWinPtr);
}

void TemplateApplication::run() {
  // Application's setup
  init_glfw();
  load_OpenGL();
  setup_menu();
  init_program();
  register_glfw_callbacks();
  // Application's main loop
  while (!glfwWindowShouldClose(mWinPtr)) {
    if (mShowMenu) {
      create_menu();
      if (mShowDemoMenu) {
        ImGui::ShowDemoWindow();
      }
      ImGui::Render(); // Prepare to render our menu, before clearing buffers (Before scene)
    }
    render();  // Render scene
    // Render the user menu (After scene)
    if (mShowMenu) {
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    glfwSwapBuffers(mWinPtr);
    update();
    glfwPollEvents();
  }
  // Window was closed. Clean and finalize
  free_resources();
}