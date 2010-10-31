#include "VideoCaptureDeviceImpl.hpp"
#include "VideoFormatImpl.hpp"
#include "comutil.h"
#include "IdUtilities.hpp"

namespace VideoCapture {

static RGBVideoFormat firstVideoFormatFromList(
    std::list<RGBVideoFormat> videoFormatList);

const std::wstring VideoCaptureDeviceImpl::s_kFriendlyName(L"FriendlyName");
const std::wstring VideoCaptureDeviceImpl::s_kNameCaptureFilter(
    L"Capture Filter");
const std::wstring VideoCaptureDeviceImpl::s_kNameSampleGrabberFilter(
    L"Sample Grabber Filter");
const std::wstring VideoCaptureDeviceImpl::s_kNameNullRendererFilter(
    L"Null Renderer Filter");
const std::string VideoCaptureDeviceImpl::s_kEmptyString("");
const double VideoCaptureDeviceImpl::s_kOneSecondNs = 1.0e9;

VideoCaptureDeviceImpl::VideoCaptureDeviceImpl(
    const QeditTypeLibrary& qeditTypeLibrary,
    const IMonikerPtr& pMoniker)
    : Uncopyable(),
      m_isInitialized(false),
      m_name(s_kEmptyString),
      m_IID_ISampleGrabber(qeditTypeLibrary.IID_ISampleGrabber()),
      m_IID_ISampleGrabberCB(qeditTypeLibrary.IID_ISampleGrabberCB()),
      m_pMoniker(pMoniker),
      m_pSampleGrabberFilter(createInstanceCOMInterface<IBaseFilter>(
          qeditTypeLibrary.CLSID_SampleGrabber(),
          IID_IBaseFilter)),
      m_currentVideoFormatId(kInvalidId) {
  m_isInitialized = initialize(qeditTypeLibrary);
  RGBVideoFormat videoFormat(firstVideoFormatFromList(videoFormatList()));
  if (!videoFormat) {
    return;
  }
  m_isInitialized = setCurrentVideoFormat(videoFormat);
}

VideoCaptureDeviceImpl::~VideoCaptureDeviceImpl() {
  stopCapturing();
}

bool VideoCaptureDeviceImpl::initialize(
    const QeditTypeLibrary& qeditTypeLibrary) {
  m_pFilterGraph.CreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER);
  m_pCaptureGraphBuilder.CreateInstance(
      CLSID_CaptureGraphBuilder2,
      0,
      CLSCTX_INPROC_SERVER);
  m_pNullRendererFilter.CreateInstance(
      qeditTypeLibrary.CLSID_NullRenderer(),
      0,
      CLSCTX_INPROC_SERVER);

  if (!initName()) {
    return false;
  }

  if (!initCaptureGraph()) {
    return false;
  }

  if (!initVideoFormatMap()) {
    return false;
  }

  return true;
}

bool VideoCaptureDeviceImpl::initName() {
  if (!m_pMoniker) {
    return false;
  }
  IBindCtx* pBindContext = 0;
  HRESULT result = CreateBindCtx(0, &pBindContext);
  if (FAILED(result)) {
    return false;
  }
  IBindCtxPtr bindContextPtr(pBindContext);
  if (!bindContextPtr) {
    return false;
  }

  IPropertyBag* pPropertyBag = 0;
  result = m_pMoniker->BindToStorage(
      bindContextPtr,
      0,
      IID_IPropertyBag,
      reinterpret_cast<void**>(&pPropertyBag));
  if (FAILED(result)) {
    return false;
  }
  IPropertyBagPtr propertyBagPtr(pPropertyBag);
  if (!propertyBagPtr) {
    return false;
  }

  _variant_t variant;
  result = propertyBagPtr->Read(
      s_kFriendlyName.c_str(),
      variant.GetAddress(),
      0);
  if (FAILED(result)) {
    return false;
  }

  _bstr_t basicString(variant);
  std::string friendlyName(basicString);
  m_name = friendlyName;
  return true;
}

bool VideoCaptureDeviceImpl::initCaptureGraph() {
  if(!initCaptureGraphBuilder()) {
    return false;
  }

  if (!buildCaptureGraph()) {
    return false;
  }

  if (!initSampleGrabber()) {
    return false;
  }

  if (!initSampleGrabberCallback()) {
    return false;
  }

  if (!initMediaControl()) {
    return false;
  }

  if (!initStreamConfig()) {
    return false;
  }

  initVideoControl();

  return true;
}

