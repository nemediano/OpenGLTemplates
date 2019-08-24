#ifndef TEXTURE_H_
#define TEXTURE_H_


#include <string>
#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>

namespace image {
  class Texture {
  private:
    unsigned int m_width;
    unsigned int m_height;
    std::vector<unsigned char> m_data;
    GLuint m_texture_id;
    void release_location();
    void ask_locations();
  public:
    Texture();
    Texture(const std::string& input_file_name);
    ~Texture();
    bool load_texture(const std::string& input_file_name);
    void bind() const;
    void send_to_gpu();
    int get_width() const;
    int get_height() const;
    GLuint get_id() const;
    bool save(const std::string& output_png_file) const;
    friend Texture defaultStencil(float cutoff, unsigned int size);
    friend Texture chessBoard(unsigned int size, unsigned int cells, glm::vec3 black, glm::vec3 white);
  };
} // namespace image

#endif