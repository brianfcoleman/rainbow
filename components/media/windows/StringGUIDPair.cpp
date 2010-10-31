#include "StringGUIDPair.hpp"

namespace VideoCapture {

StringGUIDPair::StringGUIDPair(const nsAString& string, const GUID& guid)
    : m_string(string),
      m_guid(guid) {

}

nsString StringGUIDPair::string() const {
  return m_string;
}

GUID StringGUIDPair::guid() const {
  return m_guid;
}

bool StringGUIDPair::isEqual(const nsAString& string) const {
  if (string.IsEmpty()) {
    return false;
  }
  if (string.IsVoid()) {
    return false;
  }
  if (m_string.IsEmpty()) {
    return false;
  }
  if (m_string.IsVoid()) {
    return false;
  }
  if (!m_string.Equals(string)) {
    return false;
  }
  return true;
}

bool operator==(const StringGUIDPair& pair, const nsAString& string) {
  return pair.isEqual(string);
}

bool operator==(const nsAString& string, const StringGUIDPair& pair) {
  return pair.isEqual(string);
}

bool operator!=(const StringGUIDPair& pair, const nsAString& string) {
  return !pair.isEqual(string);
}

bool operator!=(const nsAString& string, const StringGUIDPair& pair) {
  return !pair.isEqual(string);
}

} // VideoCapture
