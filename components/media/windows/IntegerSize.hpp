#ifndef VIDEOCAPTURE_INTEGER_SIZE_H
#define VIDEOCAPTURE_INTEGER_SIZE_H

#include "nspr.h"

namespace VideoCapture {

class IntegerSize {
 public:
  IntegerSize();
  IntegerSize(
      const PRInt32 width,
      const PRInt32 height);
  PRInt32 width() const;
  PRInt32 height() const;
  bool isValid() const;
  operator bool() const {
    return isValid();
  }
 private:
  PRInt32 m_width;
  PRInt32 m_height;
};

bool operator==(const IntegerSize& lhs, const IntegerSize& rhs);

bool operator!=(const IntegerSize& lhs, const IntegerSize& rhs);

} // VideoCapture

#endif // VIDEOCAPTURE_INTEGER_SIZE_H
