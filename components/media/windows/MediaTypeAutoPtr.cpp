#include "MediaTypeAutoPtr.hpp"

namespace VideoCapture {

static void DeleteMediaType(AM_MEDIA_TYPE* pMediaType);

static void FreeMediaType(AM_MEDIA_TYPE& mediaType);

MediaTypeAutoPtr::MediaTypeAutoPtr()
    : m_pMediaType(0) {

}

MediaTypeAutoPtr::MediaTypeAutoPtr(AM_MEDIA_TYPE* const pMediaType)
    : m_pMediaType(pMediaType) {

}

MediaTypeAutoPtr::MediaTypeAutoPtr(MediaTypeAutoPtr& mediaTypeAutoPtr)
    : m_pMediaType(0) {
  std::swap((*this), mediaTypeAutoPtr);
}

MediaTypeAutoPtr::~MediaTypeAutoPtr() {
  if (!isInitialized()) {
      return;
  }
  DeleteMediaType(m_pMediaType);
}

AM_MEDIA_TYPE* MediaTypeAutoPtr::get() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_pMediaType;
}

bool MediaTypeAutoPtr::isInitialized() const {
  if (!m_pMediaType) {
    return false;
  }
  return true;
}

static void DeleteMediaType(AM_MEDIA_TYPE* pMediaType) {
  if (!pMediaType) {
    return;
  }
  FreeMediaType(*(pMediaType));
  CoTaskMemFree(reinterpret_cast<void*>(pMediaType));
}

static void FreeMediaType(AM_MEDIA_TYPE& mediaType) {
  if (mediaType.cbFormat) {
      CoTaskMemFree(reinterpret_cast<void*>(mediaType.pbFormat));
      mediaType.cbFormat = 0;
      mediaType.pbFormat = 0;
  }
  if (mediaType.pUnk) {
      mediaType.pUnk->Release();
      mediaType.pUnk = 0;
  }
}

} // VideoCapture
