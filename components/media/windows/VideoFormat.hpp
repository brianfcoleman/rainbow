#ifndef VIDEO_CAPTURE_VIDEO_FORMAT_H
#define VIDEO_CAPTURE_VIDEO_FORMAT_H

#include "VideoFormatTypes.hpp"

namespace VideoCapture {

class VideoFormatImpl;

class VideoFormat {
 public:
  VideoFormat();
  explicit VideoFormat(VideoFormatImpl* const pImpl);
  bool isInitialized() const;
  PRInt32 id() const;
  double framesPerSecond() const;
  IntegerSize sizePixels() const;
  PRInt32 angleRotationDegrees() const;
  PRInt32 bitsPerPixel() const;
  PRInt32 sizeBytes() const;
  PRInt32 sizeRowBytes() const;
  RGBFormat rgbFormat() const;
  VideoFormatImpl* get() const;
  operator bool() const {
    return isInitialized();
  }
 private:
  VideoFormatImpl* m_pImpl;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_VIDEO_FORMAT_H
