#include "COMManager.hpp"
#include "objbase.h"

namespace VideoCapture {

COMManager::COMManager()
    : m_isInitialized(false) {
  m_isInitialized = initialize();
}

COMManager::~COMManager() {
  uninitialize();
}

bool COMManager::initialize() {
  HRESULT result = CoInitializeEx(
      static_cast<void*>(0),
      COINIT_APARTMENTTHREADED);
  if (result == S_OK) {
    return true;
  }
  if (result == S_FALSE) {
    return true;
  }
  return false;
}

void COMManager::uninitialize() {
  if (!isInitialized()) {
    return;
  }
  CoUninitialize();
}

bool COMManager::isInitialized() const {
  return m_isInitialized;
}

} // VideoCapture

