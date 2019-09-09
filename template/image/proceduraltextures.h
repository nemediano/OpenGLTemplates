#ifndef PROCEDURAL_TEXTURES_H_
#define PROCEDURAL_TEXTURES_H_

#include "texture.h"

namespace image {
  //! Factories for procedural textures commonly used in Computer Graphics

  //! Returns a procedurally created texture of a radial diffused circle
  /*!
    Create a texture that represents a centered grayscaled circle. Which is very usefull as default
    stencil for a spotlight.
    @param cutoff normalized distance in [0.0f, 1.0f] from the edge to the center
    at which the stencil lets all the light pass. Defaults to 0.2 (20%)
    @param size in pixels of the texture (it's always a square texture)
  */
  Texture defaultStencil(float cutoff = 0.2f, unsigned int size = 1024);
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
  Texture chessBoard(unsigned int size = 1024, unsigned int cells = 8,
      glm::vec3 black = glm::vec3(0.0f), glm::vec3 white = glm::vec3(1.0f));
} // namespace image

#endif