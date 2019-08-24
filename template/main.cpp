#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
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

#include "ui/trackball.h"

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
GLuint vertex_shader = 0;
GLuint fragment_shader = 0;
GLuint program = 0;
// Global variables for the program logic
int nTriangles = 0;
bool rotating = false;
float current_angle = 0.0f;
double last_time = 0.0;
bool mouse_drag = false;
int zoom_level = 0;
std::string context_info;
// Manage the Vertex Buffer Objects using a Vertex Array Object
GLuint vao;
// Keep track of the window state
class WindowState {
public:
  GLFWmonitor* monitorPtr;
  int x_pos;
  int y_pos;
  int width;
  int height;
};
WindowState window_state;
ui::Trackball ball;
// Function declarations
void init_glfw();
void load_OpenGL();
void init_program();
void create_primitives_and_send_to_gpu();
void setup_menu();
void create_menu();
void render();
void update();
void free_resources();
void change_window_mode();
// GLFW related callbacks
void register_glfw_callbacks();
void glfw_error_callback(int error, const char* description);
void key_callback(GLFWwindow* windowPtr, int key, int scancode, int action, int mods);
void resize_callback(GLFWwindow* windowPtr, int new_window_width, int new_window_height);
void mouse_button_callback(GLFWwindow* windowPtr, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* windowPtr, double xpos, double ypos);
void scroll_callback(GLFWwindow* windowPtr, double x_offset, double y_offset);
// OpenGL's debug logger callback (needs context 4.3 or above)
void APIENTRY opengl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                            GLsizei length, const GLchar *message, const void *userParam);
// Print shader compilation errors
void print_shader_log(GLint const shader);
// get info from the used libraries versions
std::string enviroment_info();

int main (int argc, char* argv[]) {
  init_glfw();
  load_OpenGL();
  setup_menu();
  init_program();
  register_glfw_callbacks();

  while (!glfwWindowShouldClose(window)) {
    create_menu();
    render();
    update();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  free_resources();

  return EXIT_SUCCESS;
}

void setup_menu() {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  const char* glsl_version{"#version 130"};
  ImGui_ImplOpenGL3_Init(glsl_version);
}

void create_menu() {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  // Draw the menu
  ImGui::Begin("Triangle's basic menu");
    ImGui::Text("Options");
    if (ImGui::Checkbox("Rotate", &rotating)) { //Imgui's controls return true on interaction
      current_angle = 0.0f;
    }
    if (ImGui::CollapsingHeader("Enviroment info:")) {
      ImGui::Text("%s", context_info.c_str());
    }
    if (ImGui::CollapsingHeader("Application stats")) {
      ImGui::Text("Average frame: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
      ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    }
  ImGui::End();
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

  window = glfwCreateWindow(900, 600, "OpenGL Template", nullptr, nullptr);
  if (!window) {
    // Window or context creation failed
    cerr << "OpenGL context not available" << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  // Save window state
  window_state.monitorPtr = glfwGetPrimaryMonitor();
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
  context_info = enviroment_info();
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

  int status{0};
  glCompileShader(vertex_shader);
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    cerr << "Vertex shader was not compiled!!" << endl;
    print_shader_log(vertex_shader);
  }
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    cerr << "Fragment shader was not compiled!!" << endl;
    print_shader_log(fragment_shader);
  }
  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  //glBindFragDataLocation(program, 0, "fragcolor");
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    cerr << "OpenGL program was not linked!!" << endl;
  }
  /* Now, that we have the program, query location of shader variables */
  u_PVM_location = glGetUniformLocation(program, "PVM");
  a_position_loc = glGetAttribLocation(program, "Position");
  a_color_loc = glGetAttribLocation(program, "Color");

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
  glfwGetWindowSize(window, &width, &height);
  ball.setWindowSize(width, height);
}

