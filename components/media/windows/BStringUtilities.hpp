#ifndef VIDEO_CAPTURE_BSTRING_UTILITIES_H
#define VIDEO_CAPTURE_BSTRING_UTILITIES_H

#include "wtypes.h"
#include "oleauto.h"
#include "nsStringAPI.h"

namespace VideoCapture {

nsString nsAStringFromBasicString(const BSTR basicString);

} // VideoCapture

#endif // VIDEO_CAPTURE_BSTRING_UTILITIES_H
