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

// GLFW related callbacks
void glfw_error_callback(int error, const char* description);
void key_callback(GLFWwindow* windowPtr, int key, int scancode, int action, int mods);
void resize_callback(GLFWwindow* windowPtr, int new_window_width, int new_window_height);
void mouse_button_callback(GLFWwindow* windowPtr, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* windowPtr, double mouse_x, double mouse_y);
void scroll_callback(GLFWwindow* windowPtr, double x_offset, double y_offset);

class OpenGLApplication {
public:
	void run();
	Trackball m_trackball;  //Trackball to change the camera rotation
	bool m_mouse_drag;      //To keep track the mouse drag for camera rotation
	int m_zoom_level;       //Zoom evel to complement the trackball camera
private:
	// Window and context handle
	GLFWwindow* m_window = nullptr;
	// Location for shader variables
	GLint m_u_PVM_location = -1;
	GLint m_a_position_loc = -1;
	GLint m_a_color_loc = -1;
	// OpenGL program handlers
	GLuint m_vertex_shader;
	GLuint m_fragment_shader;
	GLuint m_program;
	// Global variables for the program logic
	int m_nTriangles;

	// Manage the Vertex Buffer Objects using a Vertex Array Object
	GLuint m_vao;
	// See if this platform supports raw mouse motion
	bool m_raw_mouse_supported;

	void init_glfw();
	void load_OpenGL();
	void init_program();
	void create_primitives_and_send_to_gpu();
	void render();
	void free_resources();
	void register_glfw_callbacks();
};

