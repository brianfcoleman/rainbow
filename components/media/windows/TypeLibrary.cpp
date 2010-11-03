/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Mozilla Labs Rainbow.
 *
 * The Initial Developer of the Original Code is
 * Brian Coleman.
 * Portions created by the Initial Developer are Copyright (C) 2010
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Brian Coleman <brianfcoleman@gmail.com>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */
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
