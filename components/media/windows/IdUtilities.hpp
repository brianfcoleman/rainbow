#ifndef VIDEO_CAPTURE_ID_UTILITIES_H
#define VIDEO_CAPTURE_ID_UTILITIES_H

#include "nspr.h"

namespace VideoCapture {

const PRInt32 kInvalidId = -1;

bool isValidId(const PRInt32 id);

} // VideoCapture

#endif // VIDEO_CAPTURE_ID_UTILITIES_H
