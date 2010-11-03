#include <cstdlib>
#include <cstdio>
#include "nsAutoPtr.h"
#include "VideoCaptureDeviceManager.hpp"

static const VideoCapture::IntegerSize kDesiredSizePixels(640, 480);

static const PRUint32 kSleepTimeSeconds = 10;

int main(int argc, char* argv[]) {
  using namespace VideoCapture;
  printf("main\n");
  VideoCaptureDeviceManager deviceManager(
      VideoCaptureDeviceManager::createVideoCapptureDeviceManager());
  if (!deviceManager) {
    return EXIT_FAILURE;
  }
  printf("deviceManager initialized\n");
  nsTArray<VideoCaptureDevice> deviceList(
      deviceManager.videoCaptureDeviceList());
  if (!deviceList.Length()) {
    return EXIT_FAILURE;
  }
  printf("deviceList.Length(): %u\n", deviceList.Length());
  VideoCaptureDevice captureDevice(deviceList[0]);
  nsTArray<RGBVideoFormat> formatList(captureDevice.videoFormatList());
  if (!formatList.Length()) {
    return EXIT_FAILURE;
  }
  printf("formatList.Length(): %u\n", formatList.Length());
  RGBVideoFormat selectedVideoFormat;
  for (PRUint32 i = 0; i < formatList.Length(); ++i) {
    RGBVideoFormat videoFormat(formatList[i]);
    IntegerSize sizePixels(videoFormat.sizePixels());
    printf(
        "videoFormat.sizePixels(): (%i, %i)\n",
        sizePixels.width(),
        sizePixels.height());
    if (sizePixels != kDesiredSizePixels) {
      continue;
    }
    selectedVideoFormat = videoFormat;
  }
  if (!selectedVideoFormat) {
    return EXIT_FAILURE;
  }
  IntegerSize sizePixels(selectedVideoFormat.sizePixels());
  printf(
      "selectedVideoFormat.sizePixels(): (%i, %i)\n",
      sizePixels.width(),
      sizePixels.height());
  if (!captureDevice.setCurrentVideoFormat(selectedVideoFormat)) {
    return EXIT_FAILURE;
  }
  printf("captureDevice setCurrentVideoFormat\n");
  nsAutoPtr<VideoFrameCallback> callback(new VideoFrameCallback);
  if (!captureDevice.addOnNewVideoFrameCallback(callback.get())) {
    return EXIT_FAILURE;
  }
  printf("captureDevice addOnNewVideoFrameCallback\n");
  if (!captureDevice.startCapturing()) {
    return EXIT_FAILURE;
  }
  PRIntervalTime sleepIntervalTime = PR_SecondsToInterval(kSleepTimeSeconds);
  PR_Sleep(sleepIntervalTime);
  printf("captureDevice startCapturing\n");
  if (!captureDevice.stopCapturing()) {
    return EXIT_FAILURE;
  }
  printf("captureDevice stopCapturing\n");
  if (!captureDevice.removeOnNewVideoFrameCallback(callback.get())) {
    return EXIT_FAILURE;
  }
  printf("captureDevice removeOnNewVideoFrameCallback\n");
  return EXIT_SUCCESS;
}
