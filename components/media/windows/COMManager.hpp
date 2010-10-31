#ifndef VIDEO_CAPTURE_COM_MANAGER_H
#define VIDEO_CAPTURE_COM_MANAGER_H

namespace VideoCapture {

class COMManager {
 public:
  COMManager();
  ~COMManager();
  bool isInitialized() const;
  operator bool() const {
    return isInitialized();
  }
 private:
  bool initialize();
  void uninitialize();
  bool m_isInitialized;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_COM_MANAGER_H
