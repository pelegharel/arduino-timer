#pragma once

#include "lightStates.hpp"
#include "timerState.hpp"
#include <boost/optional.hpp>
#include "utils.hpp"
#include <memory>

namespace arduinoTimer {
template <int lightsNum> class Stater {
  OffTimerState offState;

 public:
  TimerState& currState;

  Stater() : currState(offState) {}

  bool shouldResetTimer(const boost::optional<LightState>& prev,
                        const boost::optional<LightState>& curr) {
    return utils::transform(prev, [curr](const auto & prevValue) {
      return utils::transform(curr, [prevValue](const auto & currValue) {
        return prevValue.light != currValue.light;
      }).value_or(true);
    }).value_or(true);
  }

  std::unique_ptr<TimerState> getNextState(
      const boost::optional<LightState>& lightState) {
    return utils::transform(lightState, [](auto x) {
      switch (x.state) {
        case LightStates::flicker:
          return std::unique_ptr<TimerState>(
              new LightOnTimerState<lightsNum>(x.light));
        case LightStates::on:
          return std::unique_ptr<TimerState>(
              new LightFlickerTimerState<lightsNum>(x.light));
      }
    }).value_or(std::unique_ptr<TimerState>(new OffTimerState()));
  }

};
}
