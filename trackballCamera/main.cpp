#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_PURE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "trackball.h"

// Define helpful macros for handling offsets into buffer objects
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
#define OFFSET_OF(type, member) ((GLvoid*)(offsetof(type, member)))
// Window and context handle
GLFWwindow* window = nullptr;
// Location for shader variables
GLint u_PVM_location = -1;
GLint a_position_loc = -1;
GLint a_color_loc = -1;
// OpenGL program handlers
GLuint vertex_shader;
GLuint fragment_shader;
GLuint program;
// Global variables for the program logic
int nTriangles;
Trackball trackball;
bool mouse_drag;
int zoom_level;
// Manage the Vertex Buffer Objects using a Vertex Array Object
GLuint vao;
// Function declarations
void init_glfw();
void load_OpenGL();
void init_program();
void create_primitives_and_send_to_gpu();
void render();
void free_resources();
// GLFW related callbacks
void register_glfw_callbacks();
void glfw_error_callback(int error, const char* description);
void key_callback(GLFWwindow* windowPtr, int key, int scancode, int action, int mods);
void resize_callback(GLFWwindow* windowPtr, int new_window_width, int new_window_height);
void mouse_button_callback(GLFWwindow* windowPtr, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* windowPtr, double mouse_x, double mouse_y);
void scroll_callback(GLFWwindow* windowPtr, double x_offset, double y_offset);

int main (int argc, char* argv[]) {
  init_glfw();
  load_OpenGL();
  init_program();
  register_glfw_callbacks();

  while (!glfwWindowShouldClose(window)) {
    render();
    glfwSwapBuffers(window);
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

  window = glfwCreateWindow(640, 480, "Simple trackball camera", nullptr, nullptr);
  if (!window) {
    // Window or context creation failed
    cerr << "OpenGL context not available" << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  // Context setting to happen before OpenGL's extension loader
  glfwMakeContextCurrent(window);
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
}

void init_program() {
  /************************************************************************/
  /*                   OpenGL program (pipeline) creation                 */
  /************************************************************************/
  using std::string;
  using std::cerr;
  using std::endl;
  /* In a normal program the shader should be in separate text files
  I put them here to avoid another layer of complexity */
  const string vertex_shader_src = R"GLSL(
    #version 130

    in vec3 Position;
    in vec3 Color;

    uniform mat4 PVM;

    out vec4 vColor;

    void main(void) {
      gl_Position = PVM * vec4(Position, 1.0f);
      vColor = vec4(Color, 1.0);
    }
  )GLSL";

  const string fragment_shader_src = R"GLSL(
    #version 130

    in vec4 vColor;

    out vec4 fragcolor;

    void main(void) {
      fragcolor = vColor;
    }
  )GLSL";

  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  const char* start = &vertex_shader_src[0];
  glShaderSource(vertex_shader, 1, &start, nullptr);

  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  start = &fragment_shader_src[0];
  glShaderSource(fragment_shader, 1, &start, nullptr);


  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  //glBindFragDataLocation(program, 0, "fragcolor");
  glLinkProgram(program);

  /* Now, that we have the program, query location of shader variables */
  u_PVM_location = glGetUniformLocation(program, "PVM");
  a_position_loc = glGetAttribLocation(program, "Position");
  a_color_loc = glGetAttribLocation(program, "Color");

  /* Then, create primitives and send data to GPU */
  create_primitives_and_send_to_gpu();
  //Initialize the trackball object
  int width;
  int height;
  glfwGetWindowSize(window, &width, &height);
  trackball = Trackball(width, height, 0.8f);
  mouse_drag = false;
  //Initialize some basic rendering state
  glEnable(GL_DEPTH_TEST);
  //Dark gray background color
  glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
}

