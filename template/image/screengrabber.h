#ifndef SCREEN_GRABBER_H_
#define SCREEN_GRABBER_H_

namespace image {
  class ScreenGrabber {
  public:
    ScreenGrabber();
    void grab(bool useFrontBuffer = false, const char* fileName = nullptr);
    void resize(int, int);
    void next();
  private:
    int m_bytesPerPixel;
    int m_frameNumber;
    int m_clipNumber;
    int m_height;
    int m_width;
  };
}
#endif