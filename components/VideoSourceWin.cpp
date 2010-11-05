#include "VideoSourceWin.h"

/*
 * Try to intelligently fetch a default video input device
 */
static HRESULT
GetDefaultInputDevice(IBaseFilter **ppSrcFilter)
{
    HRESULT hr = S_OK;
    IBaseFilter *pSrc = NULL;
    IMoniker *pMoniker = NULL;
    ICreateDevEnum *pDevEnum = NULL;
    IEnumMoniker *pClassEnum = NULL;

    if (!ppSrcFilter) {
        return E_POINTER;
    }
   
    hr = CoCreateInstance(
        CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC,
        IID_ICreateDevEnum, (void **)&pDevEnum
    );
    if (FAILED(hr)) return hr;
    
    hr = pDevEnum->CreateClassEnumerator(
        CLSID_VideoInputDeviceCategory, &pClassEnum, 0
    );
    if (FAILED(hr)) return hr;
    
    if (pClassEnum == NULL) {
        // No devices available
        return E_FAIL;
    }

    // Pick the first device from the list.
    // Note that if the Next() call succeeds but there are no monikers,
    // it will return S_FALSE (which is not a failure).
    hr = pClassEnum->Next (1, &pMoniker, NULL);
    if (hr == S_FALSE) return E_FAIL;
    
    hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&pSrc);
    if (FAILED(hr)) return hr;
    
    *ppSrcFilter = pSrc;
    (*ppSrcFilter)->AddRef();

    SAFE_RELEASE(pSrc);
    SAFE_RELEASE(pMoniker);
    SAFE_RELEASE(pDevEnum);
    SAFE_RELEASE(pClassEnum);

    return hr;
}

VideoSourceWin::VideoSourceWin(int n, int d, int w, int h)
    : VideoSource(n, d, w, h)
{
    HRESULT hr;
    CoInitialize(0);

    hr = CoCreateInstance(
        CLSID_FilterGraph, NULL, CLSCTX_INPROC,
        IID_IGraphBuilder, (void **) &pGraph
    );
    if (FAILED(hr)) return;

    hr = CoCreateInstance(
        CLSID_CaptureGraphBuilder2 , NULL, CLSCTX_INPROC,
        IID_ICaptureGraphBuilder2, (void **) &pCapture
    );
    if (FAILED(hr)) return;

    hr = pGraph->QueryInterface(IID_IMediaControl, (LPVOID *) &pMC);
    if (FAILED(hr)) return;
    hr = pCapture->SetFiltergraph(pGraph);
    if (FAILED(hr)) return;

    hr = GetDefaultInputDevice(&pSrcFilter);
    if (FAILED(hr)) return;

    // Add capture device to graph
    hr = pGraph->AddFilter(pSrcFilter, L"Video Capture");
    if (FAILED(hr)) {
        pSrcFilter->Release();
        return;
    }

    // Create SampleGrabber
    hr = CoCreateInstance(
        CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&pGrabberF)
    );
    if (FAILED(hr)) {
        pSrcFilter->Release();
        return;
    }
    hr = pGrabberF->QueryInterface(IID_ISampleGrabber, (void**)&pGrabber);
    if (FAILED(hr)) {
        pSrcFilter->Release();
        return;
    }
 
    // Create Null Renderer (required sink by ISampleGrabber)
    hr = CoCreateInstance(
        CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER,
        IID_IBaseFilter, (void **)&pNullF
    );
    if (FAILED(hr)) {
        pSrcFilter->Release();
        return;
    }
}

VideoSourceWin::~VideoSourceWin()
{
    CoUninitialize();
    SAFE_RELEASE(pMC);
    SAFE_RELEASE(pNullF);
    SAFE_RELEASE(pGraph);
    SAFE_RELEASE(pCapture);
    SAFE_RELEASE(pGrabber);
}

nsresult
VideoSourceWin::Start(nsIOutputStream *pipe)
{
    HRESULT hr;
    AM_MEDIA_TYPE mt;
    AM_MEDIA_TYPE *pCapmt;
    VIDEOINFOHEADER *pVid;
    IAMStreamConfig *pConfig;
    
    hr = pCapture->FindInterface(
        &PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video,pSrcFilter,
        IID_IAMStreamConfig, (void**)&pConfig
    );
    if (FAILED(hr)) {
        pSrcFilter->Release();
        return NS_ERROR_FAILURE;
    }
    
    hr = pConfig->GetFormat(&pCapmt);
    pVid = reinterpret_cast<VIDEOINFOHEADER*>(pCapmt->pbFormat);
    pVid->bmiHeader.biWidth = width;
    pVid->bmiHeader.biHeight = height;
    pVid->AvgTimePerFrame = 10000000 * (fps_d / fps_n);
    pConfig->SetFormat(pCapmt);
    
    ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
    mt.majortype = MEDIATYPE_Video;
    mt.subtype = MEDIASUBTYPE_ARGB32;
    mt.formattype = FORMAT_VideoInfo;
    hr = pGrabber->SetMediaType(&mt);
    hr = pGrabber->SetBufferSamples(TRUE);
    
    // Set our callback
    cb = new VideoSourceWinCallback(pipe);
    hr = pGrabber->SetCallback(cb, 1);
    
    // Add Sample Grabber to graph
    hr = pGraph->AddFilter(pGrabberF, L"Sample Grabber");
    if (FAILED(hr)) {
        pSrcFilter->Release();
        return NS_ERROR_FAILURE;
    }

    // Add Null Renderer to graph
    hr = pGraph->AddFilter(pNullF, L"Null Filter");
    if (FAILED(hr)) {
        pSrcFilter->Release();
        return NS_ERROR_FAILURE;
    }

    // Connect capture pin to sample grabber, and that to null renderer
    hr = pCapture->RenderStream(
        &PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video,
        pSrcFilter, pGrabberF, pNullF
    );
    if (FAILED(hr)) {
        pSrcFilter->Release();
        return NS_ERROR_FAILURE;
    }
    pSrcFilter->Release();
    
    hr = pMC->Run();
    if (FAILED(hr)) return NS_ERROR_FAILURE;

    return NS_OK;
}

nsresult
VideoSourceWin::Stop()
{
    pMC->StopWhenReady();
    delete cb;
    return NS_OK;
}

// Callback class
VideoSourceWinCallback::VideoSourceWinCallback(nsIOutputStream *pipe)
{
    output = pipe;
    m_refCount = 0;
}

STDMETHODIMP_(ULONG)
VideoSourceWinCallback::AddRef()
{
    return ++m_refCount;
}

STDMETHODIMP_(ULONG)
VideoSourceWinCallback::Release()
{
    if (!m_refCount) {
        return 0;
    }
    return --m_refCount;
}

STDMETHODIMP
VideoSourceWinCallback::QueryInterface(REFIID riid, void **ppvObject)
{
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

STDMETHODIMP
VideoSourceWinCallback::SampleCB(double Time, IMediaSample *pSample)
{
    return E_NOTIMPL;
}

STDMETHODIMP
VideoSourceWinCallback::BufferCB(double Time, BYTE *pBuffer, long BufferLen)
{
    nsresult rv;
    PRUint32 wr;

    rv = output->Write((const char *)pBuffer, BufferLen, &wr);
    return S_OK;
}