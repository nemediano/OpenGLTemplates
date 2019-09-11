#include "materialphong.h"

namespace light {

  MatPhong::MatPhong() : MatPhong(glm::vec3(0.5f)) {

  }

  MatPhong::MatPhong(const MatPhong& other) {
    m_Ka = other.m_Ka;
    m_Kd = other.m_Kd;
    m_Ks = other.m_Ks;
    m_alpha = other.m_alpha;
  }

  MatPhong::MatPhong(const glm::vec3& ambientAndDiffuse) {
    setKa(0.25f * ambientAndDiffuse);
    setKd(ambientAndDiffuse);
    setKs(glm::vec3(0.5f));
    setAlpha(16.0f);
  }

  MatPhong::MatPhong(const glm::vec3& Ka, const glm::vec3& Kd, const glm::vec3& Ks, float alpha) {
    setKa(Ka);
    setKd(Kd);
    setKs(Ks);
    setAlpha(alpha);
  }

  void MatPhong::setKa(const glm::vec3& Ka) {
    m_Ka = glm::clamp(Ka, glm::vec3(0.0f), glm::vec3(1.0f));
  }

  void MatPhong::setKd(const glm::vec3& Kd) {
    m_Kd = glm::clamp(Kd, glm::vec3(0.0f), glm::vec3(1.0f));
  }

  void MatPhong::setKs(const glm::vec3& Ks) {
    m_Ks = glm::clamp(Ks, glm::vec3(0.0f), glm::vec3(1.0f));
  }

  void MatPhong::setAlpha(float alpha) {
    m_alpha = glm::clamp(alpha, 0.0f, 256.0f);
  }

  glm::vec3 MatPhong::getKa() const {
    return m_Ka;
  }

  glm::vec3 MatPhong::getKd() const {
    return m_Kd;
  }

  glm::vec3 MatPhong::getKs() const {
    return m_Ks;
  }

  float MatPhong::getAlpha() const {
    return m_alpha;
  }

  MatPhong::~MatPhong() {

  }
} // namespace light