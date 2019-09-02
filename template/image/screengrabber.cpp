#include <string>

#include <FreeImage.h>
#include <GL/glew.h>

#include "screengrabber.h"

namespace image {
ScreenGrabber::ScreenGrabber() : m_bytesPerPixel(3), m_frameNumber(0), m_clipNumber(0), 
    m_height(0), m_width(0) {
  FreeImage_Initialise(1);
}

void ScreenGrabber::resize(int w, int h) {
  m_width = w; 
  m_height = h;
}

void ScreenGrabber::next() {
  m_frameNumber = 0; 
  m_clipNumber++;
}

void ScreenGrabber::grab(bool useFrontBuffer, const char* fileName) {
  
  GLubyte* screenBuffer = new GLubyte[(m_width * m_height * m_bytesPerPixel)];
  
  (useFrontBuffer) ? glReadBuffer(GL_FRONT) : glReadBuffer(GL_BACK);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadPixels(0, 0, m_width, m_height, GL_BGR, GL_UNSIGNED_BYTE, screenBuffer);

  FIBITMAP* image = FreeImage_ConvertFromRawBits(screenBuffer, m_width, m_height, (3 * m_width), 
      24, 0x0000FF, 0xFF0000, 0x00FF00, false);

  if (fileName) {
    FreeImage_Save(FIF_PNG, image, fileName, 0);
  } else {
    const char* locationFormat = "Screenshoots/%d-%d.png";
    char fileNameAsNumber[256];
#if defined WIN32 || defined _WIN32
    sprintf_s(fileNameAsNumber, locationFormat, m_clipNumber, m_frameNumber);
#else
    sprintf(fileNameAsNumber, locationFormat, m_clipNumber, m_frameNumber);
#endif
    FreeImage_Save(FIF_PNG, image, fileNameAsNumber, 0);
  }

  FreeImage_Unload(image);
  m_frameNumber++;

  delete[] screenBuffer;
}

} // namespace image