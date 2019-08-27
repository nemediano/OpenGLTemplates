#ifndef OGL_PROGRAM_H_
#define OGL_PROGRAM_H_

#include <string>

#include <GL/glew.h>

namespace ogl {

class OGLProgram {

public:
  OGLProgram(std::string vertex_shader_file_name, std::string fragment_shader_file_name);
  OGLProgram(std::string vertex_shader_file_name,
    std::string fragment_shader_file_name,
    std::string geometry_shader_file_name);
  ~OGLProgram();

  void use();

  GLint uniformLoc(std::string variable);
  GLint attribLoc(std::string variable);
  GLint subroutineUniformLoc(GLenum shadertype, std::string variable);
  GLuint subroutineIndexLoc(GLenum shadertype, std::string variable);
  GLuint getID();

  bool isOK();

private:
  std::string m_vertex_shader_source;
  std::string m_fragment_shader_source;
  std::string m_geometry_shader_source;

  bool m_program_ok;
  bool m_has_geometry;

  OGLProgram();

  GLuint m_vertex_shader;
  GLuint m_fragment_shader;
  GLuint m_geometry_shader;
  GLuint m_program;

  bool validate_shader_type(const GLenum& shadertype);
};

} //namespace ogl

#endif
