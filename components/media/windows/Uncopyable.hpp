#ifndef VIDEO_CAPTURE_UNCOPYABLE_H
#define VIDEO_CAPTURE_UNCOPYABLE_H

namespace VideoCapture {

class Uncopyable {
 public:
  Uncopyable();
  virtual ~Uncopyable();
 private:
  Uncopyable(const Uncopyable& uncopyable);
  Uncopyable& operator=(const Uncopyable& uncopyable) {

  }
};

} // VideoCapture

#endif // VIDEO_CAPTURE_UNCOPYABLE_H
