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
#include "RGBVideoFormat.hpp"
#include "IdUtilities.hpp"

namespace VideoCapture {

RGBVideoFormat::RGBVideoFormat()
    : m_isInitialized(false),
      m_id(kInvalidId),
      m_angleRotationDegrees(0),
      m_rgbFormat(kRGBNone) {

}

RGBVideoFormat::RGBVideoFormat(const VideoFormat& videoFormat)
    : m_isInitialized(videoFormat),
      m_id(videoFormat.id()),
      m_sizePixels(videoFormat.sizePixels()),
      m_angleRotationDegrees(videoFormat.angleRotationDegrees()),
      m_rgbFormat(videoFormat.rgbFormat()) {

}

RGBVideoFormat::RGBVideoFormat(
    const PRUint32 id,
    const IntegerSize sizePixels,
    const PRInt32 angleRotationDegrees,
    const RGBFormat rgbFormat)
    : m_isInitialized(false),
      m_id(id),
      m_sizePixels(sizePixels),
      m_angleRotationDegrees(angleRotationDegrees % kAngleFullRotationDegrees),
      m_rgbFormat(rgbFormat) {
  m_isInitialized = initialize();
}

bool RGBVideoFormat::initialize() {
  if (!isValidId(m_id)) {
    return false;
  }

  if (!isValidRGBFormat(m_rgbFormat)) {
    return false;
  }

  if (!m_sizePixels) {
    return false;
  }

  return true;
}

bool RGBVideoFormat::isInitialized() const {
  return m_isInitialized;
}

PRUint32 RGBVideoFormat::id() const {
  return m_id;
}

IntegerSize RGBVideoFormat::sizePixels() const {
  return m_sizePixels;
}

PRInt32 RGBVideoFormat::angleRotationDegrees() const {
  return m_angleRotationDegrees;
}

PRInt32 RGBVideoFormat::bitsPerPixel() const {
  if (!isInitialized()) {
    return 0;
  }

  return bitsPerPixelForRGBFormat(m_rgbFormat);
}

PRInt32 RGBVideoFormat::sizeBytes() const {
  if (!isInitialized()) {
    return 0;
  }

  return sizeBytesForRGBFormat(m_sizePixels, m_rgbFormat);
}

PRInt32 RGBVideoFormat::sizeRowBytes() const {
  if (!isInitialized()) {
    return 0;
  }

  return sizeRowBytesForRGBFormat(m_sizePixels, m_rgbFormat);
}

RGBFormat RGBVideoFormat::rgbFormat() const {
  return m_rgbFormat;
}

bool operator==(const RGBVideoFormat& lhs, const RGBVideoFormat& rhs) {
  if (!lhs) {
    return false;
  }
  if (!rhs) {
    return false;
  }
  if (lhs.id() != rhs.id()) {
    return false;
  }
  if (lhs.sizePixels() != rhs.sizePixels()) {
    return false;
  }
  if (lhs.angleRotationDegrees() != rhs.angleRotationDegrees()) {
    return false;
  }
  if (lhs.bitsPerPixel() != rhs.bitsPerPixel()) {
    return false;
  }
  if (lhs.sizeBytes() != rhs.sizeBytes()) {
    return false;
  }
  if (lhs.sizeRowBytes() != rhs.sizeRowBytes()) {
    return false;
  }
  if (lhs.rgbFormat() != rhs.rgbFormat()) {
    return false;
  }
  return true;
}

bool operator==(const RGBVideoFormat& lhs, const VideoFormat& rhs) {
  if (!lhs) {
    return false;
  }
  if (!rhs) {
    return false;
  }
  if (lhs.id() != rhs.id()) {
    return false;
  }
  if (lhs.sizePixels() != rhs.sizePixels()) {
    return false;
  }
  if (lhs.angleRotationDegrees() != rhs.angleRotationDegrees()) {
    return false;
  }
  if (lhs.bitsPerPixel() != rhs.bitsPerPixel()) {
    return false;
  }
  if (lhs.sizeBytes() != rhs.sizeBytes()) {
    return false;
  }
  if (lhs.sizeRowBytes() != rhs.sizeRowBytes()) {
    return false;
  }
  if (lhs.rgbFormat() != rhs.rgbFormat()) {
    return false;
  }
  return true;
}

bool operator==(const VideoFormat& lhs, const RGBVideoFormat& rhs) {
  return operator==(rhs, lhs);
}

bool operator!=(const RGBVideoFormat& lhs, const RGBVideoFormat& rhs) {
  return !operator==(lhs, rhs);
}

bool operator!=(const RGBVideoFormat& lhs, const VideoFormat& rhs) {
  return !operator==(lhs, rhs);
}

bool operator!=(const VideoFormat& lhs, const RGBVideoFormat& rhs) {
  return !operator==(lhs, rhs);
}

} // VideoCapture