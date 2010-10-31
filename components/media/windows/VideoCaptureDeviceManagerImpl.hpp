#ifndef VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_MANAGER_IMPL_H
#define VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_MANAGER_IMPL_H

#include "nsAutoPtr.h"
#include "nsTArray.h"
#include "DirectShow.hpp"
#include "VideoCaptureDevice.hpp"
#include "Uncopyable.hpp"

namespace VideoCapture {

class QeditTypeLibraryImpl;
class VideoCaptureDeviceImpl;

class VideoCaptureDeviceManagerImpl : private Uncopyable {
 public:
  VideoCaptureDeviceManagerImpl();
  nsTArray<VideoCaptureDevice> videoCaptureDeviceList() const;
  bool isInitialized() const;
  operator bool() const {
    return isInitialized();
  }
 private:
  bool initialize();
  bool initQeditTypeLibraryImpl();
  bool initQeditTypeLibrary();
  bool initVideoCaptureDeviceList();
  bool m_isInitialized;
  COMManager m_comManager;
  nsAutoPtr<QeditTypeLibraryImpl> m_qeditTypeLibraryImpl;
  nsTArray<nsAutoPtr<VideoCaptureDeviceImpl>> m_videoCaptureDeviceImplList;
  QeditTypeLibrary m_qeditTypeLibrary;
  nsTArray<VideoCaptureDevice> m_videoCaptureDeviceList;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_MANAGER_IMPL_H
