#include "ByteBuffer.hpp"

namespace VideoCapture {
ByteBuffer::ByteBuffer(PRUint8* pFirstBufferByte, const PRInt32 sizeBytes)
    : m_pFirstBufferByte(pFirstBufferByte),
      m_sizeBytes(sizeBytes) {

}

PRUint8* ByteBuffer::firstBufferByte() const {
  return m_pFirstBufferByte;
}

PRInt32 ByteBuffer::sizeBytes() const {
  return m_sizeBytes;
}

bool ByteBuffer::hasData() const {
  if (!m_pFirstBufferByte) {
    return false;
  }
  PRUint8* byteArray = m_pFirstBufferByte;
  for (PRInt32 i = 0; i < m_sizeBytes; ++i) {
    PRUint8 byteValue = byteArray[i];
    if (byteValue) {
      return true;
    }
  }
  return false;
}

} // VideoCapture
