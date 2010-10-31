#include "SampleGrabberCallback.hpp"

namespace VideoCapture {

SampleGrabberCallback::SampleGrabberCallback(
    const IID& IID_ISampleGrabberCB,
    const std::set<ByteBufferCallback*>& byteBufferCallbackSet)
    : m_refCount(1),
      m_IID_ISampleGrabberCB(IID_ISampleGrabberCB),
      m_byteBufferCallbackSet(byteBufferCallbackSet) {

}

STDMETHODIMP_(ULONG) SampleGrabberCallback::AddRef() {
  return ++m_refCount;
}

STDMETHODIMP_(ULONG) SampleGrabberCallback::Release() {
  if (!m_refCount) {
    return 0;
  }
  return --m_refCount;
}

STDMETHODIMP SampleGrabberCallback::QueryInterface(
    REFIID riid,
    void **ppvObject) {
  if (!ppvObject) {
    return E_POINTER;
  }

  if (riid == IID_IUnknown) {
    *ppvObject = static_cast<IUnknown*>(this);
    return S_OK;
  }

  if (riid == m_IID_ISampleGrabberCB) {
    *ppvObject = static_cast<ISampleGrabberCB*>(this);
    return S_OK;
  }

  return E_NOTIMPL;
}

STDMETHODIMP SampleGrabberCallback::SampleCB(
    double Time,
    IMediaSample *pSample) {
  return E_NOTIMPL;
}

STDMETHODIMP SampleGrabberCallback::BufferCB(
    double Time,
    BYTE *pBuffer,
    long BufferLen) {
  if (!pBuffer) {
    return S_FALSE;
  }

  if (BufferLen <= 0) {
    return S_FALSE;
  }

  ByteBuffer buffer(pBuffer, BufferLen);

  for(std::set<ByteBufferCallback*>::iterator iterator(
          m_byteBufferCallbackSet.begin());
      iterator != m_byteBufferCallbackSet.end();
      ++iterator) {
    ByteBufferCallback* pCallback(*iterator);
    pCallback->onNewByteBuffer(buffer);
  }

  return S_OK;
}

} // VideoCapture

