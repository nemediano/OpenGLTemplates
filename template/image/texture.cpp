#include <iostream>
#include <string>

#include <FreeImage.h>

#include "texture.h"

namespace image {

Texture::Texture() : m_width(0), m_height(0), m_texture_id(0) {

}

Texture::Texture(const std::string& input_file_name) : Texture() {
  load_texture(input_file_name);
  //ask_locations();
  //send_to_gpu();
}

Texture::~Texture() {
  release_location();
}

bool Texture::load_texture(const std::string& input_file_name) {
  
  FIBITMAP* tempImg = FreeImage_Load(FreeImage_GetFileType(input_file_name.c_str(), 0), input_file_name.c_str());
  if (!tempImg) {
    std::cerr << "Could not load image: " << input_file_name << std::endl;
    return false;
  }
  FIBITMAP* img = FreeImage_ConvertTo32Bits(tempImg);
  if (!img) {
    std::cerr << "Image: " << input_file_name << " is damaged" << std::endl;
    return false;
  }

  FreeImage_Unload(tempImg);

  m_width = FreeImage_GetWidth(img);
  m_height = FreeImage_GetHeight(img);
  GLuint scanW = FreeImage_GetPitch(img);

  m_data.resize(m_height * scanW);
  FreeImage_ConvertToRawBits(m_data.data(), img, scanW, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE);
  FreeImage_Unload(img);

  return true;
}

bool Texture::save(const std::string& output_png_file) const {
  const int bytesPerPixel = 4;
  unsigned char* tmpBuffer = new unsigned char[m_width * m_height * bytesPerPixel];
  std::copy(m_data.begin(), m_data.end(), tmpBuffer);
  
  FIBITMAP* image = FreeImage_ConvertFromRawBits(tmpBuffer, m_width, m_height, m_width * bytesPerPixel, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE);
  FreeImage_Save(FIF_PNG, image, output_png_file.c_str(), 0);

  FreeImage_Unload(image);
  
  delete[] tmpBuffer;

  return true;
}

void Texture::bind() const {
  glBindTexture(GL_TEXTURE_2D, m_texture_id);
}

int Texture::get_width() const {
  return static_cast<int>(m_width);
}

int Texture::get_height() const {
  return static_cast<int>(m_height);
}

GLuint Texture::get_id() const {
  return m_texture_id;
}

void Texture::send_to_gpu() {

  if (m_texture_id == 0) {
    ask_locations();
  }

  glBindTexture(GL_TEXTURE_2D, m_texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, m_data.data());
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::release_location() {
  glDeleteTextures(1, &m_texture_id);
}

void Texture::ask_locations() {
  glGenTextures(1, &m_texture_id);
}

} // namespace image