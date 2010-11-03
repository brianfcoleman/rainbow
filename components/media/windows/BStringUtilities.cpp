#include "BStringUtilities.hpp"
#include "nsAutoPtr.h"
#ifdef VIDEO_CAPTURE_DEBUG
#include <cstdio>
#endif

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
#ifdef VIDEO_CAPTURE_DEBUG
  printf(
      "nsAStringFromBasicString countCharsBasicString: %u\n",
      countCharsBasicString);
#endif
  nsString string(basicString, countCharsBasicString);
#ifdef VIDEO_CAPTURE_DEBUG
  NS_LossyConvertUTF16toASCII_external convertedString(string);
  printf("nsAStringFromBasicString convertedString: %s\n", convertedString);
#endif
  return string;
}

} // VideoCapture
