#include "VideoCaptureDeviceManagerImpl.hpp"
#include "QeditTypeLibraryImpl.hpp"
#include "VideoCaptureDeviceImpl.hpp"

namespace VideoCapture {

VideoCaptureDeviceManagerImpl::VideoCaptureDeviceManagerImpl()
    : m_isInitialized(false) {
  m_isInitialized = initialize();
}

bool VideoCaptureDeviceManagerImpl::initialize() {
  if (!m_comManager) {
    return false;
  }

  if (!initQeditTypeLibrary()) {
    return false;
  }

  if (!initVideoCaptureDeviceList()) {
    return false;
  }

  return true;
}

bool VideoCaptureDeviceManagerImpl::initQeditTypeLibrary() {
  if (!initQeditTypeLibraryImpl()) {
    return false;
  }
  QeditTypeLibrary qeditTypeLibrary(m_qeditTypeLibraryImpl.get());
  if (!qeditTypeLibrary) {
    return false;
  }
  m_qeditTypeLibrary = qeditTypeLibrary;
  return true;
}

bool VideoCaptureDeviceManagerImpl::initQeditTypeLibraryImpl() {
  QeditTypeLibraryImpl* pQeditTypeLibraryImpl = new QeditTypeLibraryImpl;
  if (!pQeditTypeLibraryImpl) {
    return false;
  }
  nsAutoPtr<QeditTypeLibraryImpl> qeditTypeLibraryImplPtr(
      pQeditTypeLibraryImpl);
  m_qeditTypeLibraryImpl = qeditTypeLibraryImplPtr;
  return true;
}

bool VideoCaptureDeviceManagerImpl::initVideoCaptureDeviceList() {
	COMAutoPtr<ICreateDevEnum> systemDeviceEnumeratorPtr(
      createInstanceCOMInterface<ICreateDevEnum>(
          CLSID_SystemDeviceEnum,
          IID_ICreateDevEnum));
	if (!systemDeviceEnumeratorPtr) {
		return false;
	}

  IEnumMoniker* pVideoInputDeviceEnumerator = 0;
  PRUint32 flags = 0;
  ICreateDevEnum* pSystemDeviceEnumerator = systemDeviceEnumeratorPtr.get();
	HRESULT result = pSystemDeviceEnumerator->CreateClassEnumerator(
		CLSID_VideoInputDeviceCategory,
		&pVideoInputDeviceEnumerator,
		flags);
	if (result != S_OK) {
		return false;
	}
  COMAutoPtr<IEnumMoniker> videoInputDeviceEnumeratorPtr(
      pVideoInputDeviceEnumerator);
	if (!videoInputDeviceEnumeratorPtr) {
		return false;
	}

	do {
		IMoniker* pMoniker;
    ULONG countElements = 1;
    ULONG countElementsFetched = 0;
		result = pVideoInputDeviceEnumerator->Next(
        countElements,
        &pMoniker,
        &countElementsFetched);
		if (result != S_OK)	{
			break;
		}
    COMAutoPtr<IMoniker> videoInputDeviceMonikerPtr(pMoniker);
		if (!videoInputDeviceMonikerPtr) {
			continue;
		}
    VideoCaptureDeviceImpl* pVideoCaptureDeviceImpl = new VideoCaptureDeviceImpl(
        m_qeditTypeLibrary,
        videoInputDeviceMonikerPtr);
		if (!pVideoCaptureDeviceImpl)	{
			continue;
		}
		if (!m_videoCaptureDeviceImplList.AppendElement(pVideoCaptureDeviceImpl)) {
      nsAutoPtr<VideoCaptureDeviceImpl> videoCaptureDeviceImplPtr(
        pVideoCaptureDeviceImpl);
      continue;
    }
    VideoCaptureDevice videoCaptureDevice(pVideoCaptureDeviceImpl);
    if (!videoCaptureDevice) {
      continue;
    }
    if (!m_videoCaptureDeviceList.AppendElement(videoCaptureDevice)) {
      continue;
    }
  }	while (result == S_OK);

  if (m_videoCaptureDeviceList.IsEmpty()) {
    return false;
  }
  return true;
}

bool VideoCaptureDeviceManagerImpl::isInitialized() const {
  return m_isInitialized;
}

nsTArray<VideoCaptureDevice>
VideoCaptureDeviceManagerImpl::videoCaptureDeviceList() const {
  if (!isInitialized()) {
    nsTArray<VideoCaptureDevice> emptyList;
    return emptyList;
  }

  return m_videoCaptureDeviceList;
}

} // VideoCapture
