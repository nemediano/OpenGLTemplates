#include <fstream>
#include <iostream>

#include "oglprogram.h"
#include "oglhelpers.h"


namespace ogl {

using namespace std;

OGLProgram::OGLProgram(string vertex_shader_file_name, string fragment_shader_file_name) {
  /* Cool preprocessing */
  m_program_ok = true;
  ifstream input_vertex_shader_file(vertex_shader_file_name, ios::in | ios::binary);
  if (input_vertex_shader_file) {
    input_vertex_shader_file.seekg(0, ios::end);
    m_vertex_shader_source.resize(static_cast<unsigned int>(input_vertex_shader_file.tellg()));
    input_vertex_shader_file.seekg(0, ios::beg);
    input_vertex_shader_file.read(&m_vertex_shader_source[0], m_vertex_shader_source.size());
    input_vertex_shader_file.close();
  }
  else {
    cerr << "Could not open vertex shader file at: " << vertex_shader_file_name << endl;
    m_program_ok = false;
  }

  ifstream input_fragment_shader_file(fragment_shader_file_name, ios::in | ios::binary);
  if (input_fragment_shader_file) {
    input_fragment_shader_file.seekg(0, ios::end);
    m_fragment_shader_source.resize(static_cast<unsigned int>(input_fragment_shader_file.tellg()));
    input_fragment_shader_file.seekg(0, ios::beg);
    input_fragment_shader_file.read(&m_fragment_shader_source[0], m_fragment_shader_source.size());
    input_fragment_shader_file.close();
  }
  else {
    cerr << "Could not open fragment shader file at: " << fragment_shader_file_name << endl;
    m_program_ok = false;
  }

  /* or not */
  m_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  const char* start = &m_vertex_shader_source[0];
  glShaderSource(m_vertex_shader, 1, &start, NULL);

  m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  start = &m_fragment_shader_source[0];
  glShaderSource(m_fragment_shader, 1, &start, NULL);

  try {
    int status;
    glCompileShader(m_vertex_shader);
    glGetShaderiv(m_vertex_shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
      throw m_vertex_shader;
    }

    glCompileShader(m_fragment_shader);
    glGetShaderiv(m_fragment_shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
      throw m_fragment_shader;
    }
  }
  catch (GLuint bad_shader) {
    printShaderInfoLog(bad_shader);
    m_program_ok = false;
  }

  m_program = glCreateProgram();
  try {
    int status;
    glAttachShader(m_program, m_vertex_shader);
    glAttachShader(m_program, m_fragment_shader);
    glLinkProgram(m_program);

    glGetProgramiv(m_program, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
      throw m_program;
    }

  }
  catch (GLuint bad_program) {
    printProgramInfoLog(bad_program);
    m_program_ok = false;
  }
}

OGLProgram::OGLProgram(std::string vertex_shader_file_name, std::string fragment_shader_file_name,
             std::string geometry_shader_file_name) {
  /* Cool preprocessing */
  m_program_ok = true;
  ifstream input_vertex_shader_file(vertex_shader_file_name, ios::in | ios::binary);
  if (input_vertex_shader_file) {
    input_vertex_shader_file.seekg(0, ios::end);
    m_vertex_shader_source.resize(static_cast<unsigned int>(input_vertex_shader_file.tellg()));
    input_vertex_shader_file.seekg(0, ios::beg);
    input_vertex_shader_file.read(&m_vertex_shader_source[0], m_vertex_shader_source.size());
    input_vertex_shader_file.close();
  } else {
    cerr << "Could not open vertex shader file at: " << vertex_shader_file_name << endl;
    m_program_ok = false;
  }

  ifstream input_fragment_shader_file(fragment_shader_file_name, ios::in | ios::binary);
  if (input_fragment_shader_file) {
    input_fragment_shader_file.seekg(0, ios::end);
    m_fragment_shader_source.resize(static_cast<unsigned int>(input_fragment_shader_file.tellg()));
    input_fragment_shader_file.seekg(0, ios::beg);
    input_fragment_shader_file.read(&m_fragment_shader_source[0], m_fragment_shader_source.size());
    input_fragment_shader_file.close();
  } else {
    cerr << "Could not open fragment shader file at: " << fragment_shader_file_name << endl;
    m_program_ok = false;
  }

  ifstream input_geometry_shader_file(geometry_shader_file_name, ios::in | ios::binary);
  if (input_geometry_shader_file) {
    input_geometry_shader_file.seekg(0, ios::end);
    m_geometry_shader_source.resize(static_cast<unsigned int>(input_geometry_shader_file.tellg()));
    input_geometry_shader_file.seekg(0, ios::beg);
    input_geometry_shader_file.read(&m_geometry_shader_source[0], m_geometry_shader_source.size());
    input_geometry_shader_file.close();
  } else {
    cerr << "Could not open geometry shader file at: " << geometry_shader_file_name << endl;
    m_program_ok = false;
  }

  /* or not */
  m_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  const char* start = &m_vertex_shader_source[0];
  glShaderSource(m_vertex_shader, 1, &start, NULL);

  m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  start = &m_fragment_shader_source[0];
  glShaderSource(m_fragment_shader, 1, &start, NULL);

  m_geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
  start = &m_geometry_shader_source[0];
  glShaderSource(m_geometry_shader, 1, &start, NULL);

  try {
    int status;
    glCompileShader(m_vertex_shader);
    glGetShaderiv(m_vertex_shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
      throw m_vertex_shader;
    }

    glCompileShader(m_fragment_shader);
    glGetShaderiv(m_fragment_shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
      throw m_fragment_shader;
    }

    glCompileShader(m_geometry_shader);
    glGetShaderiv(m_geometry_shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
      m_has_geometry = false;
      throw m_geometry_shader;
    } else {
      m_has_geometry = true;
    }
  } catch (GLuint bad_shader) {
    printShaderInfoLog(bad_shader);
    m_program_ok = false;
  }

  m_program = glCreateProgram();
  try {
    int status;
    glAttachShader(m_program, m_vertex_shader);
    glAttachShader(m_program, m_fragment_shader);
    glAttachShader(m_program, m_geometry_shader);
    glLinkProgram(m_program);

    glGetProgramiv(m_program, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
      throw m_program;
    }

  } catch (GLuint bad_program) {
    printProgramInfoLog(bad_program);
    m_program_ok = false;
  }
}

OGLProgram::~OGLProgram() {
  glDetachShader(m_program, m_vertex_shader);
  glDetachShader(m_program, m_fragment_shader);
  if (m_has_geometry) {
    glDetachShader(m_program, m_geometry_shader);
    glDeleteShader(m_geometry_shader);
  }
  glDeleteShader(m_vertex_shader);
  glDeleteShader(m_fragment_shader);
  glDeleteProgram(m_program);
}

void OGLProgram::use() {
  glUseProgram(m_program);
}

GLint OGLProgram::uniformLoc(string variable) {
  return glGetUniformLocation(m_program, variable.c_str());
}

GLint OGLProgram::attribLoc(string variable) {
  return glGetAttribLocation(m_program, variable.c_str());
}

GLint OGLProgram::subroutineUniformLoc(GLenum shadertype, string variable) {
  return glGetSubroutineUniformLocation(m_program, shadertype, variable.c_str());
}

GLuint OGLProgram::subroutineIndexLoc(GLenum shadertype, string variable) {
  return glGetSubroutineIndex(m_program, shadertype, variable.c_str());
}

GLuint OGLProgram::getID() {
  return m_program;
}

bool OGLProgram::isOK() {
  return m_program_ok;
}

bool OGLProgram::validate_shader_type(const GLenum& shadertype) {
  if (shadertype == GL_VERTEX_SHADER ||
    shadertype == GL_FRAGMENT_SHADER ||
    shadertype == GL_GEOMETRY_SHADER ||
    shadertype == GL_TESS_CONTROL_SHADER ||
    shadertype == GL_TESS_EVALUATION_SHADER) {
    return true;
  }
  return false;
}

OGLProgram::OGLProgram() {

}

} //namespacer ogl
