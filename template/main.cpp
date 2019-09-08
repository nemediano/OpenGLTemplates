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
#include "menu.h"

// Location for shader variables
GLint u_PVM_location = -1;
GLint u_NormalMat_location = -1;
GLint u_DiffuseMap_location = -1;
GLint u_SpecularMap_location = -1;
GLint u_Alpha_location = -1;
GLint a_position_loc = -1;
GLint a_normal_loc = -1;
GLint a_textureCoord_loc = -1;
// OpenGL program handler
ogl::OGLProgram* ogl_program_ptr = nullptr;
// Two buffers to interact with the Model class
std::vector<image::Texture*> textures;
std::vector<mesh::MeshData> separators;
// To keep track the elapsed time between frames
double last_time = 0.0;
// Vertex Array Object used to manage the Vertex Buffer Objects
GLuint vao;
// Function declarations
void init_glfw();
void load_OpenGL();
void init_program();
void load_model_data_and_send_to_gpu();
void render();
void update();
void free_resources();

int main (int argc, char* argv[]) {
  // Application's setup
  init_glfw();
  load_OpenGL();
  setup_menu();
  init_program();
  register_glfw_callbacks();
  // Application's main loop
  while (!glfwWindowShouldClose(common::window)) {
    if (common::show_menu) {
      create_menu();
      ImGui::Render(); // Prepare to render our menu, before clearing buffers (Before scene)
    }
    render();  // Render scene
    // Render the user menu (After scene)
    if (common::show_menu) {
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    glfwSwapBuffers(common::window);
    update();
    glfwPollEvents();
  }
  // Window was closed. Clean and finalize
  free_resources();
  return EXIT_SUCCESS;
}

void init_glfw() {
  using std::cerr;
  using std::endl;
  // Set error log for GLFW
  glfwSetErrorCallback(glfw_error_callback);
  // Try to init libary
  if (!glfwInit()) {
    // Initialization failed
    cerr << "GLFW initialization failed!" << endl;
    exit(EXIT_FAILURE);
  }
  // Library was initializated, now try window and context
  // This depends on the HW (GPU) and SW (Driver), use the best avialble
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // This lines require OpenGL 4.3 or above, comment them if you dont have it
  //glfwWindowHint(GLFW_SAMPLES, 4);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  common::window = glfwCreateWindow(900, 600, "OpenGL Template", nullptr, nullptr);
  if (!common::window) {
    // Window or context creation failed
    cerr << "OpenGL context not available" << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  // Save window state
  common::window_state.monitorPtr = glfwGetPrimaryMonitor();
  // Context setting needs to happen before OpenGL's extension loader
  glfwMakeContextCurrent(common::window);
  glfwSwapInterval(1);
}

void load_OpenGL() {
  using std::cout;
  using std::cerr;
  using std::endl;
  /************************************************************************/
  /*                    Init OpenGL context                               */
  /************************************************************************/
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    cerr << "Glew initialization failed: " << glewGetErrorString(err) << endl;
  }
  // Save a string with the enviroment and context info (we display it in menu)
  common::context_info = ogl::enviroment_info();
  // If our context allow us, ask for a debug callback
  if (ogl::getErrorLog()) {
    cout << "OpenGL's debug logger active" << endl;
  } else {
    cout << "OpenGL's debug disabled" << endl;
  }
}

void init_program() {
  /************************************************************************/
  /*                   OpenGL program (pipeline) creation                 */
  /************************************************************************/
  ogl_program_ptr = new ogl::OGLProgram("shaders/vertex.glsl", "shaders/fragment.glsl");
  /* Now, that we have the program, query location of shader variables */
  u_PVM_location = ogl_program_ptr->uniformLoc("PVM");
  u_NormalMat_location = ogl_program_ptr->uniformLoc("NormalMat");
  u_Alpha_location = ogl_program_ptr->uniformLoc("uAlpha");
  u_DiffuseMap_location = ogl_program_ptr->uniformLoc("uDiffuseMap");
  u_SpecularMap_location = ogl_program_ptr->uniformLoc("uSpecularMap");
  a_position_loc = ogl_program_ptr->attribLoc("posAttr");
  a_normal_loc = ogl_program_ptr->attribLoc("normalAttr");
  a_textureCoord_loc = ogl_program_ptr->attribLoc("textCoordAttr");
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
  glfwGetWindowSize(common::window, &width, &height);
  common::ball.setWindowSize(width, height);
  // Also, let the screen grabber know the current buffer size
  common::sg.resize(width, height);
  // Initial values for program logic
  common::alpha = 4.0f;
  common::show_menu = true;
  common::rotating = false;
  common::zoom_level = -1;
}

void load_model_data_and_send_to_gpu() {
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
  separators = model.getSeparators();
  // Since we use the model to get the paths for the textures, I need
  // fill the textures collection here
  for (auto t : model.getDiffuseTextures()) {
    std::string texture_file = model_folder + t.filePath;
    image::Texture* texture = new image::Texture(texture_file);
    texture->send_to_gpu();
    textures.push_back(texture);
  }
  // Create the vertex buffer objects and VAO
  GLuint vbo;
  GLuint indexBuffer;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &indexBuffer);
  // Bind the vao this need to be done before anything
  glBindVertexArray(vao);
  // Send data to GPU: first send the vertices
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(a_position_loc);
  glVertexAttribPointer(a_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        OFFSET_OF(Vertex, position));
  glEnableVertexAttribArray(a_normal_loc);
  glVertexAttribPointer(a_normal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          OFFSET_OF(Vertex, normal));
  glEnableVertexAttribArray(a_textureCoord_loc);
  glVertexAttribPointer(a_textureCoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
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

void render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ogl_program_ptr->use();
  /************************************************************************/
  /* Calculate  Model, View and Projection Matrices                       */
  /************************************************************************/
  // Identity matrix, as start for some calculations
  glm::mat4 I(1.0f);
  // Model
  glm::mat4 M = glm::scale(I, 2.0f * glm::vec3(1.0f));
  if (common::rotating) {
    M = glm::rotate(M, glm::radians(common::current_angle), glm::vec3(0.0f, 1.0f, 0.0f));
  }
  // View
  glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 3.5f);
  glm::vec3 camera_eye = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::mat4 Cam_Init_Pos = glm::lookAt(camera_position, camera_eye, camera_up);
  // Use trackball to get a rotation applied to the camera's initial pose
  glm::mat4 V = Cam_Init_Pos * common::ball.getRotation();
  // Projection
  int width;
  int height;
  glfwGetWindowSize(common::window, &width, &height);
  GLfloat aspect = float(width) / float(height);
  const float TAU = 6.28318f; // Math constant equal two PI (Remember, we are in radians)
  GLfloat fovy = TAU / 8.0f + common::zoom_level * (TAU / 50.0f);
  GLfloat zNear = 1.0f;
  GLfloat zFar = 5.0f;
  glm::mat4 P = glm::perspective(fovy, aspect, zNear, zFar);
  /************************************************************************/
  /* Send uniform values to shader                                        */
  /************************************************************************/
  if (u_PVM_location != -1) {
    glUniformMatrix4fv(u_PVM_location, 1, GL_FALSE, glm::value_ptr(P * V * M));
  }
  if (u_NormalMat_location != -1) {
    glUniformMatrix4fv(u_NormalMat_location, 1, GL_FALSE,
        glm::value_ptr(glm::inverse(glm::transpose(V * M))));
  }
  if (u_Alpha_location != -1) {
    glUniform1f(u_Alpha_location, common::alpha);
  }
  /************************************************************************/
  /* Bind buffer object and their corresponding attributes (use VAO)      */
  /************************************************************************/
  glBindVertexArray(vao);
  /* Draw */
  for (size_t i = 0; i < separators.size(); ++i) {
    mesh::MeshData sep = separators[i];
    if (sep.diffuseIndex == -1 || sep.specIndex == -1) {
      // This mesh is missing some texture
      // Do not render (Not with these shaders at least)
      continue;
    }
    // Send diffuse texture in unit 0
    glActiveTexture(GL_TEXTURE0);
    textures[sep.diffuseIndex]->bind();
    glUniform1i(u_DiffuseMap_location, 0);
    // Send specular texture in unit 1
    glActiveTexture(GL_TEXTURE1);
    textures[sep.specIndex]->bind();
    glUniform1i(u_SpecularMap_location, 1);
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

void update() {
  // We use GLFW (rather than OpenGL) to timer
  double time = glfwGetTime();
  double elapsed = time - last_time; // elapsed is time in seconds between frames
  last_time = time;
  /* If rotating, then update angle*/
  if (common::rotating) {
    const float speed = 180.0f; // In degrees per second
    common::current_angle += float(elapsed) * speed;
    if (common::current_angle > 360.0f) {
      int quotient = int(common::current_angle / 360.0f);
      // Current angle is the float point modulo with 360.0f of previous current angle
      common::current_angle -= quotient * 360.0f;
    }
  }
}

void free_resources() {
  /* Release imgui resources */
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  /* Delete texture pointers */
  for (size_t i = 0; i < textures.size(); ++i) {
    delete textures[i];
  }
  /* Delete OpenGL program */
  delete ogl_program_ptr;
  // Window and context destruction
  glfwDestroyWindow(common::window);
}
