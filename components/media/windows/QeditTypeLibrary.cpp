#include "QeditTypeLibrary.hpp"
#include "QeditTypeLibraryImpl.hpp"
#include "GUIDUtilities.hpp"

namespace VideoCapture {

QeditTypeLibrary::QeditTypeLibrary()
    : m_pImpl(0) {

}

QeditTypeLibrary::QeditTypeLibrary(QeditTypeLibraryImpl* const pImpl)
    : m_pImpl(pImpl) {

}

const IID QeditTypeLibrary::IID_ISampleGrabber() const {
  if (!isInitialized()) {
    return emptyGUID();
  }
  return m_pImpl->IID_ISampleGrabber();
}

const IID QeditTypeLibrary::IID_ISampleGrabberCB() const {
  if (!isInitialized()) {
    return emptyGUID();
  }
  return m_pImpl->IID_ISampleGrabberCB();
}

const CLSID QeditTypeLibrary::CLSID_SampleGrabber() const {
  if (!isInitialized()) {
    return emptyGUID();
  }
  return m_pImpl->CLSID_SampleGrabber();
}

const CLSID QeditTypeLibrary::CLSID_NullRenderer() const {
  if (!isInitialized()) {
    return emptyGUID();
  }
  return m_pImpl->CLSID_NullRenderer();
}

bool QeditTypeLibrary::isInitialized() const {
  if (!m_pImpl) {
    return false;
  }
  if (!m_pImpl->isInitialized()) {
    return false;
  }
  return true;
}

}
