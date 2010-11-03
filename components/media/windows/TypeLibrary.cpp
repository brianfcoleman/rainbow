#include "TypeLibrary.hpp"
#include "nspr.h"
#include "nsAutoPtr.h"
#include "COMAutoPtr.hpp"
#include "GUIDUtilities.hpp"
#include "TypeAttributeAutoPtr.hpp"
#ifdef VIDEO_CAPTURE_DEBUG
#include <cstdio>
#endif

namespace VideoCapture {

static COMAutoPtr<ITypeLib> typeLibAutoPtr(const nsAString& fileNameTypeLib);

static GUID guidOfTypeKind(
    ITypeLib* const pTypeLib,
    const nsAString& nameTypeInfo,
    const TYPEKIND typeKind);

static GUID uniqueGUIDOfTypeKind(
    const nsTArray<ITypeInfo*>& typeInfoPtrArray,
    const TYPEKIND typeKind);

bool mapNamesToGUIDs(
    ITypeLib* const pTypeLib,
    const TYPEKIND typeKind,
    const nsTArray<nsString>& names,
    nsTArray<StringGUIDPair>& guidsByName);

GUID findGUIDByName(
    const nsAString& name,
    const nsTArray<StringGUIDPair>& guidsByName);

static COMAutoPtr<ITypeLib> typeLibAutoPtr(const nsAString& fileNameTypeLib) {
  ITypeLib* pTypeLib = 0;
  HRESULT result = LoadTypeLib(
      static_cast<const OLECHAR FAR*>(fileNameTypeLib.BeginReading()),
      static_cast<ITypeLib FAR* FAR*>(&pTypeLib));
  if (result != S_OK) {
    COMAutoPtr<ITypeLib> typeLibAutoPtr;
    return typeLibAutoPtr;
  }
  COMAutoPtr<ITypeLib> typeLibAutoPtr(pTypeLib);
  return typeLibAutoPtr;
}

static GUID guidOfTypeKind(
    ITypeLib* const pTypeLib,
    const nsAString& nameTypeInfo,
    const TYPEKIND typeKind) {
  if (!pTypeLib) {
    return emptyGUID();
  }
  UINT countTypeInfo = pTypeLib->GetTypeInfoCount();
  if (!countTypeInfo) {
    return emptyGUID();
  }
  nsString copyNameTypeInfo(nameTypeInfo);
  nsAutoArrayPtr<ITypeInfo*> typeInfoPtrArray(
      new ITypeInfo*[countTypeInfo]);
  nsAutoArrayPtr<MEMBERID> memberIdArray(new MEMBERID[countTypeInfo]);
  long hashValue = 0;
  USHORT countFound = countTypeInfo;
  HRESULT result =
  pTypeLib->FindName(
      static_cast<OLECHAR FAR*>(copyNameTypeInfo.BeginWriting()),
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
  nsTArray<COMAutoPtr<ITypeInfo>> typeInfoAutoPtrArray;
  nsTArray<ITypeInfo*> typeInfoRawPtrArray;
  for (PRUint32 i = 0; i < countFound; ++i) {
    ITypeInfo** pTypeInfoArray = typeInfoPtrArray.get();
    ITypeInfo* pTypeInfo = pTypeInfoArray[i];
    typeInfoAutoPtrArray.AppendElement(pTypeInfo);
    typeInfoRawPtrArray.AppendElement(pTypeInfo);
  }
  return uniqueGUIDOfTypeKind(typeInfoRawPtrArray, typeKind);
}

static GUID uniqueGUIDOfTypeKind(
    const nsTArray<ITypeInfo*>& typeInfoPtrArray,
    const TYPEKIND typeKind) {
  if (typeInfoPtrArray.IsEmpty()) {
    return emptyGUID();
  }
  PRInt32 countGUIDsOfTypeKind = 0;
  GUID uniqueGUID = emptyGUID();
  for (PRUint32 i = 0; i < typeInfoPtrArray.Length(); ++i) {
    ITypeInfo* pTypeInfo = typeInfoPtrArray[i];
    TypeAttributeAutoPtr typeAttribute(pTypeInfo);
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
    uniqueGUID = pTypeAttribute->guid;
  }
  if (countGUIDsOfTypeKind != 1) {
    return emptyGUID();
  }
  return uniqueGUID;
}

bool mapNamesToGUIDs(
    ITypeLib* const pTypeLib,
    const TYPEKIND typeKind,
    const nsTArray<nsString>& names,
    nsTArray<StringGUIDPair>& guidsByName) {
  if (!pTypeLib) {
    return false;
  }
  if (names.IsEmpty()) {
    return false;
  }
  for (PRUint32 i = 0; i < names.Length(); ++i) {
    const nsString nameTypeInfo(names[i]);
    if (nameTypeInfo.IsEmpty()) {
      guidsByName.Clear();
      return false;
    }
    if (nameTypeInfo.IsVoid()) {
      guidsByName.Clear();
      return false;
    }
#ifdef VIDEO_CAPTURE_DEBUG
    NS_LossyConvertUTF16toASCII_external convertedString(nameTypeInfo);
    printf("nameTypeInfo: %s\n", convertedString);
#endif
    const GUID guid(guidOfTypeKind(pTypeLib, nameTypeInfo, typeKind));
    if (isEmptyGUID(guid)) {
      guidsByName.Clear();
      return false;
    }
    StringGUIDPair pair(nameTypeInfo, guid);
    guidsByName.AppendElement(pair);
  }
  return true;
}

GUID findGUIDByName(
    const nsAString& name,
    const nsTArray<StringGUIDPair>& guidsByName) {
  if (name.IsEmpty()) {
    return emptyGUID();
  }
  if (name.IsVoid()) {
    return emptyGUID();
  }
  if (guidsByName.IsEmpty()) {
    return emptyGUID();
  }
  for (PRUint32 i = 0; i < guidsByName.Length(); ++i) {
    StringGUIDPair pair(guidsByName[i]);
    if (pair != name) {
      continue;
    }
    return pair.guid();
  }
  return emptyGUID();
}

TypeLibrary::TypeLibrary(
    const nsAString& typeLibraryName,
    const nsTArray<nsString>& interfaceIdentifierNames,
    const nsTArray<nsString>& classIdentifierNames)
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
    const nsAString& typeLibraryName,
    const nsTArray<nsString>& interfaceIdentifierNames,
    const nsTArray<nsString>& classIdentifierNames) {
  COMAutoPtr<ITypeLib> typeLib = typeLibAutoPtr(typeLibraryName);
  if (!typeLib) {
    return false;
  }
#ifdef VIDEO_CAPTURE_DEBUG
  printf("TypeLibrary typeLib found\n");
#endif
  if (!mapNamesToGUIDs(
          typeLib.get(),
          TKIND_INTERFACE,
          interfaceIdentifierNames,
          m_interfaceIdentifiersByName)) {
    return false;
  }
#ifdef VIDEO_CAPTURE_DEBUG
  printf("TypeLibrary mapped names to IIDs\n");
#endif
  if (!mapNamesToGUIDs(
          typeLib.get(),
          TKIND_COCLASS,
          classIdentifierNames,
          m_classIdentifiersByName)) {
    return false;
  }
#ifdef VIDEO_CAPTURE_DEBUG
  printf("TypeLibrary mapped names to CLSIDs\n");
#endif
  return true;
}

bool TypeLibrary::isInitialized() const {
  return m_isInitialized;
}

IID TypeLibrary::interfaceIdByName(const nsAString& name) const {
  if (!isInitialized()) {
    return emptyGUID();
  }
  return findGUIDByName(name, m_interfaceIdentifiersByName);
}

CLSID TypeLibrary::classIdByName(const nsAString& name) const {
  if (!isInitialized()) {
    return emptyGUID();
  }
  return findGUIDByName(name, m_classIdentifiersByName);
}

} // VideoCapture
