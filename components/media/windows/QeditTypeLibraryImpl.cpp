#include "QeditTypeLibraryImpl.hpp"

namespace VideoCapture {

static const std::wstring s_kTypeLibraryName(L"qedit.dll");

static const std::wstring s_kIID_ISampleGrabberName(L"ISampleGrabber");

static const std::wstring s_kIID_ISampleGrabberCBName(L"ISampleGrabberCB");

static const std::wstring s_kCLSID_SampleGrabberName(L"SampleGrabber");

static const std::wstring s_kCLSID_NullRendererName(L"NullRenderer");

static const std::vector<const std::wstring> interfaceIdentifierNames();

static const std::vector<const std::wstring> classIdentifierNames();

static const std::vector<const std::wstring> interfaceIdentifierNames() {
  std::vector<const std::wstring> names;
  names.push_back(s_kIID_ISampleGrabberName);
  names.push_back(s_kIID_ISampleGrabberCBName);
  return names;
}

static const std::vector<const std::wstring> classIdentifierNames() {
  std::vector<const std::wstring> names;
  names.push_back(s_kCLSID_SampleGrabberName);
  names.push_back(s_kCLSID_NullRendererName);
  return names;
}

QeditTypeLibraryImpl::QeditTypeLibraryImpl()
    : TypeLibrary(
        s_kTypeLibraryName,
        interfaceIdentifierNames(),
        classIdentifierNames()) {

}

const IID QeditTypeLibraryImpl::IID_ISampleGrabber() const {
  return TypeLibrary::interfaceIdByName(s_kIID_ISampleGrabberName);
}

const IID QeditTypeLibraryImpl::IID_ISampleGrabberCB() const {
  return TypeLibrary::interfaceIdByName(s_kIID_ISampleGrabberCBName);
}

const CLSID QeditTypeLibraryImpl::CLSID_SampleGrabber() const {
  return TypeLibrary::classIdByName(s_kCLSID_SampleGrabberName);
}

const CLSID QeditTypeLibraryImpl::CLSID_NullRenderer() const {
  return TypeLibrary::classIdByName(s_kCLSID_NullRendererName);
}

} // VideoCapture
