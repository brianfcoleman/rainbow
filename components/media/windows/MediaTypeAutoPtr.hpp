#ifndef VIDEO_CAPTURE_MEDIA_TYPE_AUTO_PTR_H
#define VIDEO_CAPTURE_MEDIA_TYPE_AUTO_PTR_H

#include "dshow.h"

namespace VideoCapture {

class MediaTypeAutoPtr {
 public:
  MediaTypeAutoPtr();
  explicit MediaTypeAutoPtr(AM_MEDIA_TYPE* const pMediaType);
  MediaTypeAutoPtr& operator=(AM_MEDIA_TYPE* const pMediaType) {
    m_pMediaType = pMediaType;
    return (*this);
  }
  ~MediaTypeAutoPtr();
  AM_MEDIA_TYPE* get() const;
  bool isInitialized() const;
  operator bool() const {
    return isInitialized();
  }
 private:
  AM_MEDIA_TYPE* m_pMediaType;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_MEDIA_TYPE_AUTO_PTR_H
