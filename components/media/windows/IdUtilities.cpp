#include "IdUtilities.hpp"

namespace VideoCapture {

bool isValidId(const PRUint32 id) {
  if (id < 0) {
    return false;
  }
  return true;
}

} // VideoCapture
