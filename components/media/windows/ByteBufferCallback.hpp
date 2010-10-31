#ifndef VIDEO_CAPTURE_BYTE_BUFFER_CALLBACK_H
#define VIDEO_CAPTURE_BYTE_BUFFER_CALLBACK_H

#include "ByteBuffer.hpp"

namespace VideoCapture {

class ByteBufferCallback {
 public:
  ByteBufferCallback();
  virtual ~ByteBufferCallback();
  virtual void onNewByteBuffer(const ByteBuffer& byteBuffer);
};

} // VideoCapture

#endif // VIDEO_CAPTURE_BYTE_BUFFER_CALLBACK_H
