#pragma once
#include "base/mcint.hpp"
enum class BlockChangedEventTarget : uchar {
  SelfBlock = 0,
  NeighborBlock = 1,
};
