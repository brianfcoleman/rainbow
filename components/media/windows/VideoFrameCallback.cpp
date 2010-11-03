#include "VideoFrameCallback.hpp"
#ifdef VIDEO_CAPTURE_DEBUG
#include <cstdio>
#endif

namespace VideoCapture {

VideoFrameCallback::VideoFrameCallback() {

}

VideoFrameCallback::~VideoFrameCallback() {

}

void VideoFrameCallback::onNewVideoFrame(
    const ByteBuffer& byteBuffer,
    const RGBVideoFormat& videoFormat) {
#ifdef VIDEO_CAPTURE_DEBUG
  printf(
      "VideoFrameCallback::onNewVideoFrame byteBuffer.sizeBytes(): %i\n",
      byteBuffer.sizeBytes());
#endif
}

} // VideoCapture
