#ifndef VIDEO_CAPTURE_RGB_VIDEO_FORMAT_H
#define VIDEO_CAPTURE_RGB_VIDEO_FORMAT_H

#include "VideoFormat.hpp"

namespace VideoCapture {

class RGBVideoFormat {
 public:
  RGBVideoFormat();
  explicit RGBVideoFormat(const VideoFormat& videoFormat);
  RGBVideoFormat(
      const PRUint32 id,
      const IntegerSize sizePixels,
      const PRInt32 angleRotationDegrees,
      const RGBFormat rgbFormat);
  bool isInitialized() const;
  PRUint32 id() const;
  IntegerSize sizePixels() const;
  PRInt32 angleRotationDegrees() const;
  PRInt32 bitsPerPixel() const;
  PRInt32 sizeBytes() const;
  PRInt32 sizeRowBytes() const;
  RGBFormat rgbFormat() const;
  operator bool() const {
    return isInitialized();
  }
 private:
  bool initialize();
  bool m_isInitialized;
  PRUint32 m_id;
  IntegerSize m_sizePixels;
  PRInt32 m_angleRotationDegrees;
  RGBFormat m_rgbFormat;
};

bool operator==(const RGBVideoFormat& lhs, const RGBVideoFormat& rhs);

bool operator==(const RGBVideoFormat& lhs, const VideoFormat& rhs);

bool operator==(const VideoFormat& lhs, const RGBVideoFormat& rhs);

bool operator!=(const RGBVideoFormat& lhs, const RGBVideoFormat& rhs);

bool operator!=(const RGBVideoFormat& lhs, const VideoFormat& rhs);

bool operator!=(const VideoFormat& lhs, const RGBVideoFormat& rhs);

} // VideoCapture

#endif // VIDEO_CAPTURE_RGB_VIDEO_FORMAT_H
