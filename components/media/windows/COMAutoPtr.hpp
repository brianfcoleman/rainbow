#ifndef VIDEO_CAPTURE_COM_AUTO_PTR_H
#define VIDEO_CAPTURE_COM_AUTO_PTR_H

#include <oaidl.h>

namespace VideoCapture {

template<typename COMInterface> class COMAutoPtr {
 public:
  COMAutoPtr()
      : m_pCOMInterface(0) {

  }

  COMAutoPtr(COMInterface* const pCOMInterface)
      : m_pCOMInterface(pCOMInterface) {

  }

  COMAutoPtr(COMAutoPtr& comAutoPtr)
      : m_pCOMInterface(comAutoPtr.forget()) {

  }

  COMAutoPtr& operator=(COMAutoPtr& comAutoPtr) {
    m_pCOMInterface = comAutoPtr.forget();
    return (*this);
  }

  ~COMAutoPtr() {
    if (!isInitialized()) {
      return;
    }
    m_pCOMInterface->Release();
  }

  COMInterface* get() const {
    if (!isInitialized()) {
      return 0;
    }
    return m_pCOMInterface;
  }

  COMInterface* forget() {
    COMInterface* pCOMInterface = m_pCOMInterface;
    m_pCOMInterface = 0;
    return pCOMInterface;
  }

  bool isInitialized() const {
    if (!m_pCOMInterface) {
      return false;
    }
    return true;
  }

  operator bool() const {
    return isInitialized();
  }
 private:
  COMInterface* m_pCOMInterface;
};

template<typename COMInterface> COMAutoPtr<
COMInterface> createInstanceCOMInterface(
      const CLSID& classId,
      const IID& interfaceId) {
  COMInterface* pCOMInterface = 0;
  HRESULT result = CoCreateInstance(
      classId,
      0,
      CLSCTX_INPROC_SERVER,
      interfaceId,
      reinterpret_cast<void**>(&pCOMInterface));
  if (FAILED(result)) {
    COMAutoPtr<COMInterface> comAutoPtr;
    return comAutoPtr;
  }
  COMAutoPtr<COMInterface> comAutoPtr(pCOMInterface);
  return comAutoPtr;
}

template<
  typename ParentCOMInterface,
  typename ChildCOMInterface> COMAutoPtr<ChildCOMInterface> queryCOMInterface(
      ParentCOMInterface* pParentCOMInterface,
      const IID& childInterfaceId) {
  if (!pParentCOMInterface) {
    COMAutoPtr<ChildCOMInterface> comAutoPtr;
    return comAutoPtr;
  }
  ChildCOMInterface* pChildCOMInterface = 0;
  HRESULT result = pParentCOMInterface->QueryInterface(
      childInterfaceId,
      reinterpret_cast<void**>(&pChildCOMInterface));
  if (FAILED(result)) {
    COMAutoPtr<ChildCOMInterface> comAutoPtr;
    return comAutoPtr;
  }
  COMAutoPtr<ChildCOMInterface> comAutoPtr(pChildCOMInterface);
  return comAutoPtr;
}

} // VideoCapture

#endif // VIDEO_CAPTURE_COM_AUTO_PTR_H
