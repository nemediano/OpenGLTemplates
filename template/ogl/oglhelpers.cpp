#include <iostream>

#include "oglhelpers.h"

namespace ogl {

using namespace std;

void printShaderInfoLog(GLuint object) {
  int infologLength = 0;
  int charsWritten = 0;
  char *infoLog;

  glGetShaderiv(object, GL_INFO_LOG_LENGTH, &infologLength);

  if (infologLength > 0) {
    infoLog = new char[infologLength];
    glGetShaderInfoLog(object, infologLength, &charsWritten, infoLog);
    cout << infoLog << endl;
    delete[] infoLog;
  }
}

void printProgramInfoLog(GLuint object) {
  int infologLength = 0;
  int charsWritten = 0;
  char *infoLog;

  glGetProgramiv(object, GL_INFO_LOG_LENGTH, &infologLength);

  if (infologLength > 0) {
    infoLog = new char[infologLength];
    glGetProgramInfoLog(object, infologLength, &charsWritten, &infoLog[0]);
    cout << infoLog << endl;
    delete[] infoLog;
  }
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

bool getErrorLog() {
  GLint flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(opengl_error_callback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    cout << "OpenGL's debug logger active" << endl;
    return true;
  } else {
    cout << "OpenGL's debug disabled" << endl;
    return false;
  }
}

string getOpenGLInfo() {
  string log;
  cout << "Hardware specification: " << endl;
  cout << "Vendor: " << glGetString(GL_VENDOR) << endl;
  cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
  cout << "Software specification: " << endl;
  cout << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;
  cout << "Using OpenGL " << glGetString(GL_VERSION) << endl;
  cout << "Using GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

  return log;
}

bool framebufferStatus() {
  GLenum status;
  status = (GLenum)glCheckFramebufferStatus(GL_FRAMEBUFFER);
  switch (status) {
    case GL_FRAMEBUFFER_COMPLETE:
      return true;
    break;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      cerr << "Framebuffer incomplete, incomplete attachment" << endl;
      return false;
    break;

    case GL_FRAMEBUFFER_UNSUPPORTED:
      cerr << "Unsupported framebuffer format" << endl;
      return false;
    break;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      cerr << "Framebuffer incomplete, missing attachment" << endl;
      return false;
    break;

    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
      cerr << "Framebuffer incomplete, missing draw buffer" << endl;
      return false;
    break;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
      cerr << "Framebuffer incomplete, missing read buffer" << endl;
      return false;
    break;
  }

  return false;
}

void printMatrix(glm::mat4 A) {
  cout << A[0][0] << "\t" << A[0][1] << "\t" << A[0][2] << "\t" << A[0][3] << endl;
  cout << A[1][0] << "\t" << A[1][1] << "\t" << A[1][2] << "\t" << A[1][3] << endl;
  cout << A[2][0] << "\t" << A[2][1] << "\t" << A[2][2] << "\t" << A[2][3] << endl;
  cout << A[3][0] << "\t" << A[3][1] << "\t" << A[3][2] << "\t" << A[3][3] << endl;
}

void printMatrix(glm::mat3 A) {
  cout << A[0][0] << "\t" << A[0][1] << "\t" << A[0][2] << endl;
  cout << A[1][0] << "\t" << A[1][1] << "\t" << A[1][2] << endl;
  cout << A[2][0] << "\t" << A[2][1] << "\t" << A[2][2] << endl;
}

void printVector(glm::vec4 u) {
  cout << "(" << u.x << ", " << u.y << ", " << u.z << ", " << u.w << ")" << endl;
}

void printVector(glm::vec3 u) {
  cout << "(" << u.x << ", " << u.y << ", " << u.z << ")" << endl;
}

} // namespace ogl