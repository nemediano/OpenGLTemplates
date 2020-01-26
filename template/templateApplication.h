// ANSI C++ includes
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
// Third party libraries includes
// Dear imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
// GLEW
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GLM
#define GLM_FORCE_PURE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Includes from this template
#include "image/texture.h"
#include "image/proceduraltextures.h"
#include "mesh/model.h"
#include "ogl/oglhelpers.h"
#include "ogl/oglprogram.h"
#include "callbacks.h"
#include "common.h"
#include "menu.h"




class TemplateApplication {
  public:
    void run();
  private:
    // Location for shader variables
    GLint u_PVM_location = -1;
    GLint u_NormalMat_location = -1;
    GLint u_DiffuseMap_location = -1;
    GLint u_SpecularMap_location = -1;
    GLint u_Alpha_location = -1;
    GLint a_position_loc = -1;
    GLint a_normal_loc = -1;
    GLint a_textureCoord_loc = -1;
    // OpenGL program handler
    ogl::OGLProgram* ogl_program_ptr = nullptr;
    // Two buffers to interact with the Model class
    std::vector<image::Texture*> textures;
    std::vector<mesh::MeshData> separators;
    // To keep track the elapsed time between frames
    double last_time = 0.0;
    // Vertex Array Object used to manage the Vertex Buffer Objects
    GLuint vao;
    void init_glfw();
    void load_OpenGL();
    void init_program();
    void load_model_data_and_send_to_gpu();
    void render();
    void update();
    void free_resources();
};