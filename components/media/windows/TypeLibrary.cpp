#include "TypeLibrary.hpp"
#include "comdef.h"
#include "comdefsp.h"
#include "nspr.h"
#include "nsAutoPtr.h"
#include "GUIDUtilities.hpp"
#include "TypeAttributeAutoPtr.hpp"

namespace VideoCapture {

static ITypeLibPtr typeLibSharedPtr(const std::wstring& fileNameTypeLib);

static GUID guidOfTypeKind(
    const ITypeLibPtr& typeLib,
    const std::wstring& nameTypeInfo,
    const TYPEKIND typeKind);

static GUID uniqueGUIDOfTypeKind(
    const std::vector<ITypeInfoPtr>& typeInfoSharedPtrVector,
    const TYPEKIND typeKind);

static nsAutoArrayPtr<wchar_t> copyWideCharArrayFromString(
    const std::wstring& wideCharString);

template<typename TypeKindGUID> bool mapNamesToGUIDs(
    const ITypeLibPtr& typeLib,
    const TYPEKIND typeKind,
    const std::vector<const std::wstring>& names,
    std::map<const std::wstring, TypeKindGUID>& guidsByName);

template<typename TypeKindGUID> TypeKindGUID findGUIDByName(
    const std::wstring& name,
    const std::map<const std::wstring, TypeKindGUID>& guidsByName);

static ITypeLibPtr typeLibSharedPtr(const std::wstring& fileNameTypeLib) {
  ITypeLib* pTypeLib = 0;
  HRESULT result = LoadTypeLib(
      static_cast<const OLECHAR FAR*>(fileNameTypeLib.c_str()),
      static_cast<ITypeLib FAR* FAR*>(&pTypeLib));
  if (result != S_OK) {
    ITypeLibPtr typeLibSharedPtr;
    return typeLibSharedPtr;
  }
  ITypeLibPtr typeLibSharedPtr(pTypeLib);
  return typeLibSharedPtr;
}

static GUID guidOfTypeKind(
    const ITypeLibPtr& typeLib,
    const std::wstring& nameTypeInfo,
    const TYPEKIND typeKind) {
  if (!typeLib) {
    return emptyGUID();
  }
  nsAutoArrayPtr<wchar_t> wideCharArrayNameTypeInfo(
      copyWideCharArrayFromString(nameTypeInfo));
  if (!wideCharArrayNameTypeInfo) {
    return emptyGUID();
  }
  UINT countTypeInfo = typeLib->GetTypeInfoCount();
  if (!countTypeInfo) {
    return emptyGUID();
  }
  nsAutoArrayPtr<ITypeInfo*> typeInfoPtrArray(
      new ITypeInfo*[countTypeInfo]);
  nsAutoArrayPtr<MEMBERID> memberIdArray(new MEMBERID[countTypeInfo]);
  long hashValue = 0;
  USHORT countFound = countTypeInfo;
  HRESULT result =
  typeLib->FindName(
      static_cast<OLECHAR FAR*>(wideCharArrayNameTypeInfo.get()),
      hashValue,
      static_cast<ITypeInfo FAR* FAR*>(typeInfoPtrArray.get()),
      static_cast<MEMBERID FAR*>(memberIdArray.get()),
      &countFound);
  if (result != S_OK) {
    return emptyGUID();
  }
  if (!countFound) {
    return emptyGUID();
  }
  std::vector<ITypeInfoPtr> typeInfoSharedPtrVector;
  for (PRInt32 i = 0; i < countFound; ++i) {
    ITypeInfo** pTypeInfoArray = typeInfoPtrArray.get();
    ITypeInfoPtr typeInfoSharedPtr(pTypeInfoArray[i]);
    typeInfoSharedPtrVector.push_back(typeInfoSharedPtr);
  }
  return uniqueGUIDOfTypeKind(typeInfoSharedPtrVector, typeKind);
}

static GUID uniqueGUIDOfTypeKind(
    const std::vector<ITypeInfoPtr>& typeInfoSharedPtrVector,
    const TYPEKIND typeKind) {
  if (!typeInfoSharedPtrVector.size()) {
    return emptyGUID();
  }
  PRInt32 countGUIDsOfTypeKind = 0;
  TypeAttributeAutoPtr typeAttributeOfTypeKind;
  for (std::vector<ITypeInfoPtr>::const_iterator
           iterator(typeInfoSharedPtrVector.begin());
       iterator != typeInfoSharedPtrVector.end();
       ++iterator) {
    ITypeInfoPtr typeInfo(*iterator);
    TypeAttributeAutoPtr typeAttribute(typeInfo);
    if (!typeAttribute) {
      continue;
    }
    TYPEATTR* pTypeAttribute = typeAttribute.get();
    if (pTypeAttribute->typekind != typeKind) {
      continue;
    }
    ++countGUIDsOfTypeKind;
    if (countGUIDsOfTypeKind > 1) {
      return emptyGUID();
    }
    typeAttributeOfTypeKind = typeAttribute;
  }
  if (countGUIDsOfTypeKind != 1) {
    return emptyGUID();
  }
  if (!typeAttributeOfTypeKind) {
    return emptyGUID();
  }
  TYPEATTR* pTypeAttribute = typeAttributeOfTypeKind.get();
  GUID guid = pTypeAttribute->guid;
  return guid;
}

static nsAutoArrayPtr<wchar_t> copyWideCharArrayFromString(
    const std::wstring& wideCharString) {
  if (!wideCharString.size()) {
    nsAutoArrayPtr<wchar_t> wideCharSharedArray;
    return wideCharSharedArray;
  }
  PRInt32 sizeWideCharStringWithNulTerminator = wideCharString.size() + 1;
  nsAutoArrayPtr<wchar_t> wideCharSharedArray(
      new wchar_t[sizeWideCharStringWithNulTerminator]);
  if (!wideCharSharedArray) {
    return wideCharSharedArray;
  }
  wcsncpy_s(
      wideCharSharedArray.get(),
      sizeWideCharStringWithNulTerminator,
      wideCharString.c_str(),
      sizeWideCharStringWithNulTerminator);
  return wideCharSharedArray;
}

template<typename TypeKindGUID> bool mapNamesToGUIDs(
    const ITypeLibPtr& typeLib,
    const TYPEKIND typeKind,
    const std::vector<const std::wstring>& names,
    std::map<const std::wstring, TypeKindGUID>& guidsByName) {
  if (!typeLib) {
    return false;
  }
  if (!names.size()) {
    return false;
  }
  for(std::vector<const std::wstring>::const_iterator iterator(names.begin());
      iterator != names.end();
      ++iterator) {
    const std::wstring& nameTypeInfo(*iterator);
    const GUID guid(guidOfTypeKind(typeLib, nameTypeInfo, typeKind));
    if (isEmptyGUID(guid)) {
      guidsByName.clear();
      return false;
    }
    guidsByName[nameTypeInfo] = static_cast<TypeKindGUID>(guid);
  }
  return true;
}

template<typename TypeKindGUID> TypeKindGUID findGUIDByName(
    const std::wstring& name,
    const std::map<const std::wstring, TypeKindGUID>& guidsByName) {
  std::map<const std::wstring, TypeKindGUID>::const_iterator iterator(
      guidsByName.find(name));
  if (iterator == guidsByName.end()) {
    return emptyGUID();
  }
  TypeKindGUID guid(iterator->second);
  return guid;
}

TypeLibrary::TypeLibrary(
    const std::wstring& typeLibraryName,
    const std::vector<const std::wstring>& interfaceIdentifierNames,
    const std::vector<const std::wstring>& classIdentifierNames)
    : Uncopyable(),
      m_isInitialized(false) {
  m_isInitialized = initialize(
      typeLibraryName,
      interfaceIdentifierNames,
      classIdentifierNames);
}

TypeLibrary::~TypeLibrary() {

}

bool TypeLibrary::initialize(
    const std::wstring& typeLibraryName,
    const std::vector<const std::wstring>& interfaceIdentifierNames,
    const std::vector<const std::wstring>& classIdentifierNames) {
  ITypeLibPtr typeLib = typeLibSharedPtr(typeLibraryName);
  if (!typeLib) {
    return false;
  }
  if (!mapNamesToGUIDs(
          typeLib,
          TKIND_INTERFACE,
          interfaceIdentifierNames,
          m_interfaceIdentifiersByName)) {
    return false;
  }
  if (!mapNamesToGUIDs(
          typeLib,
          TKIND_COCLASS,
          classIdentifierNames,
          m_classIdentifiersByName)) {
    return false;
  }
  return true;
}

bool TypeLibrary::isInitialized() const {
  return m_isInitialized;
}

IID TypeLibrary::interfaceIdByName(const std::wstring& name) const {
  if (!isInitialized()) {
    return emptyGUID();
  }
  return findGUIDByName(name, m_interfaceIdentifiersByName);
}

CLSID TypeLibrary::classIdByName(const std::wstring& name) const {
  if (!isInitialized()) {
    return emptyGUID();
  }
  return findGUIDByName(name, m_classIdentifiersByName);
}

} // VideoCapture
