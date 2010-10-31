#include "QeditTypeLibraryImpl.hpp"

namespace VideoCapture {

static const nsString s_kTypeLibraryName(NS_LITERAL_STRING("qedit.dll"));

static const nsString s_kIID_ISampleGrabberName(
    NS_LITERAL_STRING("ISampleGrabber"));

static const nsString s_kIID_ISampleGrabberCBName(
    NS_LITERAL_STRING("ISampleGrabberCB"));

static const nsString s_kCLSID_SampleGrabberName(
    NS_LITERAL_STRING("SampleGrabber"));

static const nsString s_kCLSID_NullRendererName(
    NS_LITERAL_STRING("NullRenderer"));

static const nsTArray<nsString> interfaceIdentifierNames();

static const nsTArray<nsString> classIdentifierNames();

static const nsTArray<nsString> interfaceIdentifierNames() {
  nsTArray<nsString> names;
  names.AppendElement(s_kIID_ISampleGrabberName);
  names.AppendElement(s_kIID_ISampleGrabberCBName);
  return names;
}

static const nsTArray<nsString> classIdentifierNames() {
  nsTArray<nsString> names;
  names.AppendElement(s_kCLSID_SampleGrabberName);
  names.AppendElement(s_kCLSID_NullRendererName);
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
