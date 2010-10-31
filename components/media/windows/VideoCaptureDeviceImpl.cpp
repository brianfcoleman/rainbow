#include "VideoCaptureDeviceImpl.hpp"
#include "VideoFormatImpl.hpp"
#include "IdUtilities.hpp"

namespace VideoCapture {

const nsString VideoCaptureDeviceImpl::s_kFriendlyName(
    NS_LITERAL_STRING("FriendlyName"));
const nsString VideoCaptureDeviceImpl::s_kNameCaptureFilter(
    NS_LITERAL_STRING("Capture Filter"));
const nsString VideoCaptureDeviceImpl::s_kNameSampleGrabberFilter(
    NS_LITERAL_STRING("Sample Grabber Filter"));
const nsString VideoCaptureDeviceImpl::s_kNameNullRendererFilter(
    NS_LITERAL_STRING("Null Renderer Filter"));
const nsString VideoCaptureDeviceImpl::s_kEmptyString(
    NS_LITERAL_STRING(""));
const double VideoCaptureDeviceImpl::s_kOneSecondNs = 1.0e9;

VideoCaptureDeviceImpl::VideoCaptureDeviceImpl(
    const QeditTypeLibrary& qeditTypeLibrary,
    COMAutoPtr<IMoniker>& pMoniker)
    : Uncopyable(),
      m_isInitialized(false),
      m_name(s_kEmptyString),
      m_IID_ISampleGrabber(qeditTypeLibrary.IID_ISampleGrabber()),
      m_IID_ISampleGrabberCB(qeditTypeLibrary.IID_ISampleGrabberCB()),
      m_pMoniker(pMoniker),
      m_pFilterGraph(createInstanceCOMInterface<IFilterGraph2>(
          CLSID_FilterGraph,
          IID_IFilterGraph2)),
      m_pCaptureGraphBuilder(createInstanceCOMInterface<ICaptureGraphBuilder2>(
          CLSID_CaptureGraphBuilder2,
          IID_ICaptureGraphBuilder2)),
      m_pSampleGrabberFilter(createInstanceCOMInterface<IBaseFilter>(
          qeditTypeLibrary.CLSID_SampleGrabber(),
          IID_IBaseFilter)),
      m_pNullRendererFilter(createInstanceCOMInterface<IBaseFilter>(
          qeditTypeLibrary.CLSID_NullRenderer(),
          IID_IBaseFilter)),
      m_currentVideoFormatId(kInvalidId) {
  m_isInitialized = initialize(qeditTypeLibrary);
  nsTArray<RGBVideoFormat> rgbVideoFormatList(videoFormatList());
  if (rgbVideoFormatList.IsEmpty()) {
    m_isInitialized = false;
    return;
  }
  RGBVideoFormat rgbVideoFormat(rgbVideoFormatList[0]);
  if (!rgbVideoFormat) {
    m_isInitialized = false;
    return;
  }
  m_isInitialized = setCurrentVideoFormat(rgbVideoFormat);
}

VideoCaptureDeviceImpl::~VideoCaptureDeviceImpl() {
  stopCapturing();
}

bool VideoCaptureDeviceImpl::initialize(
    const QeditTypeLibrary& qeditTypeLibrary) {
  if (!qeditTypeLibrary) {
    return false;
  }

  if (!initName()) {
    return false;
  }

  if (!initCaptureGraph()) {
    return false;
  }

  if (!initVideoFormatList()) {
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
  COMAutoPtr<IBindCtx> bindContextPtr(pBindContext);
  if (!bindContextPtr) {
    return false;
  }

  IPropertyBag* pPropertyBag = 0;
  IMoniker* pMoniker = m_pMoniker.get();
  result = pMoniker->BindToStorage(
      bindContextPtr.get(),
      0,
      IID_IPropertyBag,
      reinterpret_cast<void**>(&pPropertyBag));
  if (FAILED(result)) {
    return false;
  }
  COMAutoPtr<IPropertyBag> propertyBagPtr(pPropertyBag);
  if (!propertyBagPtr) {
    return false;
  }

  VariantAutoPtr variantPtr;
  VARIANT* pVariant = variantPtr.get();
  result = pPropertyBag->Read(s_kFriendlyName.get(), pVariant, 0);
  if (FAILED(result)) {
    return false;
  }

  const BSTR basicString = pVariant->bstrVal;
  if (!basicString) {
    return false;
  }
  nsString friendlyName(nsAStringFromBasicString(basicString));
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

  ICaptureGraphBuilder2* pCaptureGraphBuilder = m_pCaptureGraphBuilder.get();
  HRESULT result = pCaptureGraphBuilder->SetFiltergraph(m_pFilterGraph.get());
  return SUCCEEDED(result);
}

bool VideoCaptureDeviceImpl::buildCaptureGraph() {
  if (!m_pFilterGraph) {
    return false;
  }

  if (!m_pMoniker) {
    return false;
  }

  IBindCtx* pBindContext = 0;
  HRESULT result = CreateBindCtx(0, &pBindContext);
  if (FAILED(result)) {
    return false;
  }
  COMAutoPtr<IBindCtx> bindContextPtr(pBindContext);
  m_pBindContext = bindContextPtr;
  if (!m_pBindContext) {
    return false;
  }

  IBaseFilter* pCaptureFilter = 0;
  IFilterGraph2* pFilterGraph = m_pFilterGraph.get();
  result = pFilterGraph->AddSourceFilterForMoniker(
      m_pMoniker.get(),
      m_pBindContext.get(),
      s_kNameCaptureFilter.get(),
      &pCaptureFilter);
  if (FAILED(result)) {
    return false;
  }
  COMAutoPtr<IBaseFilter> captureFilterPtr(pCaptureFilter);
  m_pCaptureFilter = captureFilterPtr;
  if (!m_pCaptureFilter) {
    return false;
  }

  if (!m_pSampleGrabberFilter) {
    return false;
  }

  result = pFilterGraph->AddFilter(
      m_pSampleGrabberFilter.get(),
      s_kNameSampleGrabberFilter.get());
  if (FAILED(result)) {
    return false;
  }

  if (!m_pNullRendererFilter) {
    return false;
  }

  result = pFilterGraph->AddFilter(
      m_pNullRendererFilter.get(),
      s_kNameNullRendererFilter.get());
  if (FAILED(result)) {
    return false;
  }

  if (!m_pCaptureGraphBuilder) {
    return false;
  }

  ICaptureGraphBuilder2* pCaptureGraphBuilder = m_pCaptureGraphBuilder.get();
  result = pCaptureGraphBuilder->RenderStream(
      &PIN_CATEGORY_CAPTURE,
      &MEDIATYPE_Video,
      m_pCaptureFilter.get(),
      m_pSampleGrabberFilter.get(),
      m_pNullRendererFilter.get());
  return SUCCEEDED(result);
}

bool VideoCaptureDeviceImpl::initSampleGrabber() {
  if (!m_pSampleGrabberFilter) {
    return false;
  }

  IBaseFilter* pSampleGrabberFilter = m_pSampleGrabberFilter.get();
  COMAutoPtr<ISampleGrabber> sampleGrabberPtr(
      queryCOMInterface<IBaseFilter, ISampleGrabber>(
      pSampleGrabberFilter,
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

  COMAutoPtr<IMediaControl> mediaControlPtr(
      queryCOMInterface<IFilterGraph2, IMediaControl>(
          m_pFilterGraph.get(),
          IID_IMediaControl));
  m_pMediaControl = mediaControlPtr;
  if (!m_pMediaControl) {
    return false;
  }

  return true;
}

bool VideoCaptureDeviceImpl::initVideoControl() {
  if (!m_pCaptureGraphBuilder) {
    return false;
  }

  if (!m_pCaptureFilter) {
    return false;
  }

  IAMVideoControl* pVideoControl = 0;
  ICaptureGraphBuilder2* pCaptureGraphBuilder = m_pCaptureGraphBuilder.get();
  HRESULT result = pCaptureGraphBuilder->FindInterface(
      &PIN_CATEGORY_CAPTURE,
      0,
      m_pCaptureFilter.get(),
      IID_IAMVideoControl,
      reinterpret_cast<void**>(&pVideoControl));
  if (FAILED(result)) {
    return false;
  }
  COMAutoPtr<IAMVideoControl> videoControlPtr(pVideoControl);
  m_pVideoControl = videoControlPtr;
  if (!m_pVideoControl) {
    return false;
  }

  return true;
}

bool VideoCaptureDeviceImpl::initCapturePin() {
  if (!m_pCaptureGraphBuilder) {
    return false;
  }

  if (!m_pCaptureFilter) {
    return false;
  }

  IPin* pCapturePin = 0;
  ICaptureGraphBuilder2* pCaptureGraphBuilder = m_pCaptureGraphBuilder.get();
  HRESULT result = pCaptureGraphBuilder->FindPin(
      m_pCaptureFilter.get(),
      PINDIR_OUTPUT,
      &PIN_CATEGORY_CAPTURE,
      &MEDIATYPE_Video,
      FALSE,
      0,
      &pCapturePin);
  if (FAILED(result)) {
    return false;
  }
  COMAutoPtr<IPin> capturePinPtr(pCapturePin);
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
  ICaptureGraphBuilder2* pCaptureGraphBuilder = m_pCaptureGraphBuilder.get();
  HRESULT result = pCaptureGraphBuilder->FindInterface(
      &PIN_CATEGORY_CAPTURE,
      0,
      m_pCaptureFilter.get(),
      IID_IAMStreamConfig,
      reinterpret_cast<void**>(&pStreamConfig));
  if (FAILED(result)) {
    return false;
  }
  COMAutoPtr<IAMStreamConfig> streamConfigPtr(pStreamConfig);
  m_pStreamConfig = streamConfigPtr;
  if (!m_pStreamConfig) {
    return false;
  }

  return true;
}

bool VideoCaptureDeviceImpl::initVideoFormatList() {
  if (!m_pStreamConfig) {
    return false;
  }

  int countCapabilities = 0;
  int sizeBytes = 0;
  IAMStreamConfig* pStreamConfig = m_pStreamConfig.get();
  HRESULT result = pStreamConfig->GetNumberOfCapabilities(
      &countCapabilities,
      &sizeBytes);
  if (FAILED(result)) {
    return false;
  }

  if (sizeBytes != sizeof(VIDEO_STREAM_CONFIG_CAPS)) {
    return false;
  }

  for (PRInt32 index = 0, id = 0; index < countCapabilities; ++index) {
    VideoFormatImpl* pVideoFormatImpl = new VideoFormatImpl(
        pStreamConfig,
        index,
        id);
    if (!pVideoFormatImpl) {
      continue;
    }
    if (!m_videoFormatImplList.AppendElement(pVideoFormatImpl)) {
      nsAutoPtr<VideoFormatImpl> videoFormatImplPtr(pVideoFormatImpl);
      continue;
    }
    if (!pVideoFormatImpl->isInitialized()) {
      continue;
    }
    if (!pVideoFormatImpl->isRGBFormat()) {
      continue;
    }
    VideoFormat videoFormat(pVideoFormatImpl);
    if (!videoFormat) {
      continue;
    }
    if (!m_videoFormatList.InsertElementAt(id, videoFormat)) {
      continue;
    }
    ++id;
  }

  return (!m_videoFormatList.IsEmpty());
}

bool VideoCaptureDeviceImpl::isInitialized() const {
  return m_isInitialized;
}

nsString VideoCaptureDeviceImpl::name() const {
  return m_name;
}

bool VideoCaptureDeviceImpl::addOnNewVideoFrameCallback(
    ByteBufferCallback* const pCallback) {
  if (!pCallback) {
    return false;
  }
  if (m_byteBufferCallbackSet.Contains(pCallback)) {
    return false;
  }
  if (!m_byteBufferCallbackSet.AppendElement(pCallback)) {
    return false;
  }
  return true;
}

bool VideoCaptureDeviceImpl::removeOnNewVideoFrameCallback(
    ByteBufferCallback* const pCallback) {
  if (!pCallback) {
    return false;
  }
  if (m_byteBufferCallbackSet.IsEmpty()) {
    return false;
  }
  if (!m_byteBufferCallbackSet.Contains(pCallback)) {
    return false;
  }
  if (!m_byteBufferCallbackSet.RemoveElement(pCallback)) {
    return false;
  }
  return true;
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

  IMediaControl* pMediaControl = m_pMediaControl.get();
  HRESULT result = pMediaControl->Run();

  return SUCCEEDED(result);
}

bool VideoCaptureDeviceImpl::stopCapturing() {
  if (!isInitialized()) {
    return true;
  }

  IMediaControl* pMediaControl = m_pMediaControl.get();
  HRESULT result = pMediaControl->Stop();
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
  IAMVideoControl* pVideoControl = m_pVideoControl.get();
  HRESULT result = pVideoControl->GetCurrentActualFrameRate(
      m_pCapturePin.get(),
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

nsTArray<RGBVideoFormat> VideoCaptureDeviceImpl::videoFormatList() const {
  if (!isInitialized()) {
    nsTArray<RGBVideoFormat> emptyList;
    return emptyList;
  }

  nsTArray<RGBVideoFormat> rgbVideoFormatList;
  for (PRUint32 i = 0; i < m_videoFormatList.Length(); ++i) {
    VideoFormat videoFormat = m_videoFormatList[i];
    if (!videoFormat) {
      continue;
    }
    RGBVideoFormat rgbVideoFormat(videoFormat);
    if (!rgbVideoFormat) {
      continue;
    }
    rgbVideoFormatList.AppendElement(rgbVideoFormat);
  }
  return rgbVideoFormatList;
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

  if (m_currentVideoFormatId < 0) {
    RGBVideoFormat rgbVideoFormat;
    return rgbVideoFormat;
  }

  if (m_currentVideoFormatId >= m_videoFormatList.Length()) {
    RGBVideoFormat rgbVideoFormat;
    return rgbVideoFormat;
  }

  VideoFormat videoFormat(m_videoFormatList[m_currentVideoFormatId]);
  if (!videoFormat) {
    RGBVideoFormat rgbVideoFormat;
    return rgbVideoFormat;
  }

  RGBVideoFormat rgbVideoFormat(videoFormat);
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

  for (PRUint32 i = 0; i < m_videoFormatList.Length(); ++i) {
    VideoFormat videoFormat(m_videoFormatList[i]);
    if (!videoFormat) {
      continue;
    }
    if (rgbVideoFormat != videoFormat) {
      continue;
    }
    if (i != videoFormat.id()) {
      continue;
    }
    VideoFormatImpl* pVideoFormatImpl = videoFormat.get();
    if (!pVideoFormatImpl->setMediaTypeOfStream(m_pStreamConfig.get())) {
      return false;
    }
    m_currentVideoFormatId = videoFormat.id();
    return true;
  }

  return false;
}

} // VideoCapture
