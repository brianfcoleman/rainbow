#include "VideoCaptureDevice.hpp"
#include "VideoCaptureDeviceImpl.hpp"

namespace VideoCapture {

const std::string VideoCaptureDevice::s_kEmptyString("");

VideoCaptureDevice::VideoCaptureDevice()
    : m_pImpl(0) {

}

VideoCaptureDevice::VideoCaptureDevice(
    VideoCaptureDeviceImpl* const pImpl)
    : m_pImpl(pImpl) {

}

bool VideoCaptureDevice::isInitialized() const {
  if (!m_pImpl) {
    return false;
  }
  if (!m_pImpl->isInitialized()) {
    return false;
  }
}

std::string VideoCaptureDevice::name() const {
  if (!isInitialized()) {
    return VideoCaptureDevice::s_kEmptyString;
  }
  return m_pImpl->name();
}

bool VideoCaptureDevice::addOnNewVideoFrameCallback(
    ByteBufferCallback* const pCallback) {
  if (!isInitialized()) {
    return false;
  }
  return m_pImpl->addOnNewVideoFrameCallback(pCallback);
}

bool VideoCaptureDevice::removeOnNewVideoFrameCallback(
    ByteBufferCallback* const pCallback) {
  if (!isInitialized()) {
    return false;
  }
  return m_pImpl->removeOnNewVideoFrameCallback(pCallback);
}

bool VideoCaptureDevice::startCapturing() {
  if (!isInitialized()) {
    return false;
  }
  return m_pImpl->startCapturing();
}

bool VideoCaptureDevice::stopCapturing() {
  if (!isInitialized()) {
    return false;
  }
  return m_pImpl->stopCapturing();
}

double VideoCaptureDevice::countFramesCapturedPerSecond() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_pImpl->countFramesCapturedPerSecond();
}

std::list<RGBVideoFormat> VideoCaptureDevice::videoFormatList() const {
  if (!isInitialized()) {
    std::list<RGBVideoFormat> emptyList;
    return emptyList;
  }
  return m_pImpl->videoFormatList();
}

RGBVideoFormat VideoCaptureDevice::currentVideoFormat() const {
  if (!isInitialized()) {
    RGBVideoFormat videoFormat;
    return videoFormat;
  }
  return m_pImpl->currentVideoFormat();
}

bool VideoCaptureDevice::setCurrentVideoFormat(
    const RGBVideoFormat& rgbVideoFormat) {
  if (!isInitialized()) {
    return false;
  }
  return m_pImpl->setCurrentVideoFormat(rgbVideoFormat);
}

} // VideoCapture
