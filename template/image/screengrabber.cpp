#include <string>

#include <FreeImage.h>
#include <GL/glew.h>

#include "screengrabber.h"

namespace image {
ScreenGrabber::ScreenGrabber() : m_bytesPerPixel(3), m_frameNumber(0), m_clipNumber(0),
    m_height(0), m_width(0) {
  FreeImage_Initialise(1);
}

void ScreenGrabber::resize(int width, int height) {
  m_width = width;
  m_height = height;
}
/*
 Reset framenumber and updates clipnumber. This method can be used by the client to "group"
 several screenshootns in "clips". With the help of other programming tool it can be used to create
 recordings of the window.
*/
void ScreenGrabber::next() {
  m_frameNumber = 0;
  m_clipNumber++;
}

void ScreenGrabber::grab(bool useFrontBuffer, const char* fileName) {
  // Create temporal buffer
  GLubyte* screenBuffer = new GLubyte[(m_width * m_height * m_bytesPerPixel)];
  // Prepare to ask OpenGL for a given buffer
  glReadBuffer(useFrontBuffer ? GL_FRONT : GL_BACK);
  // This is needed in some drivers (not all). If unsure, just call it
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  // Capture framebuffer in temporal buffer
  glReadPixels(0, 0, m_width, m_height, GL_BGR, GL_UNSIGNED_BYTE, screenBuffer);
  // Save the temporal buffer in FreeImage structure
  FIBITMAP* image = FreeImage_ConvertFromRawBits(screenBuffer, m_width, m_height, (3 * m_width),
      24, 0x0000FF, 0xFF0000, 0x00FF00, false);
  // If user provided file name use it
  if (fileName) {
    // Save the buffer as an image in file
    FreeImage_Save(FIF_PNG, image, fileName, 0);
  } else {
    // User did not provide file name: calculate your own
    const char* locationFormat = "Screenshoots/%d-%d.png";
    char fileNameAsNumber[256];
#if defined WIN32 || defined _WIN32
    sprintf_s(fileNameAsNumber, locationFormat, m_clipNumber, m_frameNumber);
#else
    sprintf(fileNameAsNumber, locationFormat, m_clipNumber, m_frameNumber);
#endif
    // Save the buffer as an image in file
    FreeImage_Save(FIF_PNG, image, fileNameAsNumber, 0);
  }
  // Delete FreeImage data struct
  FreeImage_Unload(image);
  // Update our internam counter
  m_frameNumber++;
  // Delete temporal buffer
  delete[] screenBuffer;
}

} // namespace image