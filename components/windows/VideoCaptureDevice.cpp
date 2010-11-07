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
#include "VideoCaptureDevice.hpp"
#include "VideoCaptureDeviceImpl.hpp"

namespace VideoCapture {

const nsString VideoCaptureDevice::s_kEmptyString(NS_LITERAL_STRING(""));

VideoCaptureDevice::VideoCaptureDevice()
    : m_pImpl(0) {

}

VideoCaptureDevice::VideoCaptureDevice(
    VideoCaptureDeviceImpl* const pImpl)
    : m_pImpl(pImpl) {

}

bool VideoCaptureDevice::isInitialized() const {
  if (!m_pImpl) {
    return false;
  }
  if (!m_pImpl->isInitialized()) {
    return false;
  }
  return true;
}

nsString VideoCaptureDevice::name() const {
  if (!isInitialized()) {
    return VideoCaptureDevice::s_kEmptyString;
  }
  return m_pImpl->name();
}

bool VideoCaptureDevice::addOnNewVideoFrameCallback(
    VideoFrameCallback* const pCallback) {
  if (!isInitialized()) {
    return false;
  }
  return m_pImpl->addOnNewVideoFrameCallback(pCallback);
}

bool VideoCaptureDevice::removeOnNewVideoFrameCallback(
    VideoFrameCallback* const pCallback) {
  if (!isInitialized()) {
    return false;
  }
  return m_pImpl->removeOnNewVideoFrameCallback(pCallback);
}

bool VideoCaptureDevice::startCapturing() {
  if (!isInitialized()) {
    return false;
  }
  return m_pImpl->startCapturing();
}

bool VideoCaptureDevice::stopCapturing() {
  if (!isInitialized()) {
    return false;
  }
  return m_pImpl->stopCapturing();
}

double VideoCaptureDevice::countFramesCapturedPerSecond() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_pImpl->countFramesCapturedPerSecond();
}

nsTArray<RGBVideoFormat> VideoCaptureDevice::videoFormatList() const {
  if (!isInitialized()) {
    nsTArray<RGBVideoFormat> emptyList;
    return emptyList;
  }
  return m_pImpl->videoFormatList();
}

RGBVideoFormat VideoCaptureDevice::currentVideoFormat() const {
  if (!isInitialized()) {
    RGBVideoFormat videoFormat;
    return videoFormat;
  }
  return m_pImpl->currentVideoFormat();
}

bool VideoCaptureDevice::setCurrentVideoFormat(
    const RGBVideoFormat& rgbVideoFormat) {
  if (!isInitialized()) {
    return false;
  }
  return m_pImpl->setCurrentVideoFormat(rgbVideoFormat);
}

} // VideoCapture
