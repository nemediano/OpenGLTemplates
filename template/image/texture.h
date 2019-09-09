#ifndef TEXTURE_H_
#define TEXTURE_H_


#include <string>
#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>

namespace image {
//! This class encapsulates an OpenGL texture
/*!
  This class let you create a texture or load it from a file, then sent it to the GPU
  and keep track of the OpenGL handle to use it.

  It depends on the library FreeImage.
*/
class Texture {

private:
  unsigned int m_width;
  unsigned int m_height;
  std::vector<unsigned char> m_data;
  GLuint m_texture_id;
  void release_location();
  void ask_locations();

public:
  //! Simple constructor that does nothing.
  Texture();
  //! Construct a texture by loading the data from a file
  Texture(const std::string& input_file_name);
  ~Texture();
  //! Loads data from a file into this texture
  bool load_texture(const std::string& input_file_name);
  //! Binds this texture so an OpenGL program can use it
  void bind() const;
  //! Send texture data to the GPU
  void send_to_gpu();
  //! Get the texture width in pixels
  int get_width() const;
  //! Get the texture height in pixels
  int get_height() const;
  //! Return OpenGL handle for this texture
  GLuint get_id() const;
  //! Save the texture data into a file
  bool save(const std::string& output_png_file) const;
  //! Returns a procedurally created texture of a radial diffusee circle
  /*!
    Create a texture that represents a centered grayscaled circle. Which is very usefull as default
    stencil for a spotlight.
    @param cutoff normalized distance in [0.0f, 1.0f] from the edge to the center
    at which the stencil lets all the light pass. Defaults to 0.2 (20%)
    @param size in pixels of the texture (it's always a square texture)
  */
  friend Texture defaultStencil(float cutoff, unsigned int size);
  //! Returns a procedurally created texture of a chessboard pattern
  /*!
    Create a texture in a chessboard pattern.
    @param size in pixels of the texture (it's always a square texture)
    @param cells number of cells per edge (defaults to 8 as in a normal chessboard)
    @param black Color (as normalized vector) of the odd cells in texture (defaults to black as
        in a normal chessboard)
    @param white Color (as normalized vector) of the even cells in texture (defaults to white as
        in a normal chessboard)
  */
  friend Texture chessBoard(unsigned int size, unsigned int cells, glm::vec3 black, glm::vec3 white);
};

} // namespace image

#endif