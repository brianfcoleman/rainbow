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
      const IAMStreamConfigPtr& pStreamConfig,
      const PRInt32 index,
      const PRInt32 id);
  bool isInitialized() const;
  bool isRGBFormat() const;
  PRInt32 id() const;
  double framesPerSecond() const;
  IntegerSize sizePixels() const;
  PRInt32 angleRotationDegrees() const;
  PRInt32 bitsPerPixel() const;
  PRInt32 sizeBytes() const;
  PRInt32 sizeRowBytes() const;
  RGBFormat rgbFormat() const;
  bool VideoFormatImpl::setMediaTypeOfStream(
      const IAMStreamConfigPtr& pStreamConfig);
  operator bool() const {
      return isInitialized();
  }
 private:
  MediaTypeAutoPtr m_pMediaType;
  VIDEO_STREAM_CONFIG_CAPS m_streamCapabilities;
  bool m_isInitialized;
  PRInt32 m_id;
  double m_framesPerSecond;
  IntegerSize m_sizePixels;
  PRInt32 m_angleRotationDegrees;
  PRInt32 m_bitsPerPixel;
  bool m_isUncompressedRGB;
  RGBFormat m_rgbFormat;
  bool initialize(
      const IAMStreamConfigPtr& pStreamConfig,
      const PRInt32 index);
  bool extractData();
  bool isVideoFormat() const;
};

} // VideoCapture

#endif // VIDEOCAPTURE_VIDEO_FORMAT_IMPL_H
