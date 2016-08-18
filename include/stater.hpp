#pragma once

#include "timerState.hpp"

namespace arduinoTimer {
template <int lightsNum> class Stater {
  OffTimerState<lightsNum> offState;

 public:
  TimerState<lightsNum>& currState;

  Stater() : currState(offState) {}

};
}
