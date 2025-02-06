#pragma once
#include "base/mcint.hpp"
struct PlayerPositionModeComponent {
public:
  enum class PositionMode : uchar {
    Normal = 0,
    Respawn = 1,
    Teleport = 2,
    OnlyHeadRot = 3,
  };

  PositionMode mPositionMode;
};
