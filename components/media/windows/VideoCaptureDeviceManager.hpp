#ifndef VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_MANAGER_H
#define VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_MANAGER_H

#include "VideoCaptureDevice.hpp"

namespace VideoCapture {

class VideoCaptureDeviceManagerImpl;

class VideoCaptureDeviceManager {
 public:
  VideoCaptureDeviceManager();
  VideoCaptureDeviceManager(VideoCaptureDeviceManagerImpl* pImpl);
  ~VideoCaptureDeviceManager();
  nsTArray<VideoCaptureDevice> videoCaptureDeviceList() const;
  bool isInitialized() const;
  operator bool() const {
    return isInitialized();
  }
 private:
  VideoCaptureDeviceManagerImpl* m_pImpl;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_MANAGER_H
