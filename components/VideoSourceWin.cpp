/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Rainbow.
 *
 * The Initial Developer of the Original Code is Mozilla Labs.
 * Portions created by the Initial Developer are Copyright (C) 2010
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Anant Narayanan <anant@kix.in>
 *   Brian Coleman <brianfcoleman@gmail.com>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include "VideoSourceWin.h"

static const PRInt32 kBitsPerByte = 8;
static const PRUint8 kMaxAlpha = 255;

static void convertVideoFrameToNonInvertedBGRA8888(
    const PRUint8* pVideoFrameData,
    VideoCapture::RGBVideoFormat& videoFormat,
    nsAutoArrayPtr<PRUint8>& pConvertedVideoFrameData);

static void convertVideoFrameToBGRA8888(
    const PRUint8* pVideoFrameData,
    VideoCapture::RGBVideoFormat& videoFormat,
    nsAutoArrayPtr<PRUint8>& pConvertedVideoFrameData);

static void convertVideoFrameToNonInverted(
    PRUint8* pVideoFrameData,
    VideoCapture::RGBVideoFormat& videoFormat);

/*
 * Try to intelligently fetch a default video input device
 */
static VideoCapture::VideoCaptureDevice
GetDefaultInputDevice(VideoCapture::VideoCaptureDeviceManager deviceManager)
{
  using namespace VideoCapture;
  if (!deviceManager) {
    VideoCaptureDevice captureDevice;
    return captureDevice;
  }
  nsTArray<VideoCaptureDevice> deviceList(
      deviceManager.videoCaptureDeviceList());
  if (!deviceList.Length()) {
    VideoCaptureDevice captureDevice;
    return captureDevice;
  }
  VideoCaptureDevice captureDevice(deviceList[0]);
  return captureDevice;
}

VideoSourceWin::VideoSourceWin(int n, int d, int w, int h)
    : VideoSource(n, d, w, h),
      mDeviceManager(
          VideoCapture::VideoCaptureDeviceManager::
          createVideoCapptureDeviceManager())
{
#ifdef DEBUG
  printf("VideoSourceWin::VideoSourceWin\n");
#endif
  if (!mDeviceManager) {
    return;
  }
  mCaptureDevice = GetDefaultInputDevice(mDeviceManager);
}

VideoSourceWin::~VideoSourceWin()
{
#ifdef DEBUG
  printf("VideoSourceWin::~VideoSourceWin\n");
#endif
}

nsresult
VideoSourceWin::Start(nsIOutputStream *pipe)
{
#ifdef DEBUG
  printf("VideoSourceWin::Start\n");
#endif
  if (!mCaptureDevice) {
    return NS_ERROR_FAILURE;
  }
  using namespace VideoCapture;
  nsTArray<RGBVideoFormat> formatList(mCaptureDevice.videoFormatList());
  if (!formatList.Length()) {
    return NS_ERROR_FAILURE;
  }
  IntegerSize desiredSizePixels(width, height);
  RGBVideoFormat invalidVideoFormat;
  mVideoFormat = invalidVideoFormat;
  for (PRUint32 i = 0; i < formatList.Length(); ++i) {
    RGBVideoFormat videoFormat(formatList[i]);
    IntegerSize sizePixels(videoFormat.sizePixels());
    if (sizePixels != desiredSizePixels) {
      continue;
    }
    mVideoFormat = videoFormat;
  }
  if (!mVideoFormat) {
    return NS_ERROR_FAILURE;
  }
  if (!mCaptureDevice.setCurrentVideoFormat(mVideoFormat)) {
    return NS_ERROR_FAILURE;
  }
  nsAutoPtr<VideoSourceWinCallback> callback(new VideoSourceWinCallback(pipe));
  if (!mCaptureDevice.addOnNewVideoFrameCallback(callback.get())) {
    return NS_ERROR_FAILURE;
  }
  if (!mCaptureDevice.startCapturing()) {
    return NS_ERROR_FAILURE;
  }
  // Swap the VideoSourceWinCallback pointers
  // Get the old VideoSourceWinCallback
  VideoSourceWinCallback* pCB = cb.forget();
  // Store the new VideoSourceWinCallback
  cb = callback;
  // Make sure that the old VideoSourceWinCallback is deleted
  callback = pCB;
  return NS_OK;
}

nsresult
VideoSourceWin::Stop()
{
#ifdef DEBUG
  printf("VideoSourceWin::Stop\n");
#endif
  if (!mCaptureDevice) {
    return NS_ERROR_FAILURE;
  }
  if (!mVideoFormat) {
    return NS_ERROR_FAILURE;
  }
  mCaptureDevice.stopCapturing();
  return NS_OK;
}

// Callback class
VideoSourceWinCallback::VideoSourceWinCallback(nsIOutputStream *pipe)
{
#ifdef DEBUG
  printf("VideoSourceWinCallback::VideoSourceWinCallback\n");
#endif
  output = pipe;
}

