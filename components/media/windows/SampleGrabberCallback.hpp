#ifndef VIDEO_CAPTURE_SAMPLE_GRABBER_CALLBACK_H
#define VIDEO_CAPTURE_SAMPLE_GRABBER_CALLBACK_H

#include "nsTArray.h"
#include "DirectShow.hpp"
#include "Uncopyable.hpp"
#include "VideoFrameCallback.hpp"

namespace VideoCapture {

class SampleGrabberCallback
    : public ISampleGrabberCB,
      private Uncopyable {
 public:
  SampleGrabberCallback(
      const IID& IID_ISampleGrabberCB,
      const RGBVideoFormat& videoFormat,
      const nsTArray<VideoFrameCallback*>& videoFrameCallbackSet);
  STDMETHODIMP_(ULONG) AddRef();
  STDMETHODIMP_(ULONG) Release();
  STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject);
  STDMETHODIMP SampleCB(double Time, IMediaSample *pSample);
  STDMETHODIMP BufferCB(double Time, BYTE *pBuffer, long BufferLen);
 private:
  PRInt32 m_refCount;
  IID m_IID_ISampleGrabberCB;
  RGBVideoFormat m_videoFormat;
  nsTArray<VideoFrameCallback*> m_videoFrameCallbackSet;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_GRABBER_CALLBACK_H
