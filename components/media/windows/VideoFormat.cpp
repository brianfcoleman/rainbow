#include "VideoFormat.hpp"
#include "VideoFormatImpl.hpp"
#include "IdUtilities.hpp"

namespace VideoCapture {

VideoFormat::VideoFormat()
    : m_pImpl(0) {

}

VideoFormat::VideoFormat(VideoFormatImpl* const pImpl)
    : m_pImpl(pImpl) {

}

bool VideoFormat::isInitialized() const {
  if (!m_pImpl) {
    return false;
  }
  if (!m_pImpl->isInitialized()) {
    return false;
  }
  return true;
}

PRInt32 VideoFormat::id() const {
  if (!isInitialized()) {
    return kInvalidId;
  }
  return m_pImpl->id();
}

double VideoFormat::framesPerSecond() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_pImpl->framesPerSecond();
}

IntegerSize VideoFormat::sizePixels() const {
  if (!isInitialized()) {
    IntegerSize zeroSize;
    return zeroSize;
  }
  return m_pImpl->sizePixels();
}

PRInt32 VideoFormat::angleRotationDegrees() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_pImpl->angleRotationDegrees();
}

PRInt32 VideoFormat::bitsPerPixel() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_pImpl->bitsPerPixel();
}

PRInt32 VideoFormat::sizeBytes() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_pImpl->sizeBytes();
}

PRInt32 VideoFormat::sizeRowBytes() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_pImpl->sizeRowBytes();
}

RGBFormat VideoFormat::rgbFormat() const {
  if (!isInitialized()) {
    return kRGBNone;
  }
  return m_pImpl->rgbFormat();
}

VideoFormatImpl* VideoFormat::get() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_pImpl;
}

} // VideoCapture

