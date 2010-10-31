#ifndef VIDEO_CAPTURE_TYPE_LIBRARY_H
#define VIDEO_CAPTURE_TYPE_LIBRARY_H

#include <map>
#include <string>
#include <vector>
#include "oaidl.h"
#include "Uncopyable.hpp"

namespace VideoCapture {

class TypeLibrary :
  private Uncopyable {
public:
  TypeLibrary(
      const std::wstring& typeLibraryName,
      const std::vector<const std::wstring>& interfaceIdentifierNames,
      const std::vector<const std::wstring>& classIdentifierNames);
  virtual ~TypeLibrary();
  IID interfaceIdByName(const std::wstring& name) const;
  CLSID classIdByName(const std::wstring& name) const;
  bool isInitialized() const;
  operator bool() const {
    return isInitialized();
  }
private:
  std::map<const std::wstring, IID> m_interfaceIdentifiersByName;
  std::map<const std::wstring, CLSID> m_classIdentifiersByName;
  bool m_isInitialized;
  bool initialize(
      const std::wstring& typeLibraryName,
      const std::vector<const std::wstring>& interfaceIdentifierNames,
      const std::vector<const std::wstring>& classIdentifierNames);
};

} // VideoCapture

#endif // VIDEO_CAPTURE_TYPE_LIBRARY_H
