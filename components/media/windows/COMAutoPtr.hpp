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
#ifndef VIDEO_CAPTURE_COM_AUTO_PTR_H
#define VIDEO_CAPTURE_COM_AUTO_PTR_H

#include <oaidl.h>

namespace VideoCapture {

template<typename COMInterface> class COMAutoPtr {
 public:
  COMAutoPtr()
      : m_pCOMInterface(0) {

  }

  explicit COMAutoPtr(COMInterface* const pCOMInterface)
      : m_pCOMInterface(pCOMInterface) {

  }

  COMAutoPtr(COMAutoPtr& comAutoPtr)
      : m_pCOMInterface(comAutoPtr.forget()) {

  }

  COMAutoPtr& operator=(COMInterface* pCOMInterface) {
    m_pCOMInterface = pCOMInterface;
    return (*this);
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
