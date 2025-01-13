#include "OwnerStorageSharePtr.hpp"

template <typename T> inline bool OwnerStorageSharePtr<T>::_hasValue() const {
  return mValue != nullptr;
}

template <typename T> void OwnerStorageSharePtr<T>::_reset() {
  mValue.reset();
}