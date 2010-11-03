#ifndef VIDEO_CAPTURE_VIDEO_FRAME_CALLBACK_H
#define VIDEO_CAPTURE_VIDEO_FRAME_CALLBACK_H

#include "ByteBuffer.hpp"
#include "RGBVideoFormat.hpp"

namespace VideoCapture {

class VideoFrameCallback {
 public:
  VideoFrameCallback();
  virtual ~VideoFrameCallback();
  virtual void onNewVideoFrame(
      const ByteBuffer& byteBuffer,
      const RGBVideoFormat& videoFormat);
};

} // VideoCapture

#endif // VIDEO_CAPTURE_VIDEO_FRAME_CALLBACK_H
