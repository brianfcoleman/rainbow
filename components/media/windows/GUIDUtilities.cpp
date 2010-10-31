#include "GUIDUtilities.hpp"

#include "nspr.h"

namespace VideoCapture {

static const PRInt32 kCountElementsData4GUID = 8;

const GUID emptyGUID() {
  GUID guid;
  guid.Data1 = 0;
  guid.Data2 = 0;
  guid.Data3 = 0;
  for (PRInt32 i = 0; i < kCountElementsData4GUID; ++i) {
    guid.Data4[i] = 0;
  }
  return guid;
}

bool isEmptyGUID(const GUID& guid) {
  bool isEmpty = true;
  if (guid.Data1) {
    return !isEmpty;
  }
  if (guid.Data2) {
    return !isEmpty;
  }
  if (guid.Data3) {
    return !isEmpty;
  }
  for (PRInt32 i = 0; i < kCountElementsData4GUID; ++i) {
    if (guid.Data4[i]) {
      return !isEmpty;
    }
  }
  return isEmpty;
}

} // VideoCapture
