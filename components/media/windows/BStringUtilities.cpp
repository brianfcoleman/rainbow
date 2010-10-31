#include "BStringUtilities.hpp"
#include "nsAutoPtr.h"

namespace VideoCapture {

static const nsString kEmptyString(NS_LITERAL_STRING(""));

nsString nsAStringFromBasicString(const BSTR basicString) {
  if (!basicString) {
    return kEmptyString;
  }
  // This length does not include a nul terminator
  UINT countCharsBasicString = SysStringLen(basicString);
  if (!countCharsBasicString) {
    return kEmptyString;
  }
  nsString string(basicString, countCharsBasicString);
  return string;
}

} // VideoCapture
