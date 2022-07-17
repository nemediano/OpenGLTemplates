#include <cstdlib>
#include <iostream>
#include <stdexcept>
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

// I have tested in Ubuntu 20.04, using standard installation of glfw, glew and GLM
// i.e installed from official repos
// g++ -std=c++11 -Wall triangle.cpp -o triangle -lGLEW -lGL -lglfw -lm

// Define helpful macros for handling offsets into buffer objects
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
#define OFFSET_OF(type, member) ((GLvoid*)(offsetof(type, member)))

void glfw_error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void resize_callback(GLFWwindow* window, int new_window_width, int new_window_height);
// OpenGL's debug logger callback (needs context 4.3 or above)
void APIENTRY opengl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                            GLsizei length, const GLchar *message, const void *userParam);

class OpenGLApplication {
public:
	OpenGLApplication() {};
	void run();
private:
	// Window and context handle
	GLFWwindow* m_Window = nullptr;
	// Location for shader variables
	GLint m_u_PVM_location = -1;
	GLint m_a_position_loc = -1;
	GLint m_a_color_loc = -1;
	// OpenGL program handlers
	GLuint m_vertex_shader = 0;
	GLuint m_fragment_shader = 0;
	GLuint m_program = 0;
	// Global variables for the program logic
	int m_nTriangles = 0;
	// Manage the Vertex Buffer Objects using a Vertex Array Object
	GLuint m_vao = 0;
	// Function declarations
	void init_glfw();
	void load_OpenGL();
	void init_program();
	void create_primitives_and_send_to_gpu();
	void render();
	void free_resources();
	// Print shader compilation errors
	void print_shader_log(GLint const shader);
	// get info form the used libraries versions
	std::string enviroment_info();
	// Callback related
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

	while (!glfwWindowShouldClose(m_Window)) {
		render();
		glfwSwapBuffers(m_Window);
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
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  m_Window = glfwCreateWindow(512, 512, "Hello world OpenGL - glfw", nullptr, nullptr);
  if (!m_Window) {
    // Window or context creation failed
	std::stringstream ss;
    ss << "OpenGL context not available" << endl;
    glfwTerminate();
    throw std::runtime_error(ss.str());
  }
  // Context setting to happen before OpenGL's extension loader
  glfwMakeContextCurrent(m_Window);
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
  cout << enviroment_info() << endl;
  /************************************************************************/
  /*                    OpenGL Debug context                              */
  /************************************************************************/
  GLint flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(opengl_error_callback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    cout << "OpenGL's debug logger active" << endl;
  } else {
    cout << "OpenGL's debug disabled" << endl;
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

  m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  start = &fragment_shader_src[0];
  glShaderSource(m_fragment_shader, 1, &start, nullptr);

  int status{0};
  glCompileShader(m_vertex_shader);
  glGetShaderiv(m_vertex_shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    cerr << "Vertex shader was not compiled!!" << endl;
    print_shader_log(m_vertex_shader);
  }
  glCompileShader(m_fragment_shader);
  glGetShaderiv(m_fragment_shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    cerr << "Fragment shader was not compiled!!" << endl;
    print_shader_log(m_fragment_shader);
  }
  m_program = glCreateProgram();
  glAttachShader(m_program, m_vertex_shader);
  glAttachShader(m_program, m_fragment_shader);
  //glBindFragDataLocation(program, 0, "fragcolor");
  glLinkProgram(m_program);
  glGetProgramiv(m_program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    cerr << "OpenGL program was not linked!!" << endl;
  }
  /* Now, that we have the program, query location of shader variables */
  m_u_PVM_location = glGetUniformLocation(m_program, "PVM");
  m_a_position_loc = glGetAttribLocation(m_program, "Position");
  m_a_color_loc = glGetAttribLocation(m_program, "Color");

  /* Then, create primitives and send data to GPU */
  create_primitives_and_send_to_gpu();
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
  vertices.push_back({ { -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } }); //0
  vertices.push_back({ {  1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } }); //1
  vertices.push_back({ {  0.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } }); //2

  std::vector<unsigned short> indices;
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);

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
  glm::mat4 V = glm::lookAt(camera_position, camera_eye, camera_up);
  //Projection
  int width;
  int height;
  glfwGetWindowSize(m_Window, &width, &height);
  GLfloat aspect = float(width) / float(height);
  GLfloat fovy = TAU / 8.0f;
  GLfloat zNear = 1.0f;
  GLfloat zFar = 4.0f;
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
  glfwSetWindowUserPointer(m_Window, static_cast<void*>(this));
  glfwSetWindowSizeCallback(m_Window, resize_callback);
  glfwSetKeyCallback(m_Window, key_callback);
}

void key_callback(GLFWwindow* windowPtr, int key, int scancode, int action, int mods) {
  // Get reference to the main class instance
  // auto* app = static_cast<OpenGLApplication*>(glfwGetWindowUserPointer(windowPtr));
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(windowPtr, GLFW_TRUE);
  }
}

