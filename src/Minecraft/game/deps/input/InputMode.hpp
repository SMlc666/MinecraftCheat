#pragma once
#include "base/mcint.hpp"
enum class InputMode : uint {
  Undefined = 0,
  Mouse = 1,
  Touch = 2,
  GamePad = 3,
  MotionController = 4,
  Count = 5,
};