void create_primitives_and_send_to_gpu() {
  //Create primitives
  struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
  };

  std::vector<Vertex> vertices;
  vertices.push_back({ { -1.0f, -1.0f, -1.0f }, { 0.15f, 0.15f, 0.15f } }); //0
  vertices.push_back({ { -1.0f, -1.0f,  1.0f }, { 0.15f, 0.15f, 0.85f } }); //1
  vertices.push_back({ { -1.0f,  1.0f, -1.0f }, { 0.15f, 0.85f, 0.15f } }); //2
  vertices.push_back({ { -1.0f,  1.0f,  1.0f }, { 0.15f, 0.85f, 0.85f } }); //3
  vertices.push_back({ {  1.0f, -1.0f, -1.0f }, { 0.85f, 0.15f, 0.15f } }); //4
  vertices.push_back({ {  1.0f, -1.0f,  1.0f }, { 0.85f, 0.15f, 0.85f } }); //5
  vertices.push_back({ {  1.0f,  1.0f, -1.0f }, { 0.85f, 0.85f, 0.15f } }); //7
  vertices.push_back({ {  1.0f,  1.0f,  1.0f }, { 0.85f, 0.85f, 0.85f } }); //8

  std::vector<unsigned short> indices;
  // Top
  indices.push_back(2);
  indices.push_back(7);
  indices.push_back(6);
  indices.push_back(2);
  indices.push_back(3);
  indices.push_back(7);
  // Buttom
  indices.push_back(0);
  indices.push_back(4);
  indices.push_back(5);
  indices.push_back(0);
  indices.push_back(5);
  indices.push_back(1);
  // Left
  indices.push_back(0);
  indices.push_back(3);
  indices.push_back(2);
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(3);
  // Right
  indices.push_back(7);
  indices.push_back(4);
  indices.push_back(6);
  indices.push_back(7);
  indices.push_back(5);
  indices.push_back(4);
  // Front
  indices.push_back(3);
  indices.push_back(5);
  indices.push_back(7);
  indices.push_back(3);
  indices.push_back(1);
  indices.push_back(5);
  // Back
  indices.push_back(2);
  indices.push_back(6);
  indices.push_back(4);
  indices.push_back(2);
  indices.push_back(4);
  indices.push_back(0);

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
  ImGui::Render(); // Prepare to render our menu, before clearing buffers
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
  if (rotating) {
    M = glm::rotate(M, glm::radians(current_angle), glm::vec3(0.0f, 1.0f, 0.0f));
  }
  //View
  glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 3.5f);
  glm::vec3 camera_eye = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::mat4 Cam_Init_Pos = glm::lookAt(camera_position, camera_eye, camera_up);
  // Use trackball to get a rotation applied to the camera's initial pose
  glm::mat4 V = Cam_Init_Pos * ball.getRotation();
  //Projection
  int width;
  int height;
  glfwGetWindowSize(window, &width, &height);
  GLfloat aspect = float(width) / float(height);
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
  /* Render menu after the geometry of our actual app*/
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void update() {
  double time = glfwGetTime();
  double elapsed = time - last_time;
  last_time = time;
  /*If rotating update angle*/
  if (rotating) {
    const float speed = 180.0f; //In degrees per second
    current_angle += float(elapsed) * speed;
    if (current_angle > 360.0f) {
      int quotient = int(current_angle / 360.0f);
      current_angle -= quotient * 360.0f;
    }
  }
}

void register_glfw_callbacks() {
  glfwSetWindowSizeCallback(window, resize_callback);
  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);
  glfwSetScrollCallback(window, scroll_callback);
}

void key_callback(GLFWwindow* windowPtr, int key, int scancode, int action, int mods) {
  ImGuiIO& io = ImGui::GetIO();
  //Imgui wants this event, since it happen inside the GUI
  if (io.WantCaptureKeyboard) {
    return;
  }
  //The event happen outside the GUI, your application should try to handle it
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(windowPtr, 1);
  } else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
    rotating = !rotating;
    current_angle = 0.0f;
  } else if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
    change_window_mode();
  }
}

