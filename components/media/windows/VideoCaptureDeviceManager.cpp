#include "VideoCaptureDeviceManager.hpp"
#include "VideoCaptureDeviceManagerImpl.hpp"

namespace VideoCapture {

VideoCaptureDeviceManager
VideoCaptureDeviceManager::createVideoCapptureDeviceManager() {
  static nsAutoPtr<VideoCaptureDeviceManagerImpl> pImpl(
      new VideoCaptureDeviceManagerImpl());
  VideoCaptureDeviceManager videoCaptureDeviceManager(pImpl);
  return videoCaptureDeviceManager;
}

VideoCaptureDeviceManager::VideoCaptureDeviceManager() {

}

VideoCaptureDeviceManager::VideoCaptureDeviceManager(
    VideoCaptureDeviceManagerImpl* pImpl)
    : m_pImpl(pImpl) {

}

VideoCaptureDeviceManager::~VideoCaptureDeviceManager() {

}

nsTArray<VideoCaptureDevice>
VideoCaptureDeviceManager::videoCaptureDeviceList() const {
  if (!isInitialized()) {
    nsTArray<VideoCaptureDevice> emptyList;
    return emptyList;
  }
  return m_pImpl->videoCaptureDeviceList();
}

bool VideoCaptureDeviceManager::isInitialized() const {
  if (!m_pImpl) {
    return false;
  }
  return m_pImpl->isInitialized();
}

} // VideoCapture
