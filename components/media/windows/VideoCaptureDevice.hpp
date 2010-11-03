#ifndef VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_H
#define VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_H

#include "nsStringAPI.h"
#include "nsTArray.h"
#include "VideoFrameCallback.hpp"
#include "RGBVideoFormat.hpp"

namespace VideoCapture {

class VideoCaptureDeviceImpl;

class VideoCaptureDevice {
 public:
  VideoCaptureDevice();
  explicit VideoCaptureDevice(VideoCaptureDeviceImpl* const pImpl);
  bool isInitialized() const;
  nsString name() const;
  bool addOnNewVideoFrameCallback(VideoFrameCallback* const pCallback);
  bool removeOnNewVideoFrameCallback(VideoFrameCallback* const pCallback);
  bool startCapturing();
  bool stopCapturing();
  double countFramesCapturedPerSecond() const;
  nsTArray<RGBVideoFormat> videoFormatList() const;
  RGBVideoFormat currentVideoFormat() const;
  bool setCurrentVideoFormat(const RGBVideoFormat& rgbVideoFormat);
  operator bool() const {
    return isInitialized();
  }
 private:
  static const nsString s_kEmptyString;
  VideoCaptureDeviceImpl* m_pImpl;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_H
