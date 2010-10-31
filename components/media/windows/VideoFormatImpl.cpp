#include "VideoFormatImpl.hpp"
#include "IdUtilities.hpp"

namespace VideoCapture {

VideoFormatImpl::VideoFormatImpl()
    : m_isInitialized(false),
      m_id(kInvalidId),
      m_framesPerSecond(0),
      m_angleRotationDegrees(0),
      m_bitsPerPixel(0),
      m_isUncompressedRGB(false),
      m_rgbFormat(kRGBNone) {

}

VideoFormatImpl::VideoFormatImpl(
    IAMStreamConfig* const pStreamConfig,
    const PRUint32 index,
    const PRUint32 id)
    : m_isInitialized(false),
      m_id(id),
      m_framesPerSecond(0),
      m_angleRotationDegrees(0),
      m_bitsPerPixel(0),
      m_isUncompressedRGB(false),
      m_rgbFormat(kRGBNone) {
  m_isInitialized = initialize(pStreamConfig, index);
}

bool VideoFormatImpl::isInitialized() const {
  return m_isInitialized;
}

bool VideoFormatImpl::isVideoFormat() const {
  if (!m_pMediaType) {
    return false;
  }
  AM_MEDIA_TYPE* pMediaType = m_pMediaType.get();
  if (pMediaType->majortype != MEDIATYPE_Video) {
    return false;
  }

  if (pMediaType->formattype != FORMAT_VideoInfo) {
    return false;
  }

  if (pMediaType->cbFormat != sizeof(VIDEOINFOHEADER)) {
    return false;
  }

  if (!pMediaType->pbFormat) {
    return false;
  }

  return true;
}

bool VideoFormatImpl::isRGBFormat() const {
  if (!isInitialized()) {
    return false;
  }

  if (!isVideoFormat()) {
    return false;
  }

  if (!m_isUncompressedRGB) {
    return false;
  }

  if (m_rgbFormat == kRGBNone) {
    return false;
  }

  return true;
}

bool VideoFormatImpl::initialize(
    IAMStreamConfig* const pStreamConfig,
    const PRUint32 index) {
  if (!pStreamConfig) {
    return false;
  }

  if (!isValidId(m_id)) {
    return false;
  }

  AM_MEDIA_TYPE* pMediaType = 0;
  HRESULT result;
  result = pStreamConfig->GetStreamCaps(
      index,
      &pMediaType,
      reinterpret_cast<PRUint8*>(&m_streamCapabilities));
  if (FAILED(result)) {
    return false;
  }
  MediaTypeAutoPtr mediaTypeAutoPtr(pMediaType);
  m_pMediaType = mediaTypeAutoPtr;
  if (!m_pMediaType) {
    return false;
  }

  if (!extractData()) {
    return false;
  }

  return true;
}

bool VideoFormatImpl::extractData() {
  if (!isVideoFormat()) {
    return false;
  }

  AM_MEDIA_TYPE* pMediaType = m_pMediaType.get();
  VIDEOINFOHEADER& videoHeader(
      *(reinterpret_cast<VIDEOINFOHEADER*>(pMediaType->pbFormat)));
  BITMAPINFOHEADER& bmiHeader(videoHeader.bmiHeader);

  double timePerFrameMs =
      static_cast<double>(videoHeader.AvgTimePerFrame) / 10000.0;
  m_framesPerSecond = 1000.0 / timePerFrameMs;

  PRInt32 widthPixels = bmiHeader.biWidth;
  PRInt32 heightPixels = bmiHeader.biHeight;
  if (heightPixels >= 0) {
    m_angleRotationDegrees = kAngleHalfRotationDegrees;
  } else {
    m_angleRotationDegrees = 0;
    heightPixels = -heightPixels;
  }
  IntegerSize sizePixels(widthPixels, heightPixels);
  m_sizePixels = sizePixels;

  m_bitsPerPixel = bmiHeader.biBitCount;

  if (bmiHeader.biCompression == BI_RGB) {
    m_isUncompressedRGB = true;
  } else {
    m_isUncompressedRGB = false;
  }

  if (pMediaType->subtype == MEDIASUBTYPE_RGB24) {
    m_rgbFormat = kBGR888;
  }

  if (pMediaType->subtype == MEDIASUBTYPE_RGB32) {
    m_rgbFormat = kABGR8888;
  }

  return true;
}

PRUint32 VideoFormatImpl::id() const {
  if (!isInitialized()) {
    return kInvalidId;
  }
  return m_id;
}

double VideoFormatImpl::framesPerSecond() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_framesPerSecond;
}

IntegerSize VideoFormatImpl::sizePixels() const {
  if (!isInitialized()) {
    IntegerSize zeroSize;
    return zeroSize;
  }
  return m_sizePixels;
}

PRInt32 VideoFormatImpl::angleRotationDegrees() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_angleRotationDegrees;
}

PRInt32 VideoFormatImpl::bitsPerPixel() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_bitsPerPixel;
}

PRInt32 VideoFormatImpl::sizeBytes() const {
  if (!isInitialized()) {
    return 0;
  }
  return sizeBytesForRGBFormat(m_sizePixels, m_rgbFormat);
}

PRInt32 VideoFormatImpl::sizeRowBytes() const {
  if (!isInitialized()) {
    return false;
  }
  return sizeRowBytesForRGBFormat(m_sizePixels, m_rgbFormat);
}

RGBFormat VideoFormatImpl::rgbFormat() const {
  if (!isInitialized()) {
    return kRGBNone;
  }
  return m_rgbFormat;
}

bool VideoFormatImpl::setMediaTypeOfStream(
    IAMStreamConfig* const pStreamConfig) {
  if (!pStreamConfig) {
    return false;
  }

  if (!isInitialized()) {
    return false;
  }

  if (!isRGBFormat()) {
    return false;
  }

  if (!pStreamConfig) {
    return false;
  }

  HRESULT result;
  result = pStreamConfig->SetFormat(m_pMediaType.get());
  return SUCCEEDED(result);
}

} // VideoCapture