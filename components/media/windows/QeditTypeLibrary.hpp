#ifndef VIDEO_CAPTURE_QEDIT_TYPE_LIBRARY
#define VIDEO_CAPTURE_QEDIT_TYPE_LIBRARY

#include "oaidl.h"

namespace VideoCapture {

class QeditTypeLibraryImpl;

class QeditTypeLibrary {
public:
  QeditTypeLibrary(QeditTypeLibraryImpl* const pImpl);
  const IID IID_ISampleGrabber() const;
  const IID IID_ISampleGrabberCB() const;
  const CLSID CLSID_SampleGrabber() const;
  const CLSID CLSID_NullRenderer() const;
  operator bool() const {
    return isInitialized();
  }
  bool isInitialized() const;
private:
  QeditTypeLibraryImpl* m_pImpl;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_QEDIT_TYPE_LIBRARY
