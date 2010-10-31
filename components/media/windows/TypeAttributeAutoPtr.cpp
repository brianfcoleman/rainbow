#include "TypeAttributeAutoPtr.hpp"

namespace VideoCapture {

TypeAttributeAutoPtr::TypeAttributeAutoPtr()
    : m_pTypeAttribute(0) {

}

TypeAttributeAutoPtr::TypeAttributeAutoPtr(const ITypeInfoPtr& typeInfo)
    : m_pTypeAttribute(0) {
  initialize();
}

TypeAttributeAutoPtr::TypeAttributeAutoPtr(
    TypeAttributeAutoPtr& typeAttributeAutoPtr)
    : m_pTypeAttribute(0) {
  swap(typeAttributeAutoPtr);
}

TypeAttributeAutoPtr::~TypeAttributeAutoPtr() {
  if (!isInitialized()) {
    return;
  }
  m_typeInfo->ReleaseTypeAttr(m_pTypeAttribute);
}

TYPEATTR* TypeAttributeAutoPtr::get() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_pTypeAttribute;
}

bool TypeAttributeAutoPtr::isInitialized() const {
  if (!m_typeInfo) {
    return false;
  }
  if (!m_pTypeAttribute) {
    return false;
  }
  return true;
}

void TypeAttributeAutoPtr::swap(TypeAttributeAutoPtr& typeAttributeAutoPtr) {
  TYPEATTR* pTypeAttribute = m_pTypeAttribute;
  m_pTypeAttribute = typeAttributeAutoPtr.m_pTypeAttribute;
  typeAttributeAutoPtr.m_pTypeAttribute = pTypeAttribute;
}

void TypeAttributeAutoPtr::initialize() {
  if (!m_typeInfo) {
    return;
  }
  HRESULT result = m_typeInfo->GetTypeAttr(
      static_cast<TYPEATTR FAR* FAR*>(&m_pTypeAttribute));
  if (result == S_OK) {
    return;
  }
  m_pTypeAttribute = 0;
}

} // VideoCapture