bool VideoCaptureDeviceImpl::initCaptureGraphBuilder() {
  if (!m_pFilterGraph) {
    return false;
  }

  if (!m_pCaptureGraphBuilder) {
    return false;
  }

  HRESULT result = m_pCaptureGraphBuilder->SetFiltergraph(m_pFilterGraph);
  return SUCCEEDED(result);
}

bool VideoCaptureDeviceImpl::buildCaptureGraph() {
  if (!m_pFilterGraph) {
    return false;
  }

  IBindCtx* pBindContext = 0;
  HRESULT result = CreateBindCtx(0, &pBindContext);
  if (FAILED(result)) {
    return false;
  }
  IBindCtxPtr bindContextPtr(pBindContext);
  m_pBindContext = bindContextPtr;
  if (!m_pBindContext) {
    return false;
  }

  IBaseFilter* pCaptureFilter = 0;
  result = m_pFilterGraph->AddSourceFilterForMoniker(
      m_pMoniker,
      m_pBindContext,
      s_kNameCaptureFilter.c_str(),
      &pCaptureFilter);
  if (FAILED(result)) {
    return false;
  }
  IBaseFilterPtr captureFilterPtr(pCaptureFilter);
  m_pCaptureFilter = captureFilterPtr;
  if (!m_pCaptureFilter) {
    return false;
  }

  if (!m_pSampleGrabberFilter) {
    return false;
  }

  result = m_pFilterGraph->AddFilter(
      m_pSampleGrabberFilter,
      s_kNameSampleGrabberFilter.c_str());
  if (FAILED(result)) {
    return false;
  }

  if (!m_pNullRendererFilter) {
    return false;
  }

  result = m_pFilterGraph->AddFilter(
      m_pNullRendererFilter,
      s_kNameNullRendererFilter.c_str());
  if (FAILED(result)) {
    return false;
  }

  if (!m_pCaptureGraphBuilder) {
    return false;
  }

  result = m_pCaptureGraphBuilder->RenderStream(
      &PIN_CATEGORY_CAPTURE,
      &MEDIATYPE_Video,
      m_pCaptureFilter,
      m_pSampleGrabberFilter,
      m_pNullRendererFilter);
  return SUCCEEDED(result);
}

bool VideoCaptureDeviceImpl::initSampleGrabber() {
  if (!m_pSampleGrabberFilter) {
    return false;
  }

  COMAutoPtr<ISampleGrabber> sampleGrabberPtr(
      queryCOMInterface<IBaseFilter, ISampleGrabber>(
      m_pSampleGrabberFilter,
      m_IID_ISampleGrabber));
  m_pSampleGrabber = sampleGrabberPtr;
  if (!m_pSampleGrabber) {
    return false;
  }
  ISampleGrabber* pSampleGrabber = m_pSampleGrabber.get();

  HRESULT result = pSampleGrabber->SetOneShot(FALSE);
  if (FAILED(result)) {
    return false;
  }

  result = pSampleGrabber->SetBufferSamples(FALSE);
  if (FAILED(result)) {
    return false;
  }

  return true;
}

bool VideoCaptureDeviceImpl::initSampleGrabberCallback() {
  if (!m_pSampleGrabber) {
    return false;
  }
  ISampleGrabber* pSampleGrabber = m_pSampleGrabber.get();

  HRESULT result = pSampleGrabber->SetCallback(
      0,
      VideoCaptureDeviceImpl::s_kUseBufferCB);
  if (FAILED(result)) {
    return false;
  }

  nsAutoPtr<SampleGrabberCallback> sampleGrabberCallbackPtr(
      new SampleGrabberCallback(
          m_IID_ISampleGrabberCB,
          m_byteBufferCallbackSet));
  m_pSampleGrabberCallback = sampleGrabberCallbackPtr;
  SampleGrabberCallback* pSampleGrabberCallback = m_pSampleGrabberCallback.get();
  if (!pSampleGrabberCallback) {
    return false;
  }

  result = pSampleGrabber->SetCallback(
      pSampleGrabberCallback,
      VideoCaptureDeviceImpl::s_kUseBufferCB);

  return SUCCEEDED(result);
}

bool VideoCaptureDeviceImpl::initMediaControl() {
  if (!m_pFilterGraph) {
    return false;
  }

  IMediaControlPtr mediaControlPtr;
  m_pFilterGraph.QueryInterface<IMediaControl>(
      IID_IMediaControl,
      &mediaControlPtr);
  m_pMediaControl = mediaControlPtr;
  if (!m_pMediaControl) {
    return false;
  }

  return true;
}

