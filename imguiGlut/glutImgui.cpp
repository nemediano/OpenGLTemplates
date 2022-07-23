#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
// Dear imgui related headers
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#define GLM_FORCE_PURE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Define helpful macros for handling offsets into buffer objects
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
#define OFFSET_OF(type, member) ((GLvoid*)(offsetof(type, member)))

//Glut callback functions
void display();
void idle();
void reshape(int new_window_width, int new_window_height);
void keyboard(unsigned char key, int mouse_x, int mouse_y);
void mouse_button(int button, int state, int mouse_x, int mouse_y);
void mouse_dragging(int mouse_x, int mouse_y);
// OpenGL's debug logger callback (needs context 4.3 or above)
void opengl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                           GLsizei length, const GLchar *message, const void *userParam);

class OpenGLApplication {
public:
	OpenGLApplication(){};
	void run(int argc, char* argv[]);
	void create_menu();
	//Global variables for the program logic
	int m_nTriangles = 0;
	bool m_rotating = false;
	float m_current_angle = 0.0f;
	int m_last_time = 0;
	bool m_mouse_drag = false;
	std::string m_context_info;
	// Manage the Vertex Buffer Objects using a Vertex Array Object
	GLuint m_vao = 0;
	GLuint m_program = 0;
	GLint m_u_PVM_location = -1;
private:
	// freeglut window id
	GLint m_window = 0;
	// Location for shader variables
	GLint m_a_position_loc = -1;
	GLint m_a_color_loc = -1;
	// OpenGL program handlers
	GLuint m_vertex_shader = 0;
	GLuint m_fragment_shader = 0;

	// Function declarations
	void init_freeglut(int argc, char* argv[]);
	void load_OpenGL();
	void init_program();
	void create_primitives_and_send_to_gpu();
	void setup_menu();
	void create_glut_callbacks();
	// Print shader compilation errors
	void print_shader_log(GLint const shader);
	// get info from the used libraries versions
	std::string enviroment_info();
	void exit_glut();
};

void OpenGLApplication::run(int argc, char* argv[]) {
  init_freeglut(argc, argv);
  load_OpenGL();
  setup_menu();
  init_program();

  create_glut_callbacks();
  glutMainLoop();

  exit_glut();
}

