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
 * The Original Code is Mozilla Labs Rainbow.
 *
 * The Initial Developer of the Original Code is
 * Brian Coleman.
 * Portions created by the Initial Developer are Copyright (C) 2010
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
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
