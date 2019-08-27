#ifndef OGL_HELPERS_H_
#define OGL_HELPERS_H_

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace ogl {

void printShaderInfoLog(GLuint object);
void printProgramInfoLog(GLuint object);
bool getErrorLog();

std::string getOpenGLInfo();
bool framebufferStatus();

void APIENTRY opengl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                            GLsizei length, const GLchar *message, const void *userParam);

//Debug functions
void printMatrix(glm::mat4 A);
void printMatrix(glm::mat3 A);
void printVector(glm::vec4 u);
void printVector(glm::vec3 u);

} // namespace ogl

#endif