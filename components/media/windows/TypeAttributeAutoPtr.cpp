#include "TypeAttributeAutoPtr.hpp"

namespace VideoCapture {

TypeAttributeAutoPtr::TypeAttributeAutoPtr(ITypeInfo* const pTypeInfo)
    : m_pTypeInfo(pTypeInfo),
      m_pTypeAttribute(0) {
  initialize();
}

TypeAttributeAutoPtr::~TypeAttributeAutoPtr() {
  if (!isInitialized()) {
    return;
  }
  m_pTypeInfo->ReleaseTypeAttr(m_pTypeAttribute);
}

TYPEATTR* TypeAttributeAutoPtr::get() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_pTypeAttribute;
}

bool TypeAttributeAutoPtr::isInitialized() const {
  if (!m_pTypeInfo) {
    return false;
  }
  if (!m_pTypeAttribute) {
    return false;
  }
  return true;
}

void TypeAttributeAutoPtr::initialize() {
  if (!m_pTypeInfo) {
    return;
  }
  HRESULT result = m_pTypeInfo->GetTypeAttr(
      static_cast<TYPEATTR FAR* FAR*>(&m_pTypeAttribute));
  if (result == S_OK) {
    return;
  }
  m_pTypeInfo = 0;
  m_pTypeAttribute = 0;
}

} // VideoCapture
