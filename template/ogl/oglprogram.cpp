#include <fstream>
#include <iostream>

#include "oglprogram.h"
#include "oglhelpers.h"

namespace ogl {

using namespace std;

OGLProgram::OGLProgram(string vertex_shader_file_name, string fragment_shader_file_name) {

  m_program_ok = true;
  /* Read shader's source code from input files */
  m_vertex_shader_source = read_shader_from_file(vertex_shader_file_name);
  if (m_vertex_shader_source.empty() || !m_program_ok) {
    cerr << "Could not open vertex shader file at: " << vertex_shader_file_name << endl;
  }
  m_fragment_shader_source = read_shader_from_file(fragment_shader_file_name);
  if (m_fragment_shader_source.empty() || !m_program_ok) {
    cerr << "Could not open fragment shader file at: " << fragment_shader_file_name << endl;
  }
  /* Send the shader source to GPU and keep the locations */
  m_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  const char* start = &m_vertex_shader_source[0];
  glShaderSource(m_vertex_shader, 1, &start, nullptr);
  m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  start = &m_fragment_shader_source[0];
  glShaderSource(m_fragment_shader, 1, &start, nullptr);
  // Try to compile the shaders
  try {
    int status;
    //Vertex shader compilation
    glCompileShader(m_vertex_shader);
    glGetShaderiv(m_vertex_shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
      throw m_vertex_shader;
    }
    //Fragment shader compilation
    glCompileShader(m_fragment_shader);
    glGetShaderiv(m_fragment_shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
      throw m_fragment_shader;
    }
  } catch (GLuint bad_shader) {
    // Some shader was not compiled print error log
    printShaderInfoLog(bad_shader);
    m_program_ok = false;
  }
  // Shaders compile individually. Now, try to link an OpenGL program
  m_program = glCreateProgram(); // Ask for location
  try {
    int status;
    // Attach shaders to program
    glAttachShader(m_program, m_vertex_shader);
    glAttachShader(m_program, m_fragment_shader);
    glLinkProgram(m_program);
    // Try to link
    glGetProgramiv(m_program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
      throw m_program;
    }
  } catch (GLuint bad_program) {
    // The program was not linked print error log
    printProgramInfoLog(bad_program);
    m_program_ok = false;
  }
}

OGLProgram::OGLProgram(std::string vertex_shader_file_name, std::string fragment_shader_file_name,
             std::string geometry_shader_file_name) {

  m_program_ok = true;
  /* Read shader's source code from input files */
  m_vertex_shader_source = read_shader_from_file(vertex_shader_file_name);
  if (m_vertex_shader_source.empty() || !m_program_ok) {
    cerr << "Could not open vertex shader file at: " << vertex_shader_file_name << endl;
  }
  m_fragment_shader_source = read_shader_from_file(fragment_shader_file_name);
  if (m_fragment_shader_source.empty() || !m_program_ok) {
    cerr << "Could not open vertex shader file at: " << fragment_shader_file_name << endl;
  }
  m_geometry_shader_source = read_shader_from_file(geometry_shader_file_name);
  if (m_geometry_shader_source.empty() || !m_program_ok) {
    cerr << "Could not open geometry shader file at: " << geometry_shader_file_name << endl;
  }
  /* Send the shader source to GPU and keep the locations */
  m_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  const char* start = &m_vertex_shader_source[0];
  glShaderSource(m_vertex_shader, 1, &start, nullptr);
  m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  start = &m_fragment_shader_source[0];
  glShaderSource(m_fragment_shader, 1, &start, nullptr);
  m_geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
  start = &m_geometry_shader_source[0];
  glShaderSource(m_geometry_shader, 1, &start, nullptr);
  // Try to compile the shaders
  try {
    int status;
    //Vertex shader compilation
    glCompileShader(m_vertex_shader);
    glGetShaderiv(m_vertex_shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
      throw m_vertex_shader;
    }
    //Fragment shader compilation
    glCompileShader(m_fragment_shader);
    glGetShaderiv(m_fragment_shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
      throw m_fragment_shader;
    }
    //Geometry shader compilation
    glCompileShader(m_geometry_shader);
    glGetShaderiv(m_geometry_shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
      m_has_geometry = false;
      throw m_geometry_shader;
    } else {
      m_has_geometry = true;
    }
  } catch (GLuint bad_shader) {
     // The program was not linked print error log
    printShaderInfoLog(bad_shader);
    m_program_ok = false;
  }
  // Shaders compile individually. Now, try to link an OpenGL program
  m_program = glCreateProgram();
  try {
    int status;
    // Attach shaders to program
    glAttachShader(m_program, m_vertex_shader);
    glAttachShader(m_program, m_fragment_shader);
    glAttachShader(m_program, m_geometry_shader);
    glLinkProgram(m_program);
    // Try to link
    glGetProgramiv(m_program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
      throw m_program;
    }
  } catch (GLuint bad_program) {
    // The program was not linked print error log
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

string OGLProgram::read_shader_from_file(const string& shader_file_name) {
  // Try to open the given file for read
  ifstream input_shader_file(shader_file_name, ios::in | ios::binary);
  string shader_source_code;
  if (input_shader_file) {
    // Send the cursor to the end of the file
    input_shader_file.seekg(0, ios::end);
    // Resize the string to fit the whole file content (Remember cursor is at the end)
    shader_source_code.resize(static_cast<unsigned int>(input_shader_file.tellg()));
    // Put the cursor back to the begining of the file 
    input_shader_file.seekg(0, ios::beg);
    // Read all the contents of the file until you fill the string (Remember string size 
    // matches file size)
    input_shader_file.read(&shader_source_code[0], shader_source_code.size());
    // Close file
    input_shader_file.close();
  } else {
    // You could not open the file for read
    m_program_ok = false;
  }
  // Return the content of the file as string
  return shader_source_code;
}

OGLProgram::OGLProgram() {
  // Empthy constructor to prevent auto generation
}

} //namespacer ogl
