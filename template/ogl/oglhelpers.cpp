#include <iostream>
#include <sstream>
#include <assimp/version.h>
#include <FreeImage.h>

#include "../imgui/imgui.h"

#include "oglhelpers.h"

namespace ogl {

using namespace std;

void printShaderInfoLog(GLuint object) {
  int infologLength = 0;
  int charsWritten = 0;
  // Query the dirver to see if there is an error log
  glGetShaderiv(object, GL_INFO_LOG_LENGTH, &infologLength);
  // If it is, then ask for it and print it to output stream
  if (infologLength > 0) {
    GLchar* infoLog = new char[infologLength];
    glGetShaderInfoLog(object, infologLength, &charsWritten, infoLog);
    cout << infoLog << endl;
    delete[] infoLog;
  }
}

void printProgramInfoLog(GLuint object) {
  int infologLength = 0;
  int charsWritten = 0;
  // Query the dirver to see if there is an error log
  glGetProgramiv(object, GL_INFO_LOG_LENGTH, &infologLength);
  // If it is, then ask for it and print it to output stream
  if (infologLength > 0) {
    GLchar* infoLog = new char[infologLength];
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
  // Ignore non-significant error/warning codes
  if(id == 131169 || id == 131185 || id == 131218 || id == 131204) {
    return;
  }
  // Gather all the info in strings
  // Source
  string source_str;
   switch (source) {
     case GL_DEBUG_SOURCE_API:             source_str = "API"; break;
     case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   source_str = "Window System"; break;
     case GL_DEBUG_SOURCE_SHADER_COMPILER: source_str = "Shader Compiler"; break;
     case GL_DEBUG_SOURCE_THIRD_PARTY:     source_str = "Third Party"; break;
     case GL_DEBUG_SOURCE_APPLICATION:     source_str = "Application"; break;
     case GL_DEBUG_SOURCE_OTHER:           source_str = "Other"; break;
   }
  // Type
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
  // Severity
  string severity_str;
  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:         severity_str = "high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       severity_str = "medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          severity_str = "low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: severity_str = "notification"; break;
  }
  // format the message
  string msg = string("OpenGL error (") + to_string(id) + string("): ") + message + string("\n") +
               string("Source: ") + source_str + string("\n") +
               string("Type: ") + type_str + string("\n") +
               string("Severity: ") + severity_str + string("\n");
  // print the error to console
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
    return true;
  } else {
    return false;
  }
}

std::string enviroment_info() {
  using std::endl;
  std::stringstream info;

  info << "Hardware: " << endl;
  info << "  Vendor: " << get_version(VENDOR) << endl;
  info << "  Renderer: " << get_version(GPU) << endl;
  info << "Software: " << endl;
  info << "  Driver:" << endl;
  info << "  OpenGL version: " << get_version(OPENGL) << endl;
  info << "  GLSL version: " << get_version(GLSL) << endl;
  info << "  Libraries:" << endl;
  info << "    GLEW version: " << get_version(GLEW) << endl;
  info << "    GLFW version: " << get_version(GLFW) << endl;
  info << "    GLM version: " << get_version(GLM) << endl;
  info << "    Dear Imgui version: " << get_version(IMGUI) << endl;
  info << "    Assimp: " << get_version(ASSIMP) << endl;
  info << "    FreeImage: " << get_version(FREEIMAGE);
  return info.str();
}

std::string get_version(const VerEnum& field) {
  std::stringstream ver;

  switch (field) {
    case VENDOR:
      ver << glGetString(GL_VENDOR);
    break;

    case GPU:
      ver << glGetString(GL_RENDERER);
    break;

    case OPENGL:
      ver << glGetString(GL_VERSION);
    break;

    case GLSL:
      ver << glGetString(GL_SHADING_LANGUAGE_VERSION);
    break;

    case GLEW:
      ver << glewGetString(GLEW_VERSION);
    break;

    case GLFW:
      ver << GLFW_VERSION_MAJOR << "." << GLFW_VERSION_MINOR << "." << GLFW_VERSION_REVISION;
    break;

    case GLM:
      ver << (GLM_VERSION / 1000) << "." << (GLM_VERSION / 100) << "." << (GLM_VERSION % 100 / 10)
          << "." << (GLM_VERSION % 10);
    break;

    case IMGUI:
      ver << ImGui::GetVersion();
    break;

    case ASSIMP:
      ver << aiGetVersionMajor() << "." << aiGetVersionMinor() << "." << aiGetVersionRevision();
    break;
    
    case FREEIMAGE:
      ver << FREEIMAGE_MAJOR_VERSION << "." << FREEIMAGE_MINOR_VERSION << "." << FREEIMAGE_RELEASE_SERIAL;
    break;

    default:
      ver << "Unknow";
  }

  return ver.str();
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