#include "VideoFormatTypes.hpp"

namespace VideoCapture {

static const PRInt32 kBitsPerByte = 8;

static PRInt32 convertBitsToBytes(PRInt32 countBits);

bool isValidRGBFormat(const RGBFormat rgbFormat) {
  switch (rgbFormat) {
    case kRGB888:
      return true;
    case kRGBA8888:
      return true;
    case kBGR888:
      return true;
    case kABGR8888:
      return true;
    case kRGBNone:
      return false;
    default:
      return false;
  }
}

PRInt32 bitsPerPixelForRGBFormat(const RGBFormat rgbFormat) {
  switch (rgbFormat) {
    case kRGB888:
      return 24;
    case kRGBA8888:
      return 32;
    case kBGR888:
      return 24;
    case kABGR8888:
      return 32;
    case kRGBNone:
      return 0;
    default:
      return 0;
  }
}

PRInt32 sizeBytesForRGBFormat(
    const IntegerSize& sizePixels,
    const RGBFormat rgbFormat) {
  if (!sizePixels) {
    return 0;
  }

  PRInt32 bitsPerPixel = bitsPerPixelForRGBFormat(rgbFormat);
  if (!bitsPerPixel) {
    return 0;
  }

  PRInt32 countPixels = sizePixels.width() * sizePixels.height();
  PRInt32 sizeBits = bitsPerPixel * countPixels;

  PRInt32 sizeBytes = convertBitsToBytes(sizeBits);
  return sizeBytes;
}

PRInt32 sizeRowBytesForRGBFormat(
    const IntegerSize& sizePixels,
    const RGBFormat rgbFormat) {
  if (!sizePixels) {
    return 0;
  }

  PRInt32 bitsPerPixel = bitsPerPixelForRGBFormat(rgbFormat);
  if (!bitsPerPixel) {
    return 0;
  }

  PRInt32 sizeRowBits = bitsPerPixel * sizePixels.width();
  PRInt32 sizeRowBytes = convertBitsToBytes(sizeRowBits);
  return sizeRowBytes;
}

static PRInt32 convertBitsToBytes(PRInt32 countBits) {
  if ((countBits % kBitsPerByte) != 0) {
    return 0;
  }

  PRInt32 countBytes = countBits / kBitsPerByte;
  return countBytes;
}

} // VideoCapture
