#ifndef VIDEO_CAPTURE_TYPE_ATTRIBUTE_AUTO_PTR_H
#define VIDEO_CAPTURE_TYPE_ATTRIBUTE_AUTO_PTR_H

#include <algorithm>
#include "comdef.h"
#include "comdefsp.h"

namespace VideoCapture {

class TypeAttributeAutoPtr {
 public:
  TypeAttributeAutoPtr();
  TypeAttributeAutoPtr(const ITypeInfoPtr& typeInfo);
  TypeAttributeAutoPtr(TypeAttributeAutoPtr& typeAttributeAutoPtr);
  TypeAttributeAutoPtr& operator=(TypeAttributeAutoPtr& typeAttributeAutoPtr) {
    std::swap((*this), typeAttributeAutoPtr);
    return (*this);
  }
  ~TypeAttributeAutoPtr();
  TYPEATTR* get() const;
  bool isInitialized() const;
  operator bool() const {
    return isInitialized();
  }
 private:
  void initialize();
  ITypeInfoPtr m_typeInfo;
  TYPEATTR* m_pTypeAttribute;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_TYPE_ATTRIBUTE_AUTO_PTR_H