void create_primitives_and_send_to_gpu() {
  //Create primitives
  struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
  };

  std::vector<Vertex> vertices;
  vertices.push_back(Vertex{{ sqrt(8.0f / 9.0f), 0.0f, -1.0f / 3.0f},              {1.0f, 0.25f, 0.25f}});
  vertices.push_back(Vertex{{-sqrt(2.0f / 9.0f), sqrt(2.0f / 3.0f), -1.0f / 3.0f}, {0.25f, 1.0f, 0.25f}});
  vertices.push_back(Vertex{{-sqrt(2.0f / 9.0f),-sqrt(2.0f / 3.0f), -1.0f / 3.0f}, {0.25f, 0.25f, 1.0f}});
  vertices.push_back(Vertex{{0.0f, 0.0f, 1.0f},                                    {1.0f,  1.0f, 0.25f}});

  std::vector<unsigned short> indices;

  indices.push_back(0u);
  indices.push_back(2u);
  indices.push_back(1u);

  indices.push_back(0u);
  indices.push_back(1u);
  indices.push_back(3u);

  indices.push_back(3u);
  indices.push_back(1u);
  indices.push_back(2u);

  indices.push_back(3u);
  indices.push_back(2u);
  indices.push_back(0u);

  nTriangles = indices.size() / 3;

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
  glEnableVertexAttribArray(a_color_loc);
  glVertexAttribPointer(a_color_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          OFFSET_OF(Vertex, color));
  //Now, the indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short),
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
  glUseProgram(program);
  /************************************************************************/
  /* Calculate  Model View Projection Matrices                            */
  /************************************************************************/
  //Math constant equal two PI (Remember, we are in radians)
  const float TAU = 6.28318f;
  //Identity matrix
  glm::mat4 I(1.0f);
  //Model
  glm::mat4 M = I;
  //View
  glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 3.5f);
  glm::vec3 camera_eye = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::mat4 V = glm::lookAt(camera_position, camera_eye, camera_up);
  // Use trackball to get a rotation applied to the camera's pose
  V = V * trackball.getRotation();
  //Projection
  int width;
  int height;
  glfwGetWindowSize(window, &width, &height);
  GLfloat aspect = float(width) / float(height);
  // zoom in and out is controlled with the fovy
  GLfloat fovy = TAU / 8.0f + zoom_level * (TAU / 50.0f);
  GLfloat zNear = 1.0f;
  GLfloat zFar = 5.0f;
  glm::mat4 P = glm::perspective(fovy, aspect, zNear, zFar);
  /************************************************************************/
  /* Send uniform values to shader                                        */
  /************************************************************************/
  if (u_PVM_location != -1) {
    glUniformMatrix4fv(u_PVM_location, 1, GL_FALSE, glm::value_ptr(P * V * M));
  }
  /************************************************************************/
  /* Bind buffer object and their corresponding attributes (use VAO)      */
  /************************************************************************/
  glBindVertexArray(vao);
  /* Draw */
  const int start_index = 0; //In location corresponding the index array
  glDrawElements(GL_TRIANGLES, 3 * nTriangles, GL_UNSIGNED_SHORT,
                 BUFFER_OFFSET(start_index * sizeof(unsigned short)));
  //Unbind and clean
  glBindVertexArray(0);
  glUseProgram(0);
}

void register_glfw_callbacks() {
  glfwSetWindowSizeCallback(window, resize_callback);
  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);
  glfwSetScrollCallback(window, scroll_callback);
}

void key_callback(GLFWwindow* windowPtr, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(windowPtr, GLFW_TRUE);
  }
}

void resize_callback(GLFWwindow* windowPtr, int new_window_width, int new_window_height) {
  glViewport(0, 0, new_window_width, new_window_height);
  trackball.setWindowSize(new_window_width, new_window_height);
}

void mouse_button_callback(GLFWwindow* windowPtr, int button, int action, int mods) {
  //The event happen outside the GUI, your application should try to handle it
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    mouse_drag = true;
    double mouse_x;
    double mouse_y;
    glfwGetCursorPos(windowPtr, &mouse_x, &mouse_y);
    trackball.startDrag(glm::ivec2(int(mouse_x), int(mouse_y)));
  } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    trackball.endDrag();
    mouse_drag = false;
  }
}

void cursor_position_callback(GLFWwindow* windowPtr, double mouse_x, double mouse_y) {
  if (mouse_drag) {
    trackball.drag(glm::ivec2(int(mouse_x), int(mouse_y)));
  }
}

void scroll_callback(GLFWwindow* windowPtr, double x_offset, double y_offset) {
  zoom_level += int(y_offset);
  zoom_level = glm::clamp(zoom_level, -5, 5);
}

void glfw_error_callback(int error, const char* description) {
  using std::cerr;
  using std::endl;
  cerr << "GLFW Error: " << description << endl;
}

void free_resources() {
  /* Delete OpenGL program */
  glDetachShader(program, vertex_shader);
  glDetachShader(program, fragment_shader);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  glDeleteProgram(program);
  //Window and context destruction
  glfwDestroyWindow(window);
}