void mouse_button_callback(GLFWwindow* windowPtr, int button, int action, int mods) {
  ImGuiIO& io = ImGui::GetIO();
  //Imgui wants this event, since it happen inside the GUI
  if (io.WantCaptureMouse) {
    return;
  }
  //The event happen outside the GUI, your application should try to handle it
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    mouse_drag = true;
    double mouse_x;
    double mouse_y;
    glfwGetCursorPos(windowPtr, &mouse_x, &mouse_y);
    ball.startDrag(glm::ivec2(int(mouse_x), int(mouse_y)));
  } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    ball.endDrag();
    mouse_drag = false;
  }
}

void cursor_position_callback(GLFWwindow* windowPtr, double mouse_x, double mouse_y) {
  ImGuiIO& io = ImGui::GetIO();
  //Imgui wants this event, since it happen inside the GUI
  if (io.WantCaptureMouse) {
    return;
  }

  if (mouse_drag) {
    ball.drag(glm::ivec2(int(mouse_x), int(mouse_y)));
  }
}

void scroll_callback(GLFWwindow* windowPtr, double x_offset, double y_offset) {
  zoom_level += int(y_offset);
  zoom_level = glm::clamp(zoom_level, -5, 5);
}

void resize_callback(GLFWwindow* windowPtr, int new_window_width, int new_window_height) {
  glViewport(0, 0, new_window_width, new_window_height);
  ball.setWindowSize(new_window_width, new_window_height);
}

void glfw_error_callback(int error, const char* description) {
  using std::cerr;
  using std::endl;
  cerr << "GLFW Error: " << description << endl;
}

void change_window_mode() {
  //Windowed windows return null as their monitor
  GLFWmonitor* monitor = glfwGetWindowMonitor(window);

  if (monitor) { // Go to windowed mode
    window_state.monitorPtr = monitor;
    glfwSetWindowMonitor(window, nullptr, window_state.x_pos, window_state.y_pos,
        window_state.width, window_state.height, 0);
  } else { // go to full screen
    glfwGetWindowPos(window, &window_state.x_pos, &window_state.y_pos);
    glfwGetWindowSize(window, &window_state.width, &window_state.height);
    const GLFWvidmode* mode = glfwGetVideoMode(window_state.monitorPtr);
    glfwSetWindowMonitor(window, window_state.monitorPtr, 0, 0, mode->width,
        mode->height, mode->refreshRate);
  }
}

void free_resources() {
  /* Release imgui resources */
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  /* Delete OpenGL program */
  glDetachShader(program, vertex_shader);
  glDetachShader(program, fragment_shader);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  glDeleteProgram(program);
  //Window and context destruction
  glfwDestroyWindow(window);
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

void print_shader_log(GLint const shader) {
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

std::string enviroment_info() {
  using std::endl;
  std::stringstream info;

  info << "Hardware: " << endl;
  info << "\tVendor: " << glGetString(GL_VENDOR) << endl;
  info << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
  info << "Software: " << endl;
  info << "\tDriver:" << endl;
  info << "\t\tOpenGL version: " << glGetString(GL_VERSION) << endl;
  info << "\t\tGLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
  info << "\tLibraries:" << endl;
  info << "\t\tGLEW version: " << glewGetString(GLEW_VERSION) << endl;
  info << "\t\tGLFW version: " << GLFW_VERSION_MAJOR << "." << GLFW_VERSION_MINOR
       << "." << GLFW_VERSION_REVISION << endl;
  info << "\t\tGLM version: " << (GLM_VERSION / 1000) << "." << (GLM_VERSION / 100)
       << "." << (GLM_VERSION % 100 / 10) << "." << (GLM_VERSION % 10) << endl;
  info << "\t\tDear Imgui version: " << ImGui::GetVersion();

  return info.str();
}
