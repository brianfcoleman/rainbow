#ifndef VIDEO_CAPTURE_STRING_GUID_PAIR_H
#define VIDEO_CAPTURE_STRING_GUID_PAIR_H

#include "oaidl.h"
#include "nsStringAPI.h"

namespace VideoCapture {

class StringGUIDPair {
 public:
  StringGUIDPair(const nsAString& string, const GUID& guid);
  nsString string() const;
  GUID guid() const;
  bool isEqual(const nsAString& string) const;
 private:
  nsString m_string;
  GUID m_guid;
};

bool operator==(const StringGUIDPair& pair, const nsAString& string);

bool operator==(const nsAString& string, const StringGUIDPair& pair);

bool operator!=(const StringGUIDPair& pair, const nsAString& string);

bool operator!=(const nsAString& string, const StringGUIDPair& pair);

} // VideoCapture

#endif // VIDEO_CAPTURE_STRING_GUID_PAIR_H
