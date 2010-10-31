#include "VariantAutoPtr.hpp"

namespace VideoCapture {

VariantAutoPtr::VariantAutoPtr() {
  VariantInit(&m_variant);
}

VariantAutoPtr::~VariantAutoPtr() {
  VariantClear(&m_variant);
}

VARIANT* VariantAutoPtr::get() {
  return &m_variant;
}

} // VideoCapture
