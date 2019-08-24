#ifndef PROCEDURAL_TEXTURES_H_
#define PROCEDURAL_TEXTURES_H_

#include "texture.h"

namespace image {
  Texture defaultStencil(float cutoff = 0.2f, unsigned int size = 1024);
  Texture chessBoard(unsigned int size = 1024, unsigned int cells = 8, 
      glm::vec3 black = glm::vec3(0.0f), glm::vec3 white = glm::vec3(1.0f));
} // namespace image

#endif