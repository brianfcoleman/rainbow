#ifndef VIDEO_CAPTURE_VARIANT_AUTO_PTR_H
#define VIDEO_CAPTURE_VARIANT_AUTO_PTR_H

#include "wtypes.h"
#include "oleauto.h"
#include "Uncopyable.hpp"

namespace VideoCapture {

class VariantAutoPtr : private Uncopyable {
 public:
  VariantAutoPtr();
  ~VariantAutoPtr();
  VARIANT* get();
 private:
  VARIANT m_variant;
};

}

#endif // VIDEO_CAPTURE_VARIANT_AUTO_PTR_H
