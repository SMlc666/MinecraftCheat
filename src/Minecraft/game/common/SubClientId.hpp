#pragma once
#include "base/mcint.hpp"
enum class SubClientId : uchar {
  PrimaryClient = 0,
  Client2 = 1,
  Client3 = 2,
  Client4 = 3,
  ExtraIdSlotStart = 100,
  EditorUI = 101,
};