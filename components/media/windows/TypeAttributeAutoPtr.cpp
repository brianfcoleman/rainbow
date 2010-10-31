#include "TypeAttributeAutoPtr.hpp"

namespace VideoCapture {

TypeAttributeAutoPtr::TypeAttributeAutoPtr()
    : m_pTypeInfo(0),
      m_pTypeAttribute(0) {

}

TypeAttributeAutoPtr::TypeAttributeAutoPtr(ITypeInfo* const pTypeInfo)
    : m_pTypeInfo(0),
      m_pTypeAttribute(0) {
  initialize();
}

TypeAttributeAutoPtr::TypeAttributeAutoPtr(
    TypeAttributeAutoPtr& typeAttributeAutoPtr)
    : m_pTypeInfo(0),
      m_pTypeAttribute(0) {
  swap(typeAttributeAutoPtr);
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

void TypeAttributeAutoPtr::swap(TypeAttributeAutoPtr& typeAttributeAutoPtr) {
  ITypeInfo* pTypeInfo = m_pTypeInfo;
  m_pTypeInfo = typeAttributeAutoPtr.m_pTypeInfo;
  typeAttributeAutoPtr.m_pTypeInfo = pTypeInfo;
  TYPEATTR* pTypeAttribute = m_pTypeAttribute;
  m_pTypeAttribute = typeAttributeAutoPtr.m_pTypeAttribute;
  typeAttributeAutoPtr.m_pTypeAttribute = pTypeAttribute;
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
