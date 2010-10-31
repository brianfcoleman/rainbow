#ifndef QEDIT_TYPE_LIBRARY_IMPL
#define QEDIT_TYPE_LIBRARY_IMPL

#include "TypeLibrary.hpp"

namespace VideoCapture {

class QeditTypeLibraryImpl :
  public TypeLibrary {
public:
  QeditTypeLibraryImpl();
  const IID IID_ISampleGrabber() const;
  const IID IID_ISampleGrabberCB() const;
  const CLSID CLSID_SampleGrabber() const;
  const CLSID CLSID_NullRenderer() const;
};

} // VideoCapture

#endif // QEDIT_TYPE_LIBRARY_IMPL
