#ifndef VIDEOCAPTURE_DIRECT_SHOW_H
#define VIDEOCAPTURE_DIRECT_SHOW_H

#include "comdef.h"
#include "dshow.h"
#include "MediaTypeAutoPtr.hpp"
#include "QeditTypeLibrary.hpp"

namespace VideoCapture {

_COM_SMARTPTR_TYPEDEF(IAMStreamConfig, __uuidof(IAMStreamConfig));
_COM_SMARTPTR_TYPEDEF(IFilterGraph2, __uuidof(IFilterGraph2));
_COM_SMARTPTR_TYPEDEF(ICaptureGraphBuilder2, _uuidof(ICaptureGraphBuilder2));
_COM_SMARTPTR_TYPEDEF(IBaseFilter, __uuidof(IBaseFilter));
_COM_SMARTPTR_TYPEDEF(IMediaControl, __uuidof(IMediaControl));
_COM_SMARTPTR_TYPEDEF(IAMVideoControl, __uuidof(IAMVideoControl));
_COM_SMARTPTR_TYPEDEF(IPin, __uuidof(IPin));

#undef INTERFACE
#define INTERFACE ISampleGrabberCB

DECLARE_INTERFACE_ (ISampleGrabberCB, IUnknown) {
  STDMETHOD (QueryInterface) (THIS_
                              REFIID,
                              void **) PURE;
  STDMETHOD_ (ULONG, AddRef) (THIS) PURE;
  STDMETHOD_ (ULONG, Release) (THIS) PURE;

  STDMETHOD (SampleCB) (THIS_
                        double SampleTime,
                        IMediaSample *pSample) PURE;
  STDMETHOD (BufferCB) (THIS_
                        double SampleTime,
                        BYTE *pBuffer,
                        long BufferLen) PURE;
};

#undef INTERFACE
#define INTERFACE ISampleGrabber

DECLARE_INTERFACE_ (ISampleGrabber, IUnknown) {
  STDMETHOD (QueryInterface) (THIS_
                              REFIID,
                              void **) PURE;
  STDMETHOD_ (ULONG, AddRef) (THIS) PURE;
  STDMETHOD_ (ULONG, Release) (THIS) PURE;

  STDMETHOD (SetOneShot) (THIS_
                          BOOL OneShot) PURE;
  STDMETHOD (SetMediaType) (THIS_
                            const AM_MEDIA_TYPE *pType) PURE;
  STDMETHOD (GetConnectedMediaType) (THIS_
                                     AM_MEDIA_TYPE *pType) PURE;
  STDMETHOD (SetBufferSamples) (THIS_
                                BOOL BufferThem) PURE;
  STDMETHOD (GetCurrentBuffer) (THIS_
                                long *pBufferSize,
                                long *pBuffer) PURE;
  STDMETHOD (GetCurrentSample) (THIS_
                                IMediaSample **ppSample) PURE;
  STDMETHOD (SetCallback) (THIS_
                           ISampleGrabberCB *pCallback,
                           long WhichMethodToCallback) PURE;
};

} // VideoCapture

#endif // VIDEOCAPTURE_DIRECT_SHOW_H
