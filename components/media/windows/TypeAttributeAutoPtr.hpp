#ifndef VIDEO_CAPTURE_TYPE_ATTRIBUTE_AUTO_PTR_H
#define VIDEO_CAPTURE_TYPE_ATTRIBUTE_AUTO_PTR_H

#include "oaidl.h"
#include "Uncopyable.hpp"

namespace VideoCapture {

class TypeAttributeAutoPtr : private Uncopyable {
 public:
  explicit TypeAttributeAutoPtr(ITypeInfo* const pTypeInfo);
  ~TypeAttributeAutoPtr();
  TYPEATTR* get() const;
  bool isInitialized() const;
  operator bool() const {
    return isInitialized();
  }
 private:
  void initialize();
  ITypeInfo* m_pTypeInfo;
  TYPEATTR* m_pTypeAttribute;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_TYPE_ATTRIBUTE_AUTO_PTR_H
