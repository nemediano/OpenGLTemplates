#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "common.h"
#include "menu.h"

void setup_menu() {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  // Basic look and feel
  ImGui::StyleColorsDark();
  // Use standard connection with GLFW and modern OpenGL
  ImGui_ImplGlfw_InitForOpenGL(common::window, true);
  const char* glsl_version{"#version 130"};
  ImGui_ImplOpenGL3_Init(glsl_version);
}

void create_menu() {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  // Draw the ui menu
  ImGui::Begin("Basic template menu"); //Name of the menu
    ImGui::Text("Options"); //Simple text
    //Imgui's controls return true on interaction
    if (ImGui::Checkbox("Rotate", &common::rotating)) { 
      common::current_angle = 0.0f; // So, besides setting variable we can execute code
    }
    ImGui::SliderFloat("Alpha", &common::alpha, 0.0f, 16.0f, "%.1f", 2.0f);
    if (ImGui::CollapsingHeader("Enviroment info:")) { // Submenu
      ImGui::Text("%s", common::context_info.c_str());
    }
    if (ImGui::CollapsingHeader("Application stats")) {
      ImGui::Text("Average frame: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
      ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    }
  ImGui::End();
}