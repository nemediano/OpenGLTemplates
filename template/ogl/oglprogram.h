#ifndef OGL_PROGRAM_H_
#define OGL_PROGRAM_H_

#include <string>

#include <GL/glew.h>

namespace ogl {

class OGLProgram {
//! This class encapsulates an OpenGL shader program
/*!
  This class let you manage an OpenGL shader program. You need to use it as a pointer

  The main purpose is to load shaders from source files and create an OpenGL program
  then you can use this object, to ask for atributes or uniform locations and to bind the
  program
*/
public:
//! Create an OpenGL program from a vertex shader and a fragment shader from source files
/*!
  This constructor will try to read the given input files and load them into member variables
  Then, it will try to compile and link an OpenGL shader program using the corresponding shaders

  In case of failure, it will print a log to default error stream
*/
  OGLProgram(std::string vertex_shader_file_name, std::string fragment_shader_file_name);
//! Create an OpenGL program from a vertex, fragement and geometry shaders from source files
/*!
  This constructor will try to read the given input files and load them into member variables
  Then, it will try to compile and link an OpenGL shader program using the corresponding shaders

  In case of failure, it will print a log to default error stream
*/
  OGLProgram(std::string vertex_shader_file_name,
    std::string fragment_shader_file_name,
    std::string geometry_shader_file_name);
  ~OGLProgram();
//! Bind this OpenGL shader program for render
  void use();
//! Ask for the location of the uniform variable in this OpenGL shader program
  GLint uniformLoc(std::string variable);
//! Ask for the location of the attribute variable in this OpenGL shader program
  GLint attribLoc(std::string variable);
//! Ask for the location of a subroutine uniform in a given shader in this OpenGL shader program
  GLint subroutineUniformLoc(GLenum shadertype, std::string variable);
//! Ask for the location of a subroutine index in a given shader in this OpenGL shader program  
  GLuint subroutineIndexLoc(GLenum shadertype, std::string variable);
//! Query this OpenGL program id (Which serves and a handle for OpenGL)
  GLuint getID();
//! Query if this OpenGL program is OK (i. e: it has been compiled and linked)
  bool isOK();

private:
  std::string m_vertex_shader_source;
  std::string m_fragment_shader_source;
  std::string m_geometry_shader_source;

  bool m_program_ok;
  bool m_has_geometry;
//! Private constructor to prevent autogeneration, cannot create an object like this
  OGLProgram();

  GLuint m_vertex_shader;
  GLuint m_fragment_shader;
  GLuint m_geometry_shader;
  GLuint m_program;

  bool validate_shader_type(const GLenum& shadertype);
  std::string read_shader_from_file(const std::string& source_file);
};

} //namespace ogl

#endif
