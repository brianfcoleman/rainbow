#include "IntegerSize.hpp"

namespace VideoCapture {

IntegerSize::IntegerSize()
    : m_width(0),
      m_height(0) {

}

IntegerSize::IntegerSize(
    const PRInt32 width,
    const PRInt32 height)
    : m_width(width),
      m_height(height) {

}

PRInt32 IntegerSize::width() const {
  return m_width;
}

PRInt32 IntegerSize::height() const {
  return m_height;
}

bool IntegerSize::isValid() const {
  if (m_width <= 0) {
    return false;;
  }
  if (m_height <= 0) {
    return false;
  }
  return true;
}

bool operator==(const IntegerSize& lhs, const IntegerSize& rhs) {
  if (!lhs) {
    return false;
  }
  if (!rhs) {
    return false;
  }
  if (lhs.width() != rhs.width()) {
    return false;
  }
  if (lhs.height() != rhs.height()) {
    return false;
  }
  return true;
}

bool operator!=(const IntegerSize& lhs, const IntegerSize& rhs) {
  if (!lhs) {
    return true;
  }
  if (!rhs) {
    return true;
  }
  if (lhs.width() == rhs.width()) {
    return false;
  }
  if (lhs.height() == rhs.height()) {
    return false;
  }
  return true;
}

} // VideoCapture
