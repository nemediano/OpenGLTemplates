#ifndef SCREEN_GRABBER_H_
#define SCREEN_GRABBER_H_

namespace image {
  //! An object that is able to save the current window content (framebuffer) as an image file
  /*!
    This class let you save the contents of the framebuffer of the current window and save it
    as an image. To use it correctlly you need let the object know everytime the frambuffer
    changes size

    An ScreenShoots directory needs to existe in the same folder as the executable. Otherwise the
    operation will be sillentlly ignored

    If no filename is provided for the filename, it will generate a standar filename with counters
    included.

    It depends on the library FreeImage. And uses *.png as the image output format
  */
class ScreenGrabber {
public:
  //! Defaul only constructor
  ScreenGrabber();
  //! Save the current content of the framebuffer as an image
  /*!
    If no filename is provided the internal logic uses two parameters to create the filename:
    <clipnumber>-<framenumber>.png. By default only framenumber gets increased. You can use
    next method to update the clipnumber
    @param useFrontBuffer if true captures front buffer (default to false, i. e. uses back buffer)
    @param filename of the image to be saved.
  */
  void grab(bool useFrontBuffer = false, const char* fileName = nullptr);
  //! Let this object know that the screenbuffer has been resized
  void resize(int width, int height);
  //! Move the next number on the clipnumber and reset framenumber to zero
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