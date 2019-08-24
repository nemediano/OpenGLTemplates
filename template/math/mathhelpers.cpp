#include "mathhelpers.h"

namespace math {
  float toRadians(const float& degree) {
    //return degree * TO_RADIANS;
    return glm::radians(degree);
  }

  float toDegree(const float& radians) {
    //return radians * TO_DEGREE;
    return glm::degrees(radians);
  }

  float toFCol(const unsigned char& c) {
    return static_cast<float>(c) / 255.f;
  }

  unsigned char toICol(const float& v) {
    return static_cast<unsigned char>(round(v * 255.0f));
  }

  glm::vec3 toFCol(const unsigned char& r, const unsigned char& g, const unsigned char& b) {
    return glm::vec3(toFCol(r), toFCol(g), toFCol(b));
  }

  glm::vec4 toFCol(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a) {
    return glm::vec4(toFCol(r), toFCol(g), toFCol(b), toFCol(a));
  }
}