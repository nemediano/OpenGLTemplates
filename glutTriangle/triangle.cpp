#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>

#define GLM_FORCE_PURE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// I have compiled in Ubuntu 18.04, usign standar installation of freeglut, glew and GLM
// i.e installed from official repos
// g++ -Wall triangle.cpp -o triangle -lGLEW -lGL -lglut -lm

// Define helpful macros for handling offsets into buffer objects
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
#define OFFSET_OF(type, member) ((GLvoid*)(offsetof(type, member)))
// freeglut window id
GLint window = 0;
// Location for shader variables
GLint u_PVM_location = -1;
GLint a_position_loc = -1;
GLint a_color_loc = -1;
// OpenGL program handlers
GLuint vertex_shader = 0;
GLuint fragment_shader = 0;
GLuint program = 0;
//Global variables for the program logic
int nTriangles = 0;
// Manage the Vertex Buffer Objects using a Vertex Array Object
GLuint vao;
// Function declarations
void init_freeglut();
void load_OpenGL();
void init_program();
void create_primitives_and_send_to_gpu();
void create_glut_callbacks();
void exit_glut();
//Glut callback functions
void display();
void idle();
void reshape(int new_window_width, int new_window_height);
void keyboard(unsigned char key, int mouse_x, int mouse_y);
// OpenGL's debug logger callback (needs context 4.3 or above)
void opengl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                           GLsizei length, const GLchar *message, const void *userParam);
// Print shader compilation errors
void print_shader_log(GLint const shader);
// get infor from the used libraries versions
std::string enviroment_info();

int main(int argc, char* argv[]) {
  glutInit(&argc, argv);
  init_freeglut();
  load_OpenGL();
  init_program();

  create_glut_callbacks();
  glutMainLoop();

  return EXIT_SUCCESS;
}

void init_freeglut() {
  glutInitContextVersion(4, 5);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
  glutSetOption(GLUT_MULTISAMPLE, 4);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glutInitWindowSize(512, 512);
  window = glutCreateWindow("Hello world OpenGL - freeglut");
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
  glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
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

void create_primitives_and_send_to_gpu() {
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

void create_glut_callbacks() {
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(idle);
}

void idle() {
  glutPostRedisplay();
}

void reshape(int new_window_width, int new_window_height) {
  glViewport(0, 0, new_window_width, new_window_height);
  glutPostRedisplay();
}

void keyboard(unsigned char key, int mouse_x, int mouse_y) {
  switch(key) {
    case 27: //Esc key
      exit_glut();
    break;
  }
  glutPostRedisplay();
}

void display() {
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
  //Projection
  GLfloat aspect = float(glutGet(GLUT_WINDOW_WIDTH)) / float(glutGet(GLUT_WINDOW_HEIGHT));
  GLfloat fovy = TAU / 8.0f;
  GLfloat zNear = 1.0f;
  GLfloat zFar = 4.0f;
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

  glutSwapBuffers();
}

void exit_glut() {
  /* Delete OpenGL program */
  glDetachShader(program, vertex_shader);
  glDetachShader(program, fragment_shader);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  glDeleteProgram(program);
  /* Delete window (freeglut) */
  glutDestroyWindow(window);
  exit(EXIT_SUCCESS);
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
  info << "\t\tUsing OpenGL version: " << glGetString(GL_VERSION) << endl;
  info << "\t\tUsing GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
  info << "\tLibraries:" << endl;
  info << "\t\tUsing GLEW version: " << glewGetString(GLEW_VERSION) << endl;
  int ver = glutGet(GLUT_VERSION);
  info << "\t\tUsing freeglut version: " << ver / 10000 << "." << (ver / 100) % 100 <<
            "." << ver % 100 << endl;
  info << "\t\tUsing GLM version: " << (GLM_VERSION / 1000) << "." << (GLM_VERSION / 100)
       << "." << (GLM_VERSION % 100 / 10) << "." << (GLM_VERSION % 10) << endl;

  return info.str();
}
