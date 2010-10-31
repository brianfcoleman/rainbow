#ifndef VIDEO_CAPTURE_VIDEO_FORMAT_TYPES
#define VIDEO_CAPTURE_VIDEO_FORMAT_TYPES

#include "IntegerSize.hpp"

namespace VideoCapture {
typedef enum RGBFormat {
  /**
   * Pixel interleaved format with each colour component stored in 8 bits.
   * The colour component order when iterating forward through the pixels
   * is R G B.
   * No alpha channel.
   */
  kRGB888 = 0,
  /**
   * Pixel interleaved format with each colour component stored in 8 bits.
   * The colour component order when iterating forward through the pixels
   * is R G B A.
   * Includes an alpha channel.
   */
  kRGBA8888 = 1,
  /**
   * Pixel interleaved format with each colour component stored in 8 bits.
   * The colour component order when iterating forward through the pixels
   * is B G R.
   * No alpha channel.
   */
  kBGR888 = 2,
  /**
   * Pixel interleaved format with each colour component stored in 8 bits.
   * The colour component order when iterating forward through the pixels
   * is A B G R.
   * Includes an alpha channel.
   */
  kABGR8888 = 3,
  kRGBNone = 4
} RGBFormat;

const PRInt32 kAngleFullRotationDegrees = 360;
const PRInt32 kAngleHalfRotationDegrees = 180;

bool isValidRGBFormat(const RGBFormat rgbFormat);
PRInt32 bitsPerPixelForRGBFormat(const RGBFormat rgbFormat);
PRInt32 sizeBytesForRGBFormat(
    const IntegerSize& sizePixels,
    const RGBFormat rgbFormat);
PRInt32 sizeRowBytesForRGBFormat(
    const IntegerSize& sizePixels,
    const RGBFormat rgbFormat);

} // VideoCapture

#endif // VIDEO_CAPTURE_VIDEO_FORMAT_TYPES
