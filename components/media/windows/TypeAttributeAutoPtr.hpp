#ifndef VIDEO_CAPTURE_TYPE_ATTRIBUTE_AUTO_PTR_H
#define VIDEO_CAPTURE_TYPE_ATTRIBUTE_AUTO_PTR_H

#include "oaidl.h"

namespace VideoCapture {

class TypeAttributeAutoPtr {
 public:
  TypeAttributeAutoPtr();
  TypeAttributeAutoPtr(ITypeInfo* const pTypeInfo);
  TypeAttributeAutoPtr(TypeAttributeAutoPtr& typeAttributeAutoPtr);
  TypeAttributeAutoPtr& operator=(TypeAttributeAutoPtr& typeAttributeAutoPtr) {
    swap(typeAttributeAutoPtr);
    return (*this);
  }
  ~TypeAttributeAutoPtr();
  TYPEATTR* get() const;
  bool isInitialized() const;
  operator bool() const {
    return isInitialized();
  }
 private:
  void swap(TypeAttributeAutoPtr& typeAttributeAutoPtr);
  void initialize();
  ITypeInfo* m_pTypeInfo;
  TYPEATTR* m_pTypeAttribute;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_TYPE_ATTRIBUTE_AUTO_PTR_H
