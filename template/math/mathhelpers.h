#ifndef MATH_HELPERS_H_
#define MATH_HELPERS_H_

#include <glm/glm.hpp>

namespace math {
//! Helper function to certain math operations.
/*!
    Since glm does almost evrything these are mostly wrappers and conversions
*/
//! Angle conversion form degree to radians
float toRadians(const float& degree);
//! Angle conversion form radiasm to degree
float toDegree(const float& radians);
//! Color format conversion from a byte in [0, 255] to a normalized float in [0.0f, 1.0f]
float toFCol(const unsigned char& c);
//! Color format conversion from a normalized float in [0.0f, 1.0f] to a byte in [0, 255]
unsigned char toICol(const float& v);
//! Color format conversion from byte values in [0, 255] to a normalized float vector in [0.0f, 1.0f]
glm::vec3 toFCol(const unsigned char& r, const unsigned char& g, const unsigned char& b);
//! Color format conversion from byte values in [0, 255] to a normalized float vector in [0.0f, 1.0f]
glm::vec4 toFCol(const unsigned char& r, const unsigned char& g, const unsigned char& b,
    const unsigned char& a);
} // namespace math

#endif