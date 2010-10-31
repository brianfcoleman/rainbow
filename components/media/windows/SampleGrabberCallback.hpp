#ifndef VIDEO_CAPTURE_SAMPLE_GRABBER_CALLBACK_H
#define VIDEO_CAPTURE_SAMPLE_GRABBER_CALLBACK_H

#include "nsTArray.h"
#include "ByteBufferCallback.hpp"
#include "DirectShow.hpp"
#include "Uncopyable.hpp"

namespace VideoCapture {

class SampleGrabberCallback
    : public ISampleGrabberCB,
      private Uncopyable {
 public:
  SampleGrabberCallback(
      const IID& IID_ISampleGrabberCB,
      const nsTArray<ByteBufferCallback*>& byteBufferCallbackSet);
  STDMETHODIMP_(ULONG) AddRef();
  STDMETHODIMP_(ULONG) Release();
  STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject);
  STDMETHODIMP SampleCB(double Time, IMediaSample *pSample);
  STDMETHODIMP BufferCB(double Time, BYTE *pBuffer, long BufferLen);
 private:
  PRInt32 m_refCount;
  IID m_IID_ISampleGrabberCB;
  nsTArray<ByteBufferCallback*> m_byteBufferCallbackSet;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_GRABBER_CALLBACK_H
