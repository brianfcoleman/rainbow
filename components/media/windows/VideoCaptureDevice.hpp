#ifndef VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_H
#define VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_H

#include <string>
#include <list>
#include "ByteBufferCallback.hpp"
#include "RGBVideoFormat.hpp"

namespace VideoCapture {

class VideoCaptureDeviceImpl;

class VideoCaptureDevice {
 public:
  VideoCaptureDevice();
  explicit VideoCaptureDevice(VideoCaptureDeviceImpl* const pImpl);
  bool isInitialized() const;
  std::string name() const;
  bool addOnNewVideoFrameCallback(ByteBufferCallback* const pCallback);
  bool removeOnNewVideoFrameCallback(ByteBufferCallback* const pCallback);
  bool startCapturing();
  bool stopCapturing();
  double countFramesCapturedPerSecond() const;
  std::list<RGBVideoFormat> videoFormatList() const;
  RGBVideoFormat currentVideoFormat() const;
  bool setCurrentVideoFormat(const RGBVideoFormat& rgbVideoFormat);
  operator bool() const {
    return isInitialized();
  }
 private:
  static const std::string s_kEmptyString;
  VideoCaptureDeviceImpl* m_pImpl;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_H
