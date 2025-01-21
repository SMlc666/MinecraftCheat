#pragma once
#include "API/mem/mem.hpp"
#include "base/mcint.hpp"
#include <cstddef>
class Timer {
public:
  float mTicksPerSecond;
  int mTicks;
  std::byte padding8[16];
  float mLastTimeSeconds;
  int mLastTimestep;
  int64 mLastMs;
  int64 mLastMsSysTime;
  float mTimeScale;
  std::byte padding34[4];
  std::byte padding38[40];
  std::byte padding60[8];

public:
  void setTimeScale(float scale);
  [[nodiscard]] float getTimeScale() const;
};
static_assert(sizeof(Timer) == 0x68);
