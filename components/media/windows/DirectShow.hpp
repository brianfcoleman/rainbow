#ifndef VIDEOCAPTURE_DIRECT_SHOW_H
#define VIDEOCAPTURE_DIRECT_SHOW_H

#include "comdef.h"
#include "dshow.h"
#include "BStringUtilities.hpp"
#include "COMManager.hpp"
#include "MediaTypeAutoPtr.hpp"
#include "QeditTypeLibrary.hpp"
#include "VariantAutoPtr.hpp"

namespace VideoCapture {

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
