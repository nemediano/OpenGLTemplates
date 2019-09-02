#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
// Dear imgui related headers
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_PURE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "image/texture.h"
#include "image/proceduraltextures.h"
#include "ogl/oglhelpers.h"
#include "ogl/oglprogram.h"
#include "common.h"
#include "menu.h"
#include "callbacks.h"
#include "mesh/model.h"

// Location for shader variables
GLint u_PVM_location = -1;
GLint u_NormalMat_location = -1;
GLint u_DiffuseMap_location = -1;
GLint u_SpecularMap_location = -1;
GLint u_Alpha_location = -1;
GLint a_position_loc = -1;
GLint a_normal_loc = -1;
GLint a_textureCoord_loc = -1;
// OpenGL program handlers
ogl::OGLProgram* ogl_program_ptr = nullptr;
std::vector<image::Texture*> textures;
std::vector<mesh::MeshData> separators;
// Global variables for the program logic
double last_time = 0.0;

// Manage the Vertex Buffer Objects using a Vertex Array Object
GLuint vao;

// Function declarations
void init_glfw();
void load_OpenGL();
void init_program();
void create_primitives_and_send_to_gpu();

void render();
void update();
void free_resources();

int main (int argc, char* argv[]) {
  init_glfw();
  load_OpenGL();
  setup_menu();
  init_program();
  register_glfw_callbacks();

  while (!glfwWindowShouldClose(common::window)) {
    create_menu();
    render();
    update();
    glfwSwapBuffers(common::window);
    glfwPollEvents();
  }
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
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
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
  // Context setting to happen before OpenGL's extension loader
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
  common::context_info = ogl::enviroment_info();

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
  create_primitives_and_send_to_gpu();
  //Initialize some basic rendering state
  glEnable(GL_DEPTH_TEST);
  //Dark gray background color
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  // Initialize trackball camera
  int width;
  int height;
  glfwGetWindowSize(common::window, &width, &height);
  common::ball.setWindowSize(width, height);
  common::sg.resize(width, height);
  common::alpha = 16.0f;
}

void create_primitives_and_send_to_gpu() {
  using namespace mesh;
  const std::string model_folder = "models/Nyra/";
  const std::string model_path = model_folder + "Nyra_pose.obj";

  Model model{model_path};
  model.toUnitCube();
  std::vector<unsigned int> indices = model.getIndices();
  std::vector<Vertex> vertices = model.getVertices();
  separators = model.getSeparators();
  //Since we use the model to get the paths for the textures, I need to do this here
  for (auto t : model.getDiffuseTextures()) {
    std::string texture_file = model_folder + t.filePath;
    //std::cout << "Textfile: " << texture_file << std::endl;
    image::Texture* texture = new image::Texture(texture_file);
    texture->send_to_gpu();
    textures.push_back(texture);
  }

  //Create the vertex buffer objects and VAO
  GLuint vbo;
  GLuint indexBuffer;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &indexBuffer);
  // Bind the vao this need to be done before anything
  glBindVertexArray(vao);
  //Send data to GPU: first send the vertices
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
  //Now, the indices
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
  ImGui::Render(); // Prepare to render our menu, before clearing buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ogl_program_ptr->use();
  /************************************************************************/
  /* Calculate  Model View Projection Matrices                            */
  /************************************************************************/
  //Identity matrix
  glm::mat4 I(1.0f);
  //Model
  glm::mat4 M = glm::scale(I, 2.0f * glm::vec3(1.0f));
  if (common::rotating) {
    M = glm::rotate(M, glm::radians(common::current_angle), glm::vec3(0.0f, 1.0f, 0.0f));
  }
  //View
  glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 3.5f);
  glm::vec3 camera_eye = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::mat4 Cam_Init_Pos = glm::lookAt(camera_position, camera_eye, camera_up);
  // Use trackball to get a rotation applied to the camera's initial pose
  glm::mat4 V = Cam_Init_Pos * common::ball.getRotation();
  //Projection
  int width;
  int height;
  glfwGetWindowSize(common::window, &width, &height);
  GLfloat aspect = float(width) / float(height);
  //Math constant equal two PI (Remember, we are in radians)
  const float TAU = 6.28318f;
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
        //This mesh does is missing some texture
        //Do not render (Not with this shader at least)
        continue;
    }
    //Send diffuse texture in unit 0
    glActiveTexture(GL_TEXTURE0);
    textures[sep.diffuseIndex]->bind();
    glUniform1f(u_DiffuseMap_location, 0);
    //Send specular texture in unit 1
    glActiveTexture(GL_TEXTURE1);
    textures[sep.specIndex]->bind();
    glUniform1f(u_SpecularMap_location, 1);
    glDrawElementsBaseVertex(GL_TRIANGLES, sep.howMany, GL_UNSIGNED_INT,
                             reinterpret_cast<void*>(sep.startIndex * int(sizeof(unsigned int))),
                             sep.startVertex);
  }
  // Clean state
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
  glUseProgram(0);
  /* Render menu after the geometry of our actual app*/
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void update() {
  double time = glfwGetTime();
  double elapsed = time - last_time;
  last_time = time;
  /*If rotating update angle*/
  if (common::rotating) {
    const float speed = 180.0f; //In degrees per second
    common::current_angle += float(elapsed) * speed;
    if (common::current_angle > 360.0f) {
      int quotient = int(common::current_angle / 360.0f);
      common::current_angle -= quotient * 360.0f;
    }
  }
}

void free_resources() {
  /* Release imgui resources */
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  for (size_t i = 0; i < textures.size(); ++i) {
    delete textures[i];
  }
  /* Delete OpenGL program */
  delete ogl_program_ptr;
  //Window and context destruction
  glfwDestroyWindow(common::window);
}


