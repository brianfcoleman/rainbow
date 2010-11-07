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
#include "SampleGrabberCallback.hpp"

namespace VideoCapture {

SampleGrabberCallback::SampleGrabberCallback(
    const IID& IID_ISampleGrabberCB,
    const RGBVideoFormat& videoFormat,
    const nsTArray<VideoFrameCallback*>& videoFrameCallbackSet)
    : m_refCount(1),
      m_IID_ISampleGrabberCB(IID_ISampleGrabberCB),
      m_videoFormat(videoFormat),
      m_videoFrameCallbackSet(videoFrameCallbackSet) {

}

STDMETHODIMP_(ULONG) SampleGrabberCallback::AddRef() {
  return ++m_refCount;
}

STDMETHODIMP_(ULONG) SampleGrabberCallback::Release() {
  if (!m_refCount) {
    return 0;
  }
  return --m_refCount;
}

STDMETHODIMP SampleGrabberCallback::QueryInterface(
    REFIID riid,
    void **ppvObject) {
  if (!ppvObject) {
    return E_POINTER;
  }

  if (riid == IID_IUnknown) {
    *ppvObject = static_cast<IUnknown*>(this);
    return S_OK;
  }

  if (riid == m_IID_ISampleGrabberCB) {
    *ppvObject = static_cast<ISampleGrabberCB*>(this);
    return S_OK;
  }

  return E_NOTIMPL;
}

STDMETHODIMP SampleGrabberCallback::SampleCB(
    double Time,
    IMediaSample *pSample) {
  return E_NOTIMPL;
}

STDMETHODIMP SampleGrabberCallback::BufferCB(
    double Time,
    BYTE *pBuffer,
    long BufferLen) {
  if (!pBuffer) {
    return S_FALSE;
  }

  if (BufferLen <= 0) {
    return S_FALSE;
  }

  if (!m_videoFormat) {
    return S_FALSE;
  }

  ByteBuffer buffer(pBuffer, BufferLen);

  if (m_videoFrameCallbackSet.IsEmpty()) {
    return S_OK;
  }

  for (PRUint32 i = 0; i < m_videoFrameCallbackSet.Length(); ++i) {
    VideoFrameCallback* pCallback(m_videoFrameCallbackSet[i]);
    if (!pCallback) {
      continue;
    }
    pCallback->onNewVideoFrame(buffer, m_videoFormat);
  }

  return S_OK;
}

} // VideoCapture

