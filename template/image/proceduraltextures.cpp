#include "../math/mathhelpers.h"
#include "proceduraltextures.h"

namespace image {

Texture defaultStencil(float cutoff, unsigned int size) {

  using glm::vec2;
  using glm::vec3;
  using glm::vec4;

  Texture t;
  t.m_height = t.m_width = size;
  const int bytesPerPixel = 4;
  t.m_data.resize(t.m_height * t.m_width * bytesPerPixel);

  for (unsigned int i = 0; i < size; i++) {
    for (unsigned int j = 0; j < size; j++) {
      // Normalized distances (vector) from the middle of an edge to the center of the texture
      // in other words: the texture coordinates of this pixel
      vec2 coordinates = (1.0f / size) * vec2(i, j) + (0.5f / size);
      // Normalized distance (scalar) to the center of the texture
      float distanceNormalized = 2.0f * glm::distance(coordinates, glm::vec2(0.5f));
      float greyLevel = 1.0f;
      if (distanceNormalized < cutoff) { //If the distance is less than the cutoff
        greyLevel = 1.0f;
      }  else if (distanceNormalized < 1.0f) {
        // The distance is greater than the cutoff but less than 1.0
        // Calculate linear gradient
        float s = (distanceNormalized - cutoff) / (1.0f - cutoff);
        greyLevel = 1.0f - s;
      } else { // Distance is greater than one (like in the corners)
        greyLevel = 0.0f;
      }
      /*
      vec4 pixel = vec4(vec3(greyLevel), 1.0f);
      unsigned char red = static_cast<unsigned char>(255.0f * pixel.r);
      unsigned char green = static_cast<unsigned char>(255.0f * pixel.g);
      unsigned char blue = static_cast<unsigned char>(255.0f * pixel.b);
      unsigned char alpha = static_cast<unsigned char>(255.0f * pixel.a);
      t.m_data[(i * t.m_width + j) * bytesPerPixel + 0] = red;
      t.m_data[(i * t.m_width + j) * bytesPerPixel + 1] = green;
      t.m_data[(i * t.m_width + j) * bytesPerPixel + 2] = blue;
      t.m_data[(i * t.m_width + j) * bytesPerPixel + 3] = alpha;
      */
      unsigned char grey = math::toICol(greyLevel);
      t.m_data[(i * t.m_width + j) * bytesPerPixel + 0] = grey;
      t.m_data[(i * t.m_width + j) * bytesPerPixel + 1] = grey;
      t.m_data[(i * t.m_width + j) * bytesPerPixel + 2] = grey;
      t.m_data[(i * t.m_width + j) * bytesPerPixel + 3] = 255;
    }
  }

  return t;
}

Texture chessBoard(unsigned int size, unsigned int cells, glm::vec3 black, glm::vec3 white) {
  using glm::vec2;
  using glm::ivec2;
  using glm::vec3;
  using glm::vec4;

  Texture t;
  t.m_height = t.m_width = size;
  const int bytesPerPixel = 4;
  t.m_data.resize(size * size * bytesPerPixel);

  for (unsigned int i = 0; i < size; i++) {
    for (unsigned int j = 0; j < size; j++) {
      // Normalized distances (vector) from the middle of an edge to the center of the texture
      // in other words: the texture coordinates of this pixel
      vec2 coordinates = (1.0f / size) * vec2(i, j) + (0.5f / size);
      // In which cell of the chessboard is this pixel
      ivec2 cell = static_cast<float>(cells) * coordinates;
      vec3 pixel_color = (cell.x + cell.y) % 2 ? white : black;
      unsigned char red = math::toICol(pixel_color.r);
      unsigned char green = math::toICol(pixel_color.g);
      unsigned char blue = math::toICol(pixel_color.b);
      t.m_data[(i * t.m_width + j) * bytesPerPixel + 0] = red;
      t.m_data[(i * t.m_width + j) * bytesPerPixel + 1] = green;
      t.m_data[(i * t.m_width + j) * bytesPerPixel + 2] = blue;
      t.m_data[(i * t.m_width + j) * bytesPerPixel + 3] = 255;
    }
  }

  return t;
}

} // namespace image