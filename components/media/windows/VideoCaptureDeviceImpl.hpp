#ifndef VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_IMPL_H
#define VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_IMPL_H

#include <string>
#include "nsAutoPtr.h"
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
  std::string name() const;
  bool addOnNewVideoFrameCallback(ByteBufferCallback* const pCallback);
  bool removeOnNewVideoFrameCallback(ByteBufferCallback* const pCallback);
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
  static const std::wstring s_kFriendlyName;
  static const std::wstring s_kNameCaptureFilter;
  static const std::wstring s_kNameSampleGrabberFilter;
  static const std::wstring s_kNameNullRendererFilter;
  static const std::string s_kEmptyString;
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
  std::string m_name;
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
  nsTArray<ByteBufferCallback*> m_byteBufferCallbackSet;
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
