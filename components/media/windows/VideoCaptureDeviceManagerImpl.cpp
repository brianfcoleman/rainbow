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