bool VideoCaptureDeviceImpl::initVideoControl()
{
  if (!m_pCaptureGraphBuilder) {
    return false;
  }

  if (!m_pCaptureFilter) {
    return false;
  }

  IAMVideoControl* pVideoControl = 0;
  HRESULT result = m_pCaptureGraphBuilder->FindInterface(
      &PIN_CATEGORY_CAPTURE,
      0,
      m_pCaptureFilter,
      IID_IAMVideoControl,
      reinterpret_cast<void**>(&pVideoControl));
  if (FAILED(result)) {
    return false;
  }
  IAMVideoControlPtr videoControlPtr(pVideoControl);
  m_pVideoControl = videoControlPtr;
  if (!m_pVideoControl) {
    return false;
  }

  return true;
}

bool VideoCaptureDeviceImpl::initCapturePin()
{
  if (!m_pCaptureGraphBuilder) {
    return false;
  }

  IPin* pCapturePin = 0;
  HRESULT result = m_pCaptureGraphBuilder->FindPin(
      m_pCaptureFilter,
      PINDIR_OUTPUT,
      &PIN_CATEGORY_CAPTURE,
      &MEDIATYPE_Video,
      FALSE,
      0,
      &pCapturePin);
  if (FAILED(result)) {
    return false;
  }
  IPinPtr capturePinPtr(pCapturePin);
  m_pCapturePin = capturePinPtr;
  if (!m_pCapturePin) {
    return false;
  }

  return true;
}

bool VideoCaptureDeviceImpl::initStreamConfig() {
  if (!m_pCaptureGraphBuilder) {
    return false;
  }

  if (!m_pCaptureFilter) {
    return false;
  }

  IAMStreamConfig* pStreamConfig = 0;
  HRESULT result = m_pCaptureGraphBuilder->FindInterface(
      &PIN_CATEGORY_CAPTURE,
      0,
      m_pCaptureFilter,
      IID_IAMStreamConfig,
      reinterpret_cast<void**>(&pStreamConfig));
  if (FAILED(result)) {
    return false;
  }
  IAMStreamConfigPtr streamConfigPtr(pStreamConfig);
  m_pStreamConfig = streamConfigPtr;
  if (!m_pStreamConfig) {
    return false;
  }

  return true;
}

bool VideoCaptureDeviceImpl::initVideoFormatMap() {
  if (!m_pStreamConfig) {
    return false;
  }

  int countCapabilities = 0;
  int sizeBytes = 0;
  HRESULT result = m_pStreamConfig->GetNumberOfCapabilities(
      &countCapabilities,
      &sizeBytes);
  if (FAILED(result)) {
    return false;
  }

  if (sizeBytes != sizeof(VIDEO_STREAM_CONFIG_CAPS)) {
    return false;
  }

  for (PRInt32 index = 0, id = 0; index < countCapabilities; ++index) {
    VideoFormat videoFormat(
        new VideoFormatImpl(m_pStreamConfig, index, id));
    if (!videoFormat) {
      continue;
    }
    VideoFormatImpl* pVideoFormat = videoFormat.get();
    if (!pVideoFormat->isRGBFormat()) {
      continue;
    }

    m_videoFormatsById[videoFormat.id()] = videoFormat;
    ++id;
  }

  return (!m_videoFormatsById.empty());
}

bool VideoCaptureDeviceImpl::isInitialized() const {
  return m_isInitialized;
}

std::string VideoCaptureDeviceImpl::name() const {
  return m_name;
}

bool VideoCaptureDeviceImpl::addOnNewVideoFrameCallback(
    ByteBufferCallback* const pCallback) {
  if (!pCallback) {
    return false;
  }
  std::pair<std::set<ByteBufferCallback*>::iterator, bool> resultPair(
      m_byteBufferCallbackSet.insert(pCallback));
  bool isInserted = resultPair.second;
  return isInserted;
}

bool VideoCaptureDeviceImpl::removeOnNewVideoFrameCallback(
    ByteBufferCallback* const pCallback) {
  if (!pCallback) {
    return false;
  }
  if (m_byteBufferCallbackSet.empty()) {
    return false;
  }
  PRInt32 countRemoved = m_byteBufferCallbackSet.erase(pCallback);
  return (countRemoved > 0);
}

bool VideoCaptureDeviceImpl::startCapturing() {
  if (!isInitialized()) {
    return false;
  }

  if (!stopCapturing()) {
    return false;
  }

  if (!initSampleGrabberCallback()) {
    return false;
  }

  HRESULT result = m_pMediaControl->Run();

  return SUCCEEDED(result);
}

