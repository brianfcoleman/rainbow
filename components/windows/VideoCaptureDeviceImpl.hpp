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
#ifndef VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_IMPL_H
#define VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_IMPL_H

#include "nsAutoPtr.h"
#include "nsStringAPI.h"
#include "nsTArray.h"
#include "COMAutoPtr.hpp"
#include "RGBVideoFormat.hpp"
#include "SampleGrabberCallback.hpp"
#include "VideoFormat.hpp"
#include "Uncopyable.hpp"

namespace VideoCapture {

class VideoCaptureDeviceImpl : private Uncopyable {
 public:
  VideoCaptureDeviceImpl(
      const QeditTypeLibrary& qeditTypeLibrary,
      COMAutoPtr<IMoniker>& pMoniker);
  ~VideoCaptureDeviceImpl();
  bool isInitialized() const;
  nsString name() const;
  bool addOnNewVideoFrameCallback(VideoFrameCallback* const pCallback);
  bool removeOnNewVideoFrameCallback(VideoFrameCallback* const pCallback);
  bool startCapturing();
  bool stopCapturing();
  double countFramesCapturedPerSecond() const;
  nsTArray<RGBVideoFormat> videoFormatList() const;
  RGBVideoFormat currentVideoFormat() const;
  bool setCurrentVideoFormat(const RGBVideoFormat& rgbVideoFormat);
  operator bool() const {
    return isInitialized();
  }
 private:
  static const PRInt32 s_kUseBufferCB = 1;
  static const nsString s_kFriendlyName;
  static const nsString s_kNameCaptureFilter;
  static const nsString s_kNameSampleGrabberFilter;
  static const nsString s_kNameNullRendererFilter;
  static const nsString s_kEmptyString;
  static const double s_kOneSecondNs;
  bool initialize(const QeditTypeLibrary& qeditTypeLibrary);
  bool initName();
  bool initCaptureGraph();
  bool buildCaptureGraph();
  bool initCaptureGraphBuilder();
  bool initSampleGrabber();
  bool initSampleGrabberCallback();
  bool initMediaControl();
  bool initVideoControl();
  bool initCapturePin();
  bool initStreamConfig();
  bool initVideoFormatList();
  bool m_isInitialized;
  nsString m_name;
  IID m_IID_ISampleGrabber;
  IID m_IID_ISampleGrabberCB;
  COMAutoPtr<IMoniker> m_pMoniker;
  COMAutoPtr<IBindCtx> m_pBindContext;
  COMAutoPtr<IFilterGraph2> m_pFilterGraph;
  COMAutoPtr<ICaptureGraphBuilder2> m_pCaptureGraphBuilder;
  COMAutoPtr<IBaseFilter> m_pCaptureFilter;
  COMAutoPtr<IBaseFilter> m_pSampleGrabberFilter;
  COMAutoPtr<IBaseFilter> m_pNullRendererFilter;
  COMAutoPtr<ISampleGrabber> m_pSampleGrabber;
  nsTArray<VideoFrameCallback*> m_videoFrameCallbackSet;
  nsAutoPtr<SampleGrabberCallback> m_pSampleGrabberCallback;
  COMAutoPtr<IMediaControl> m_pMediaControl;
  COMAutoPtr<IAMStreamConfig> m_pStreamConfig;
  COMAutoPtr<IAMVideoControl> m_pVideoControl;
  COMAutoPtr<IPin> m_pCapturePin;
  PRUint32 m_currentVideoFormatId;
  nsTArray<nsAutoPtr<VideoFormatImpl>> m_videoFormatImplList;
  nsTArray<VideoFormat> m_videoFormatList;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_IMPL_H
