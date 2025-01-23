#pragma once
#include "OwnerStorageSharePtr.hpp"

template <typename T> struct SharePtrRefTraits {
  using StackRef = T;
  using OwnerStorage = OwnerStorageSharePtr<T>;
  using OwnerStackRef = SharePtrRefTraits<T>::StackRef;
};