void resize_callback(GLFWwindow* windowPtr, int new_window_width, int new_window_height) {
  glViewport(0, 0, new_window_width, new_window_height);
}

void glfw_error_callback(int error, const char* description) {
  using std::cerr;
  using std::endl;
  std::stringstream ss;
  ss << "GLFW Error: " << description << endl;
  throw std::runtime_error(ss.str());
}

void OpenGLApplication::free_resources() {
  /* Delete OpenGL program */
  glDetachShader(m_program, m_vertex_shader);
  glDetachShader(m_program, m_fragment_shader);
  glDeleteShader(m_vertex_shader);
  glDeleteShader(m_fragment_shader);
  glDeleteProgram(m_program);
  //Window and context destruction
  glfwDestroyWindow(m_Window);
}

void APIENTRY opengl_error_callback(GLenum source,
                                    GLenum type,
                                    GLuint id,
                                    GLenum severity,
                                    GLsizei length,
                                    const GLchar *message,
                                    const void *userParam) {
  using std::cout;
  using std::endl;
  using std::string;
  using std::to_string;
  // ignore non-significant error/warning codes
  if(id == 131169 || id == 131185 || id == 131218 || id == 131204) {
    return;
  }
  // Gather all the info in strings
  string source_str;
   switch (source) {
     case GL_DEBUG_SOURCE_API:             source_str = "API"; break;
     case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   source_str = "Window System"; break;
     case GL_DEBUG_SOURCE_SHADER_COMPILER: source_str = "Shader Compiler"; break;
     case GL_DEBUG_SOURCE_THIRD_PARTY:     source_str = "Third Party"; break;
     case GL_DEBUG_SOURCE_APPLICATION:     source_str = "Application"; break;
     case GL_DEBUG_SOURCE_OTHER:           source_str = "Other"; break;
   }

  string type_str;
  switch (type) {
    case GL_DEBUG_TYPE_ERROR:               type_str = "Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_str = "Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  type_str = "Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         type_str = "Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         type_str = "Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              type_str = "Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          type_str = "Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           type_str = "Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               type_str = "Other"; break;
  }

  string severity_str;
  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:         severity_str = "high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       severity_str = "medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          severity_str = "low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: severity_str = "notification"; break;
  }
  // format message
  string msg = string("OpenGL error (") + to_string(id) + string("): ") + message + string("\n") +
               string("Source: ") + source_str + string("\n") +
               string("Type: ") + type_str + string("\n") +
               string("Severity: ") + severity_str + string("\n");
  // print error to console
  cout << msg << endl;
}

void OpenGLApplication::print_shader_log(GLint const shader) {
  using std::cerr;
  using std::endl;

  int log_length = 0;
  int chars_written = 0;

  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

  if (log_length > 0) {
    GLchar *log = new GLchar[log_length];
    glGetShaderInfoLog(shader, log_length, &chars_written, log);
    cerr << "Shader Info Log:" << endl;
    cerr << log << endl;
    delete [] log;
  }
}

std::string OpenGLApplication::enviroment_info() {
  using std::endl;
  std::stringstream info;

  info << "Hardware: " << endl;
  info << "\tVendor: " << glGetString(GL_VENDOR) << endl;
  info << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
  info << "Software: " << endl;
  info << "\tDriver:" << endl;
  info << "\t\tUsing OpenGL version: " << glGetString(GL_VERSION) << endl;
  info << "\t\tUsing GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
  info << "\tLibraries:" << endl;
  info << "\t\tUsing GLEW version: " << glewGetString(GLEW_VERSION) << endl;
  info << "\t\tUsing GLFW version: " << GLFW_VERSION_MAJOR << "." << GLFW_VERSION_MINOR
       << "." << GLFW_VERSION_REVISION << endl;
  info << "\t\tUsing GLM version: " << (GLM_VERSION / 1000) << "." << (GLM_VERSION / 100)
       << "." << (GLM_VERSION % 100 / 10) << "." << (GLM_VERSION % 10) << endl;

  return info.str();
}
