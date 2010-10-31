#ifndef VIDEO_CAPTURE_BYTE_BUFFER_H
#define VIDEO_CAPTURE_BYTE_BUFFER_H

#include "nspr.h"

namespace VideoCapture {

class ByteBuffer {
 public:
  ByteBuffer(PRUint8* pFirstBufferByte, const PRInt32 sizeBytes);
  PRUint8* firstBufferByte() const;
  PRInt32 sizeBytes() const;
  bool hasData() const;
 private:
  PRUint8* m_pFirstBufferByte;
  PRInt32 m_sizeBytes;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_BYTE_BUFFER_H