void
VideoSourceWinCallback::onNewVideoFrame(
    const VideoCapture::ByteBuffer& byteBuffer,
    const VideoCapture::RGBVideoFormat& videoFormat)
{
  using namespace VideoCapture;
  if (!videoFormat) {
    return;
  }
  if (!byteBuffer.hasData()) {
    return;
  }
  if (byteBuffer.sizeBytes() != videoFormat.sizeBytes()) {
    return;
  }
  RGBVideoFormat convertedVideoFormat(videoFormat);
  nsAutoArrayPtr<PRUint8> pConvertedVideoFrameData;
  convertVideoFrameToNonInvertedBGRA8888(
      byteBuffer.firstBufferByte(),
      convertedVideoFormat,
      pConvertedVideoFrameData);
  if (!convertedVideoFormat) {
    return;
  }
  if (!pConvertedVideoFrameData.get()) {
    return;
  }
  nsresult rv;
  PRUint32 wr;
  rv = output->Write(
      reinterpret_cast<const char*>(pConvertedVideoFrameData.get()),
      convertedVideoFormat.sizeBytes(),
      &wr);
}

static void convertVideoFrameToNonInvertedBGRA8888(
    const PRUint8* pVideoFrameData,
    VideoCapture::RGBVideoFormat& videoFormat,
    nsAutoArrayPtr<PRUint8>& pConvertedVideoFrameData) {
  using namespace VideoCapture;
  if (!videoFormat) {
    return;
  }
  if (!pVideoFrameData) {
    return;
  }
  convertVideoFrameToBGRA8888(
      pVideoFrameData,
      videoFormat,
      pConvertedVideoFrameData);
  convertVideoFrameToNonInverted(
      pConvertedVideoFrameData.get(),
      videoFormat);
}

static void convertVideoFrameToBGRA8888(
    const PRUint8* pVideoFrameData,
    VideoCapture::RGBVideoFormat& videoFormat,
    nsAutoArrayPtr<PRUint8>& pConvertedVideoFrameData) {
  using namespace VideoCapture;
  if (!videoFormat) {
    return;
  }
  if (!pVideoFrameData) {
    return;
  }
  RGBVideoFormat convertedVideoFormat(
      videoFormat.id(),
      videoFormat.sizePixels(),
      videoFormat.angleRotationDegrees(),
      kBGRA8888);
  PRInt32 convertedSizeBytes = convertedVideoFormat.sizeBytes();
  PRInt32 unconvertedSizeBytes = videoFormat.sizeBytes();
  PRInt32 convertedBytesPerPixel =
      convertedVideoFormat.bitsPerPixel() / kBitsPerByte;
  PRInt32 unconvertedBytesPerPixel =
      videoFormat.bitsPerPixel() / kBitsPerByte;
  pConvertedVideoFrameData = new PRUint8[convertedSizeBytes];
  PRUint8* pConvertedPixels = pConvertedVideoFrameData.get();
  const PRUint8* pUnconvertedPixels = pVideoFrameData;
  RGBFormat unconvertedRGBFormat = videoFormat.rgbFormat();
  for (PRInt32 indexUnconverted = 0, indexConverted = 0;
       indexUnconverted < unconvertedSizeBytes &&
           indexConverted < convertedSizeBytes;
       indexUnconverted += unconvertedBytesPerPixel,
           indexConverted += convertedBytesPerPixel) {
    PRInt32 offsetBlue = 0;
    PRInt32 offsetGreen = 1;
    PRInt32 offsetRed = 2;
    PRInt32 offsetAlpha = 3;
    pConvertedPixels[indexConverted + offsetBlue] =
        pUnconvertedPixels[indexUnconverted + offsetBlue];
    pConvertedPixels[indexConverted + offsetGreen] =
        pUnconvertedPixels[indexUnconverted + offsetGreen];
    pConvertedPixels[indexConverted + offsetRed] =
        pUnconvertedPixels[indexUnconverted + offsetRed];
    pConvertedPixels[indexConverted + offsetAlpha] = kMaxAlpha;
  }
  videoFormat = convertedVideoFormat;
  return;
}

static void convertVideoFrameToNonInverted(
    PRUint8* pVideoFrameData,
    VideoCapture::RGBVideoFormat& videoFormat) {
  using namespace VideoCapture;
  if (!videoFormat) {
    return;
  }
  if (!pVideoFrameData) {
    return;
  }
  PRInt32 angleRotationDegrees = videoFormat.angleRotationDegrees();
  if (angleRotationDegrees != kAngleHalfRotationDegrees) {
    return;
  }
  angleRotationDegrees = 0;
  RGBVideoFormat convertedVideoFormat(
      videoFormat.id(),
      videoFormat.sizePixels(),
      angleRotationDegrees,
      videoFormat.rgbFormat());
  PRInt32 sizeBytes = videoFormat.sizeBytes();
  PRInt32 bytesPerPixel = videoFormat.bitsPerPixel() / kBitsPerByte;
  for (PRInt32 indexStart = 0, indexEnd = sizeBytes - bytesPerPixel;
       indexStart < sizeBytes / 2;
       indexStart += bytesPerPixel, indexEnd -= bytesPerPixel) {
    for (PRInt32 offset = 0; offset < bytesPerPixel; ++offset) {
      PRUint8 dataStart = pVideoFrameData[indexStart + offset];
      PRUint8 dataEnd = pVideoFrameData[indexEnd + offset];
      pVideoFrameData[indexStart + offset] = dataEnd;
      pVideoFrameData[indexEnd + offset] = dataStart;
    }
  }
  videoFormat = convertedVideoFormat;
}