bool VideoCaptureDeviceImpl::stopCapturing() {
  if (!isInitialized()) {
    return true;
  }

  HRESULT result;
  result = m_pMediaControl->Stop();
  return SUCCEEDED(result);
}

double VideoCaptureDeviceImpl::countFramesCapturedPerSecond() const {
  if (!isInitialized()) {
    return 0;
  }

  if (!m_pVideoControl) {
    return 0;
  }

  if (!m_pCapturePin) {
    return 0;
  }

  PRInt64 framePeriod = 0;
  HRESULT result = m_pVideoControl->GetCurrentActualFrameRate(
      m_pCapturePin,
      &framePeriod);
  if (FAILED(result)) {
    return 0;
  }
  if (framePeriod <= 0) {
    return 0;
  }
  double framePeriodNs = framePeriod * 100.0;
  double frameRate = s_kOneSecondNs / framePeriodNs;
  return frameRate;
}

std::list<RGBVideoFormat> VideoCaptureDeviceImpl::videoFormatList() const {
  if (!isInitialized()) {
    std::list<RGBVideoFormat> emptyList;
    return emptyList;
  }

  std::list<RGBVideoFormat> rgbVideoFormatList;
  for (std::map<PRInt32, VideoFormat>::const_iterator iterator(
           m_videoFormatsById.begin());
       iterator != m_videoFormatsById.end();
       ++iterator) {
    const std::pair<PRInt32, VideoFormat> idVideoFormatPair(*iterator);
    RGBVideoFormat rgbVideoFormat(rgbVideoFormatFromPair(idVideoFormatPair));
    if (!rgbVideoFormat) {
      continue;
    }
    rgbVideoFormatList.push_back(rgbVideoFormat);
  }
  return rgbVideoFormatList;
}

RGBVideoFormat VideoCaptureDeviceImpl::rgbVideoFormatFromPair(
    const std::pair<PRInt32, VideoFormat>& idVideoFormatPair) const {
  const VideoFormat videoFormat(idVideoFormatPair.second);
  if (!videoFormat) {
    RGBVideoFormat rgbVideoFormat;
    return rgbVideoFormat;
  }
  RGBVideoFormat rgbVideoFormat(videoFormat);
  return rgbVideoFormat;
}

RGBVideoFormat VideoCaptureDeviceImpl::currentVideoFormat() const {
  if (!isInitialized()) {
    RGBVideoFormat rgbVideoFormat;
    return rgbVideoFormat;
  }

  if (!isValidId(m_currentVideoFormatId)) {
    RGBVideoFormat rgbVideoFormat;
    return rgbVideoFormat;
  }

  std::map<PRInt32, VideoFormat>::const_iterator iterator(
      m_videoFormatsById.find(m_currentVideoFormatId));
  if (iterator == m_videoFormatsById.end()) {
    RGBVideoFormat rgbVideoFormat;
    return rgbVideoFormat;
  }

  const std::pair<PRInt32, VideoFormat> idVideoFormatPair(*iterator);
  const RGBVideoFormat rgbVideoFormat(
      rgbVideoFormatFromPair(idVideoFormatPair));
  return rgbVideoFormat;
}

bool VideoCaptureDeviceImpl::setCurrentVideoFormat(
    const RGBVideoFormat& rgbVideoFormat) {
  if (!isInitialized()) {
    return false;
  }

  if (!rgbVideoFormat) {
    return false;
  }

  if (!stopCapturing()) {
    return false;
  }

  const PRInt32 id = rgbVideoFormat.id();
  if (!isValidId(id)) {
    return false;
  }

  std::map<PRInt32, VideoFormat>::const_iterator iterator(
      m_videoFormatsById.find(id));
  if (iterator == m_videoFormatsById.end()) {
    return false;
  }

  const std::pair<PRInt32, VideoFormat> idVideoFormatPair(*iterator);
  const VideoFormat videoFormat(idVideoFormatPair.second);
  if (!videoFormat) {
    return false;
  }
  VideoFormatImpl* pVideoFormat = videoFormat.get();

  m_currentVideoFormatId = idVideoFormatPair.first;

  if (!pVideoFormat->setMediaTypeOfStream(m_pStreamConfig)) {
    return false;
  }

  return true;
}

static RGBVideoFormat firstVideoFormatFromList(
    std::list<RGBVideoFormat> videoFormatList) {
  if (!videoFormatList.size()) {
    RGBVideoFormat videoFormat;
    return videoFormat;
  }
  RGBVideoFormat videoFormat(videoFormatList.front());
  return videoFormat;
}

} // VideoCapture
