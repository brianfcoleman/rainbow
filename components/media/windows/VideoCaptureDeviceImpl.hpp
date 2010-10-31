#ifndef VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_IMPL_H
#define VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_IMPL_H

#include <list>
#include <map>
#include <set>
#include <string>
#include "nsAutoPtr.h"
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
      const IMonikerPtr& pMoniker);
  ~VideoCaptureDeviceImpl();
  bool isInitialized() const;
  std::string name() const;
  bool addOnNewVideoFrameCallback(ByteBufferCallback* const pCallback);
  bool removeOnNewVideoFrameCallback(ByteBufferCallback* const pCallback);
  bool startCapturing();
  bool stopCapturing();
  double countFramesCapturedPerSecond() const;
  std::list<RGBVideoFormat> videoFormatList() const;
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
  bool initVideoFormatMap();
  RGBVideoFormat rgbVideoFormatFromPair(
      const std::pair<PRInt32, VideoFormat>& idVideoFormatPair) const;
  bool m_isInitialized;
  std::string m_name;
  IID m_IID_ISampleGrabber;
  IID m_IID_ISampleGrabberCB;
  IMonikerPtr m_pMoniker;
  IBindCtxPtr m_pBindContext;
  IFilterGraph2Ptr m_pFilterGraph;
  ICaptureGraphBuilder2Ptr m_pCaptureGraphBuilder;
  IBaseFilterPtr m_pCaptureFilter;
  IBaseFilterPtr m_pSampleGrabberFilter;
  IBaseFilterPtr m_pNullRendererFilter;
  COMAutoPtr<ISampleGrabber> m_pSampleGrabber;
  std::set<ByteBufferCallback*> m_byteBufferCallbackSet;
  nsAutoPtr<SampleGrabberCallback> m_pSampleGrabberCallback;
  IMediaControlPtr m_pMediaControl;
  IAMStreamConfigPtr m_pStreamConfig;
  IAMVideoControlPtr m_pVideoControl;
  IPinPtr m_pCapturePin;
  PRInt32 m_currentVideoFormatId;
  std::map<PRInt32, VideoFormat> m_videoFormatsById;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_IMPL_H
