#ifndef VIDEO_CAPTURE_GUID_UTILITIES_H
#define VIDEO_CAPTURE_GUID_UTILITIES_H

#include "oaidl.h"

namespace VideoCapture {

const GUID emptyGUID();

bool isEmptyGUID(const GUID& guid);

} // VideoCapture

#endif // VIDEO_CAPTURE_GUID_UTILITIES_H