int main(int argc, char* argv[]) {
  OpenGLApplication app;
  try {
	app.run(argc, argv);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
	return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void OpenGLApplication::setup_menu() {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  ImGui::StyleColorsDark();
  ImGui_ImplGLUT_Init();
  //Not sure if we need to call this if we provide replacement
  //for some function but not for all. It works as expected here
  ImGui_ImplGLUT_InstallFuncs();

  const char* glsl_version{"#version 130"};
  ImGui_ImplOpenGL3_Init(glsl_version);

}

void OpenGLApplication::create_menu() {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGLUT_NewFrame();

  // Draw the menu
  ImGui::Begin("Triangle's basic menu");
    ImGui::Text("Options");
    if (ImGui::Checkbox("Rotate", &m_rotating)) { //Imgui's controls return true on interaction
    	m_current_angle = 0.0f;
    }
    if (ImGui::CollapsingHeader("Enviroment info:")) {
      ImGui::Text("%s", m_context_info.c_str());
    }
    if (ImGui::CollapsingHeader("Application stats")) {
      ImGui::Text("Average frame: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
      ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    }
  ImGui::End();
}

void OpenGLApplication::init_freeglut(int argc, char* argv[]) {
  glutInit(&argc, argv);
  glutInitContextVersion(4, 6);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
  glutSetOption(GLUT_MULTISAMPLE, 4);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glutInitWindowSize(900, 600);
  m_window = glutCreateWindow("OpenGL Menu sample - freeglut");
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
	ss << "Glew initialization failed: " << glewGetErrorString(err);
	throw std::runtime_error(ss.str());
  }
  m_context_info = enviroment_info();
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

void OpenGLApplication::create_glut_callbacks() {
  glutSetWindowData(static_cast<void*>(this));
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMotionFunc(mouse_dragging);
  glutMouseFunc(mouse_button);
  glutIdleFunc(idle);
}

void idle() {
  auto* app = static_cast<OpenGLApplication*>(glutGetWindowData());
  int time = glutGet(GLUT_ELAPSED_TIME);
  int elapsed = time - app->m_last_time;
  float delta_seconds = 0.001f * elapsed;
  app->m_last_time = time;
  /*If rotating update angle*/
  if (app->m_rotating) {
    const float speed = 180.0f; //In degrees per second
    app->m_current_angle += delta_seconds * speed;
    if (app->m_current_angle > 360.0f) {
      int quotient = int(app->m_current_angle / 360.0f);
      app->m_current_angle -= quotient * 360.0f;
    }
  }
  glutPostRedisplay();
}

void reshape(int new_window_width, int new_window_height) {
  ImGuiIO& io = ImGui::GetIO();
  io.DisplaySize.x = float(new_window_width);
  io.DisplaySize.y = float(new_window_height);
  glViewport(0, 0, new_window_width, new_window_height);
  glutPostRedisplay();
}

void mouse_button(int button, int state, int mouse_x, int mouse_y) {
  ImGuiIO& io = ImGui::GetIO();
  //Imgui wants this event, since it happen inside the GUI
  if (io.WantCaptureMouse) {
    ImGui_ImplGLUT_MouseFunc(button, state, mouse_x, mouse_y);
    glutPostRedisplay();
    return;
  }
  //The event happen outside the GUI, your application should try to handle it
  auto* app = static_cast<OpenGLApplication*>(glutGetWindowData());
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    //std::cout << "Click in!" << std::endl;
	app->m_mouse_drag = true;
  } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    //std::cout << "Click out!" << std::endl;
	app->m_mouse_drag = false;
  }
}

void mouse_dragging(int mouse_x, int mouse_y) {
  ImGuiIO& io = ImGui::GetIO();
  //Imgui wants this event, since it happen inside the GUI
  if (io.WantCaptureMouse) {
    ImGui_ImplGLUT_MotionFunc(mouse_x, mouse_y);
    glutPostRedisplay();
    return;
  }
  //The event happen outside the GUI, your application should try to handle it
  //if (mouse_drag) {
  //  std::cout << "(" << mouse_x << ", " << mouse_y << ")" << std::endl;
  //}
}

void keyboard(unsigned char key, int mouse_x, int mouse_y) {
  ImGuiIO& io = ImGui::GetIO();
  //Imgui wants this event, since it happen inside the GUI
  if (io.WantCaptureKeyboard) {
    ImGui_ImplGLUT_KeyboardFunc(key, mouse_x, mouse_y);
    glutPostRedisplay();
    return;
  }
  //The event happen outside the GUI, your application should try to handle it
  auto* app = static_cast<OpenGLApplication*>(glutGetWindowData());
  switch(key) {
    case 27: //Esc key
    	glutLeaveMainLoop();
    break;
    case 'r':
    case 'R': {
      app->m_rotating = !app->m_rotating;
      app->m_current_angle = 0.0f;
    } break;
  }
  glutPostRedisplay();
}

void display() {
  auto* app = static_cast<OpenGLApplication*>(glutGetWindowData());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(app->m_program);
  /************************************************************************/
  /* Calculate  Model View Projection Matrices                            */
  /************************************************************************/
  //Math constant equal two PI (Remember, we are in radians)
  const float TAU = 6.28318f;
  //Identity matrix
  glm::mat4 I(1.0f);
  //Model
  glm::mat4 M = I;
  if (app->m_rotating) {
    M = glm::rotate(M, glm::radians(app->m_current_angle), glm::vec3(0.0f, 1.0f, 0.0f));
  }
  //View
  glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 3.5f);
  glm::vec3 camera_eye = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::mat4 V = glm::lookAt(camera_position, camera_eye, camera_up);
  //Projection
  GLfloat aspect = float(glutGet(GLUT_WINDOW_WIDTH)) / float(glutGet(GLUT_WINDOW_HEIGHT));
  GLfloat fovy = TAU / 8.0f;
  GLfloat zNear = 1.0f;
  GLfloat zFar = 5.0f;
  glm::mat4 P = glm::perspective(fovy, aspect, zNear, zFar);
  /************************************************************************/
  /* Send uniform values to shader                                        */
  /************************************************************************/
  if (app->m_u_PVM_location != -1) {
    glUniformMatrix4fv(app->m_u_PVM_location, 1, GL_FALSE, glm::value_ptr(P * V * M));
  }
  /************************************************************************/
  /* Bind buffer object and their corresponding attributes (use VAO)      */
  /************************************************************************/
  glBindVertexArray(app->m_vao);
  /* Draw */
  const int start_index = 0; //In location corresponding the index array
  glDrawElements(GL_TRIANGLES, 3 * app->m_nTriangles, GL_UNSIGNED_SHORT,
                 BUFFER_OFFSET(start_index * sizeof(unsigned short)));
  //Unbind and clean
  glBindVertexArray(0);
  glUseProgram(0);

  /* Render menu after the geometry of our actual app*/
  app->create_menu();
  ImGui::Render(); // Prepare to render our menu, before clearing buffers
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glutSwapBuffers();
}

void OpenGLApplication::exit_glut() {
  /* Release imgui resources */
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGLUT_Shutdown();
  ImGui::DestroyContext();
  /* Delete OpenGL program */
  glDetachShader(m_program, m_vertex_shader);
  glDetachShader(m_program, m_fragment_shader);
  glDeleteShader(m_vertex_shader);
  glDeleteShader(m_fragment_shader);
  glDeleteProgram(m_program);
  /* Delete window (freeglut) */
  glutDestroyWindow(m_window);
}


void opengl_error_callback(GLenum source,
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
  info << "\t\tOpenGL version: " << glGetString(GL_VERSION) << endl;
  info << "\t\tGLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
  info << "\tLibraries:" << endl;
  info << "\t\tGLEW version: " << glewGetString(GLEW_VERSION) << endl;
  int ver = glutGet(GLUT_VERSION);
  info << "\t\tfreeglut version: " << ver / 10000 << "." << (ver / 100) % 100 <<
            "." << ver % 100 << endl;
  info << "\t\tGLM version: " << (GLM_VERSION / 1000) << "." << (GLM_VERSION / 100)
       << "." << (GLM_VERSION % 100 / 10) << "." << (GLM_VERSION % 10) << endl;
  info << "\t\tDear Imgui version: " << ImGui::GetVersion();

  return info.str();
}