int main (int argc, char* argv[]) {
  OpenGLApplication app;

  try {
	  app.run();
  } catch (const std::exception& e) {
	  std::cerr << e.what() << std::endl;
	  return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void OpenGLApplication::run() {
  init_glfw();
  load_OpenGL();
  init_program();
  register_glfw_callbacks();

  while (!glfwWindowShouldClose(m_window)) {
	render();
	glfwSwapBuffers(m_window);
	glfwPollEvents();
  }
  free_resources();
}

void OpenGLApplication::init_glfw() {
  using std::cerr;
  using std::endl;
  // Set error log for GLFW
  glfwSetErrorCallback(glfw_error_callback);
  // Try to init libary
  if (!glfwInit()) {
    // Initialization failed
	std::stringstream ss;
	ss << "GLFW initialization failed!" << endl;
	throw std::runtime_error(ss.str());
  }
  // Library was initializated, now try window and context
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  m_window = glfwCreateWindow(640, 480, "Simple trackball camera", nullptr, nullptr);
  if (!m_window) {
    // Window or context creation failed
	glfwTerminate();
	std::stringstream ss;
    ss << "OpenGL context not available" << endl;
    throw std::runtime_error(ss.str());
  }
  // Query if this platform support raw mouse motion
  m_raw_mouse_supported = glfwRawMouseMotionSupported();
  // Context setting to happen before OpenGL's extension loader
  glfwMakeContextCurrent(m_window);
  glfwSwapInterval(1);
}

void OpenGLApplication::load_OpenGL() {
  using std::cout;
  using std::cerr;
  using std::endl;
  /************************************************************************/
  /*                    Init OpenGL context                               */
  /************************************************************************/
  GLenum err = glewInit();
  if (GLEW_OK != err) {
	std::stringstream ss;
    ss << "Glew initialization failed: " << glewGetErrorString(err) << endl;
    throw std::runtime_error(ss.str());
  }
}

void OpenGLApplication::init_program() {
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

  m_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  const char* start = &vertex_shader_src[0];
  glShaderSource(m_vertex_shader, 1, &start, nullptr);
  glCompileShader(m_vertex_shader);

  m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  start = &fragment_shader_src[0];
  glShaderSource(m_fragment_shader, 1, &start, nullptr);
  glCompileShader(m_fragment_shader);

  m_program = glCreateProgram();
  glAttachShader(m_program, m_vertex_shader);
  glAttachShader(m_program, m_fragment_shader);
  glLinkProgram(m_program);

  /* Now, that we have the program, query location of shader variables */
  m_u_PVM_location = glGetUniformLocation(m_program, "PVM");
  m_a_position_loc = glGetAttribLocation(m_program, "Position");
  m_a_color_loc = glGetAttribLocation(m_program, "Color");

  /* Then, create primitives and send data to GPU */
  create_primitives_and_send_to_gpu();
  //Initialize the trackball object
  int width;
  int height;
  glfwGetWindowSize(m_window, &width, &height);
  m_trackball = Trackball(width, height, 0.8f);
  m_mouse_drag = false;
  //Initialize some basic rendering state
  glEnable(GL_DEPTH_TEST);
  //Dark gray background color
  glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
}

void OpenGLApplication::create_primitives_and_send_to_gpu() {
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

  m_nTriangles = indices.size() / 3;

  //Create the vertex buffer objects and VAO
  GLuint vbo;
  GLuint indexBuffer;
  glGenVertexArrays(1, &m_vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &indexBuffer);
  // Bind the vao this need to be done before anything
  glBindVertexArray(m_vao);
  //Send data to GPU: first send the vertices
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(m_a_position_loc);
  glVertexAttribPointer(m_a_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        OFFSET_OF(Vertex, position));
  glEnableVertexAttribArray(m_a_color_loc);
  glVertexAttribPointer(m_a_color_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
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

void OpenGLApplication::render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(m_program);
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
  glm::mat4 Cam_Init_Pos = glm::lookAt(camera_position, camera_eye, camera_up);
  // Use trackball to get a rotation applied to the camera's initial pose
  glm::mat4 V = Cam_Init_Pos * m_trackball.getRotation();
  //Projection
  int width;
  int height;
  glfwGetWindowSize(m_window, &width, &height);
  GLfloat aspect = float(width) / float(height);
  // zoom in and out is controlled with the fovy
  GLfloat fovy = TAU / 8.0f + m_zoom_level * (TAU / 50.0f);
  GLfloat zNear = 1.0f;
  GLfloat zFar = 5.0f;
  glm::mat4 P = glm::perspective(fovy, aspect, zNear, zFar);
  /************************************************************************/
  /* Send uniform values to shader                                        */
  /************************************************************************/
  if (m_u_PVM_location != -1) {
    glUniformMatrix4fv(m_u_PVM_location, 1, GL_FALSE, glm::value_ptr(P * V * M));
  }
  /************************************************************************/
  /* Bind buffer object and their corresponding attributes (use VAO)      */
  /************************************************************************/
  glBindVertexArray(m_vao);
  /* Draw */
  const int start_index = 0; //In location corresponding the index array
  glDrawElements(GL_TRIANGLES, 3 * m_nTriangles, GL_UNSIGNED_SHORT,
                 BUFFER_OFFSET(start_index * sizeof(unsigned short)));
  //Unbind and clean
  glBindVertexArray(0);
  glUseProgram(0);
}

void OpenGLApplication::register_glfw_callbacks() {
  // Associate the class instance so, it can be access from
  // inside the static callback functions
  glfwSetWindowUserPointer(m_window, static_cast<void*>(this));
  glfwSetWindowSizeCallback(m_window, resize_callback);
  glfwSetKeyCallback(m_window, key_callback);
  glfwSetMouseButtonCallback(m_window, mouse_button_callback);
  glfwSetCursorPosCallback(m_window, cursor_position_callback);
  glfwSetScrollCallback(m_window, scroll_callback);
}

void key_callback(GLFWwindow* windowPtr, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(windowPtr, GLFW_TRUE);
  }
}

void resize_callback(GLFWwindow* windowPtr, int new_window_width, int new_window_height) {
  glViewport(0, 0, new_window_width, new_window_height);
  auto* app = static_cast<OpenGLApplication*>(glfwGetWindowUserPointer(windowPtr));
  app->m_trackball.setWindowSize(new_window_width, new_window_height);
}

void mouse_button_callback(GLFWwindow* windowPtr, int button, int action, int mods) {
  auto* app = static_cast<OpenGLApplication*>(glfwGetWindowUserPointer(windowPtr));
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    app->m_mouse_drag = true;
    double mouse_x;
    double mouse_y;
    glfwGetCursorPos(windowPtr, &mouse_x, &mouse_y);
    app->m_trackball.startDrag(glm::ivec2(int(mouse_x), int(mouse_y)));
    //These options are provided by GLFW designed for this kind of use case
    //However, I find them more confussing than helping. (Un)comment to test
    //glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //if (raw_mouse_supported) {
    //  glfwSetInputMode(windowPtr, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    //}
  } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
	app->m_trackball.endDrag();
	app->m_mouse_drag = false;
    //These options are provided by GLFW designed for this kind of use case
    //However, I find them more confussing than helping. (Un)comment to test
    //glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //if (raw_mouse_supported) {
    //  glfwSetInputMode(windowPtr, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
    //}
  }
}

void cursor_position_callback(GLFWwindow* windowPtr, double mouse_x, double mouse_y) {
  auto* app = static_cast<OpenGLApplication*>(glfwGetWindowUserPointer(windowPtr));
  if (app->m_mouse_drag) {
	  app->m_trackball.drag(glm::ivec2(int(mouse_x), int(mouse_y)));
  }
}

void scroll_callback(GLFWwindow* windowPtr, double x_offset, double y_offset) {
  auto* app = static_cast<OpenGLApplication*>(glfwGetWindowUserPointer(windowPtr));
  app->m_zoom_level += int(y_offset);
  app->m_zoom_level = glm::clamp(app->m_zoom_level, -5, 5);
}

void glfw_error_callback(int error, const char* description) {
  using std::cerr;
  using std::endl;
  cerr << "GLFW Error: " << description << endl;
}

void OpenGLApplication::free_resources() {
  /* Delete OpenGL program */
  glDetachShader(m_program, m_vertex_shader);
  glDetachShader(m_program, m_fragment_shader);
  glDeleteShader(m_vertex_shader);
  glDeleteShader(m_fragment_shader);
  glDeleteProgram(m_program);
  //Window and context destruction
  glfwDestroyWindow(m_window);
}

