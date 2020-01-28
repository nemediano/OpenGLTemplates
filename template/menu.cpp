#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "ogl/oglhelpers.h"
#include "common.h"

#include "templateApplication.h"

void TemplateApplication::setup_menu() {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  // Basic look and feel
  ImGui::StyleColorsDark();
  // Use standard connection with GLFW and modern OpenGL
  ImGui_ImplGlfw_InitForOpenGL(mWinPtr, true);
  const char* glsl_version{"#version 130"};
  ImGui_ImplOpenGL3_Init(glsl_version);
}

void TemplateApplication::create_menu() {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  // Draw the ui menu
  ImGui::Begin("Basic template menu"); //Name of the menu
    ImGui::Text("Options"); //Simple text
    //Imgui's controls return true on interaction
    if (ImGui::Checkbox("Rotate", &mRotating)) {
      //common::current_angle = 0.0f; // So, besides setting variable we can execute code
      mCurrentAngle = 0.0f; // So, besides setting variable we can execute code
    }
    ImGui::SliderFloat("Alpha", &mAlpha, 1.0f, 16.0f, "%.1f", 2.0f);
    if (ImGui::CollapsingHeader("Enviroment info:")) { // Submenu
      ImGui::Text("%s", "Hardware");
      ImGui::TextColored(ImVec4(0,0.5,1,1), "GPU:");
      ImGui::SameLine();
      ImGui::Text("%s", ogl::get_version(ogl::GPU).c_str());
      ImGui::TextColored(ImVec4(0,0.5,1,1), "Vendor:");
      ImGui::SameLine();
      ImGui::Text("%s", ogl::get_version(ogl::VENDOR).c_str());
      ImGui::Text("%s", "Software");
      ImGui::TextColored(ImVec4(0,0.5,1,1), "OpenGL:");
      ImGui::SameLine();
      ImGui::Text("%s", ogl::get_version(ogl::OPENGL).c_str());
      ImGui::TextColored(ImVec4(0,0.5,1,1), "GLSL:");
      ImGui::SameLine();
      ImGui::Text("%s", ogl::get_version(ogl::GLSL).c_str());
      ImGui::TextColored(ImVec4(1,0.5,0,1), "Libraries");
      ImGui::TextColored(ImVec4(0,0.5,1,1), "GLFW:");
      ImGui::SameLine();
      ImGui::Text("%s", ogl::get_version(ogl::GLFW).c_str());
      ImGui::TextColored(ImVec4(0,0.5,1,1), "Dear ImGui:");
      ImGui::SameLine();
      ImGui::Text("%s", ogl::get_version(ogl::IMGUI).c_str());
      ImGui::TextColored(ImVec4(0,0.5,1,1), "GLM:");
      ImGui::SameLine();
      ImGui::Text("%s", ogl::get_version(ogl::GLM).c_str());
      ImGui::TextColored(ImVec4(0,0.5,1,1), "GLEW:");
      ImGui::SameLine();
      ImGui::Text("%s", ogl::get_version(ogl::GLEW).c_str());
    }
    if (ImGui::CollapsingHeader("Application stats")) {
      ImGui::Text("Average frame: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
      ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
      ImGui::Text("OpenGL's debug log is %s", mHasDebug ? "enabled" : "disabled");
    }
  ImGui::End();
}