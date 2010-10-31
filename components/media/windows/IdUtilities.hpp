#ifndef VIDEO_CAPTURE_ID_UTILITIES_H
#define VIDEO_CAPTURE_ID_UTILITIES_H

#include "nspr.h"

namespace VideoCapture {

const PRUint32 kInvalidId = -1;

bool isValidId(const PRUint32 id);

} // VideoCapture

#endif // VIDEO_CAPTURE_ID_UTILITIES_H
