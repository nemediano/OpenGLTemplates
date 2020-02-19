#ifndef OGL_HELPERS_H_
#define OGL_HELPERS_H_

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// OpenGL helper debug functions
namespace ogl {
//! Print the given shader error log to default output stream
void printShaderInfoLog(GLuint object);
//! Print the given OpenGL shader program error log to default output stream
/*!
  Was designed to print error in the link process. However,
  Note that if a shader failed to compile, this log will contain both the shader error
  and the link error
*/
void printProgramInfoLog(GLuint object);
//! Tries to enable the OpenGL debugger extension
/*!
  Try to connect the current OpenGL context to the OpenGL debug extension. In case of success
  returns true. In case of failure returns false.

  In order to activate the debug logger the extension needs to be available and the context
  need to have made aware of this at creation (GLFW manages this logic) as a context creation hint
  Also, Debug extension is only available in context version 4.3 and above

  If the debug logger is available, then a callback function is registered. This is the most
  powerfull (and complete) debug logger for OpenGL. However, keep in mind that depends on the
  driver. In other words: different drivers give different errors.

  The callback function connected is: opengl_error_callback wich is declared below
*/
bool getErrorLog();
//! Return a string that contains the enviroment info
/*!
 This string includes in the verision of OpenGL, the driver, the kind GPU and
 the versions of all the third party libraries used in the template
*/
std::string enviroment_info();
//! Return a string that contains specific field info
/*!
 This method returns a strig with the version of a given field
*/
enum VerEnum {VENDOR, GPU, OPENGL, GLSL, GLEW, GLFW, GLM, IMGUI, ASSIMP, FREEIMAGE};
std::string get_version(const VerEnum& field);
//! Print the given shader error log to default output stream
bool framebufferStatus();
//! OpenGL debug error log callback function
void APIENTRY opengl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                            GLsizei length, const GLchar *message, const void *userParam);
//! Print a glm matrix to the default output strem in human readable format
void printMatrix(glm::mat4 A);
//! Print a glm matrix to the default output strem in human readable format
void printMatrix(glm::mat3 A);
//! Print a glm vector to the default output strem in human readable format
void printVector(glm::vec4 u);
//! Print a glm vector to the default output strem in human readable format
void printVector(glm::vec3 u);
} // namespace ogl

// Define helpful macros for handling offsets into buffer objects
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
#define OFFSET_OF(type, member) ((GLvoid*)(offsetof(type, member)))

#endif