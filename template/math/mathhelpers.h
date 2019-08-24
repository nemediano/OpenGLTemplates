#ifndef MATH_HELPERS_H_
#define MATH_HELPERS_H_

#include <glm/glm.hpp>

namespace math {
  float toRadians(const float& degree);
  float toDegree(const float& radians);
  float toFCol(const unsigned char& c);
  unsigned char toICol(const float& v);
  glm::vec3 toFCol(const unsigned char& r, const unsigned char& g, const unsigned char& b);
  glm::vec4 toFCol(const unsigned char& r, const unsigned char& g, const unsigned char& b, 
      const unsigned char& a);
} // namespace math

#endif