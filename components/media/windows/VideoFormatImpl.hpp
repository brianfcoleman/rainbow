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
#ifndef VIDEOCAPTURE_VIDEO_FORMAT_IMPL_H
#define VIDEOCAPTURE_VIDEO_FORMAT_IMPL_H

#include "DirectShow.hpp"
#include "VideoFormatTypes.hpp"
#include "Uncopyable.hpp"

namespace VideoCapture {

class VideoFormatImpl : private Uncopyable {
 public:
  VideoFormatImpl();
  VideoFormatImpl(
      IAMStreamConfig* const pStreamConfig,
      const PRUint32 index,
      const PRUint32 id);
  bool isInitialized() const;
  bool isRGBFormat() const;
  PRUint32 id() const;
  double framesPerSecond() const;
  IntegerSize sizePixels() const;
  PRInt32 angleRotationDegrees() const;
  PRInt32 bitsPerPixel() const;
  PRInt32 sizeBytes() const;
  PRInt32 sizeRowBytes() const;
  RGBFormat rgbFormat() const;
  bool VideoFormatImpl::setMediaTypeOfStream(
      IAMStreamConfig* const pStreamConfig);
  operator bool() const {
      return isInitialized();
  }
 private:
  MediaTypeAutoPtr m_pMediaType;
  VIDEO_STREAM_CONFIG_CAPS m_streamCapabilities;
  bool m_isInitialized;
  PRUint32 m_id;
  double m_framesPerSecond;
  IntegerSize m_sizePixels;
  PRInt32 m_angleRotationDegrees;
  PRInt32 m_bitsPerPixel;
  bool m_isUncompressedRGB;
  RGBFormat m_rgbFormat;
  bool initialize(
      IAMStreamConfig* const pStreamConfig,
      const PRUint32 index);
  bool extractData();
  bool isVideoFormat() const;
};

} // VideoCapture

#endif // VIDEOCAPTURE_VIDEO_FORMAT_IMPL_H
