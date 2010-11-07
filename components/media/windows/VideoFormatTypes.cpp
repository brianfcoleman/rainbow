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
#include "VideoFormatTypes.hpp"

namespace VideoCapture {

static const PRInt32 kBitsPerByte = 8;

static PRInt32 convertBitsToBytes(PRInt32 countBits);

bool isValidRGBFormat(const RGBFormat rgbFormat) {
  switch (rgbFormat) {
    case kBGR888:
      return true;
    case kBGRA8888:
      return true;
    case kRGBNone:
      return false;
    default:
      return false;
  }
}

PRInt32 bitsPerPixelForRGBFormat(const RGBFormat rgbFormat) {
  switch (rgbFormat) {
    case kBGR888:
      return 24;
    case kBGRA8888:
      return 32;
    case kRGBNone:
      return 0;
    default:
      return 0;
  }
}

PRInt32 sizeBytesForRGBFormat(
    const IntegerSize& sizePixels,
    const RGBFormat rgbFormat) {
  if (!sizePixels) {
    return 0;
  }

  PRInt32 bitsPerPixel = bitsPerPixelForRGBFormat(rgbFormat);
  if (!bitsPerPixel) {
    return 0;
  }

  PRInt32 countPixels = sizePixels.width() * sizePixels.height();
  PRInt32 sizeBits = bitsPerPixel * countPixels;

  PRInt32 sizeBytes = convertBitsToBytes(sizeBits);
  return sizeBytes;
}

PRInt32 sizeRowBytesForRGBFormat(
    const IntegerSize& sizePixels,
    const RGBFormat rgbFormat) {
  if (!sizePixels) {
    return 0;
  }

  PRInt32 bitsPerPixel = bitsPerPixelForRGBFormat(rgbFormat);
  if (!bitsPerPixel) {
    return 0;
  }

  PRInt32 sizeRowBits = bitsPerPixel * sizePixels.width();
  PRInt32 sizeRowBytes = convertBitsToBytes(sizeRowBits);
  return sizeRowBytes;
}

static PRInt32 convertBitsToBytes(PRInt32 countBits) {
  if ((countBits % kBitsPerByte) != 0) {
    return 0;
  }

  PRInt32 countBytes = countBits / kBitsPerByte;
  return countBytes;
}

} // VideoCapture
