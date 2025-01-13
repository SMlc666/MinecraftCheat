#pragma once
#include "OwnerStorageSharePtr.hpp"

template <typename T> struct SharePtrRefTraits {
  typedef T StackRef;
  typedef OwnerStorageSharePtr<T> OwnerStorage;
  typedef SharePtrRefTraits<T>::StackRef OwnerStackRef;
};