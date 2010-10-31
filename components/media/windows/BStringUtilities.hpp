#ifndef VIDEO_CAPTURE_BSTRING_UTILITIES_H
#define VIDEO_CAPTURE_BSTRING_UTILITIES_H

#include <string>
#include "wtypes.h"
#include "oleauto.h"

namespace VideoCapture {

std::string utf8StringFromBasicString(const BSTR basicString);

} // VideoCapture

#endif // VIDEO_CAPTURE_BSTRING_UTILITIES_H
