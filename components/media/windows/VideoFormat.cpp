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
#include "VideoFormat.hpp"
#include "VideoFormatImpl.hpp"
#include "IdUtilities.hpp"

namespace VideoCapture {

VideoFormat::VideoFormat()
    : m_pImpl(0) {

}

VideoFormat::VideoFormat(VideoFormatImpl* const pImpl)
    : m_pImpl(pImpl) {

}

bool VideoFormat::isInitialized() const {
  if (!m_pImpl) {
    return false;
  }
  if (!m_pImpl->isInitialized()) {
    return false;
  }
  return true;
}

PRUint32 VideoFormat::id() const {
  if (!isInitialized()) {
    return kInvalidId;
  }
  return m_pImpl->id();
}

double VideoFormat::framesPerSecond() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_pImpl->framesPerSecond();
}

IntegerSize VideoFormat::sizePixels() const {
  if (!isInitialized()) {
    IntegerSize zeroSize;
    return zeroSize;
  }
  return m_pImpl->sizePixels();
}

PRInt32 VideoFormat::angleRotationDegrees() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_pImpl->angleRotationDegrees();
}

PRInt32 VideoFormat::bitsPerPixel() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_pImpl->bitsPerPixel();
}

PRInt32 VideoFormat::sizeBytes() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_pImpl->sizeBytes();
}

PRInt32 VideoFormat::sizeRowBytes() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_pImpl->sizeRowBytes();
}

RGBFormat VideoFormat::rgbFormat() const {
  if (!isInitialized()) {
    return kRGBNone;
  }
  return m_pImpl->rgbFormat();
}

VideoFormatImpl* VideoFormat::get() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_pImpl;
}

} // VideoCapture

