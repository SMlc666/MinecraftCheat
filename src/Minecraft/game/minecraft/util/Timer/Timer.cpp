#include "Timer.hpp"
#include <cmath>
void Timer::setTimeScale(float Scale) {
  this->mTimeScale = fminf(fmaxf(Scale, 0.0F), 10.0F);
}
float Timer::getTimeScale() const {
  return this->mTimeScale;
}