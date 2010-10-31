#ifndef VIDEO_CAPTURE_TYPE_LIBRARY_H
#define VIDEO_CAPTURE_TYPE_LIBRARY_H

#include "nsTArray.h"
#include "StringGUIDPair.hpp"
#include "Uncopyable.hpp"

namespace VideoCapture {

class TypeLibrary :
  private Uncopyable {
public:
  TypeLibrary(
      const nsAString& typeLibraryName,
      const nsTArray<nsString>& interfaceIdentifierNames,
      const nsTArray<nsString>& classIdentifierNames);
  virtual ~TypeLibrary();
  IID interfaceIdByName(const nsAString& name) const;
  CLSID classIdByName(const nsAString& name) const;
  bool isInitialized() const;
  operator bool() const {
    return isInitialized();
  }
private:
  nsTArray<StringGUIDPair> m_interfaceIdentifiersByName;
  nsTArray<StringGUIDPair> m_classIdentifiersByName;
  bool m_isInitialized;
  bool initialize(
      const nsAString& typeLibraryName,
      const nsTArray<nsString>& interfaceIdentifierNames,
      const nsTArray<nsString>& classIdentifierNames);
};

} // VideoCapture

#endif // VIDEO_CAPTURE_TYPE_LIBRARY_H
