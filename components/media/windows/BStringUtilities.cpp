#include "BStringUtilities.hpp"
#include "nsAutoPtr.h"

namespace VideoCapture {

static const std::string kEmptyString("");

std::string utf8StringFromBasicString(const BSTR basicString) {
  if (!basicString) {
    return kEmptyString;
  }
  // This length does not include a nul terminator
  UINT countCharsBasicString = SysStringLen(basicString);
  if (!countCharsBasicString) {
    return kEmptyString;
  }
  UINT codePage = CP_UTF8;
  DWORD flags = 0;
  LPCWSTR wideCharString = static_cast<LPCWSTR>(basicString);
  int countCharsWideCharString = static_cast<int>(countCharsBasicString);
  LPSTR pUTF8String = 0;
  int countBytesUTF8String = 0;
  LPCSTR pDefaultChar = 0;
  LPBOOL pUsedDefaultChar = 0;
  countBytesUTF8String = WideCharToMultiByte(
      codePage,
      flags,
      wideCharString,
      countCharsWideCharString,
      pUTF8String,
      countBytesUTF8String,
      pDefaultChar,
      pUsedDefaultChar);
  if (countBytesUTF8String <= 0) {
   return kEmptyString;
  }
  // Allocate an extra char for the nul terminator
  int countBytesUTF8StringWithNulTerminator = countBytesUTF8String + 1;
  nsAutoArrayPtr<char> charArrayUTF8String(
      new char[countBytesUTF8StringWithNulTerminator]);
  if (!charArrayUTF8String) {
    return kEmptyString;
  }
  pUTF8String = static_cast<LPSTR>(charArrayUTF8String.get());
  WideCharToMultiByte(
      codePage,
      flags,
      wideCharString,
      countCharsWideCharString,
      pUTF8String,
      countBytesUTF8String,
      pDefaultChar,
      pUsedDefaultChar);
  // Append the nul terminator
  int indexNulTerminator = countBytesUTF8StringWithNulTerminator - 1;
  charArrayUTF8String[indexNulTerminator] = '\0';
  const std::string utf8String(charArrayUTF8String.get());
  return utf8String;
}

} // VideoCapture
