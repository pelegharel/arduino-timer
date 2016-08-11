#pragma once

#include <chrono>
#include <tuple>
#include <memory>
#include <boost/optional.hpp>
#include "lightStates.hpp"
#include "utils.hpp"

namespace arduinoTimer {
class TimerState {

 public:
  using nanos = std::chrono::nanoseconds;
  virtual std::tuple<std::unique_ptr<TimerState>, boost::optional<nanos>>
      nextState(const boost::optional<LightState>& light, const nanos& lastTime,
                const nanos& timeDelta) = 0;
};

class OffTimerState : public TimerState {
  std::tuple<std::unique_ptr<TimerState>, boost::optional<nanos>> nextState(
      const boost::optional<LightState>& lightState, const nanos& lastTime,
      const nanos& timeDelta) {

    return utils::transform(lightState, [](const auto & x) {
      return std::make_tuple(
          std::unique_ptr<TimerState>(new OffTimerState()),
          boost::optional<nanos>(std::chrono::nanoseconds(0)));
    })
        .value_or(std::make_tuple(
            std::unique_ptr<TimerState>(new OffTimerState()), boost::none));
  }
};

class LightTimerState : public TimerState {
 protected:
  const int lightIndex;

 public:
  LightTimerState(int index) : lightIndex(index) {}
};

class LightOnTimerState : public LightTimerState {
 public:
  LightOnTimerState(int index) : LightTimerState(index) {}
  std::tuple<std::unique_ptr<TimerState>, boost::optional<nanos>> nextState(
      const boost::optional<LightState>& lightState, const nanos& lastTime,
      const nanos& timeDelta) {

    return utils::transform(lightState, [](const auto & x) {
      return std::make_tuple(
          std::unique_ptr<TimerState>(new OffTimerState()),
          boost::optional<nanos>(std::chrono::nanoseconds(0)));
    })
        .value_or(
            std::make_tuple(std::unique_ptr<TimerState>(this), boost::none));
  }
};

class LightFlickerTimerState : public LightTimerState {
 public:
  LightFlickerTimerState(int index) : LightTimerState(index) {}

  std::tuple<std::unique_ptr<TimerState>, boost::optional<nanos>> nextState(
      const boost::optional<LightState>& lightState, const nanos& lastTime,
      const nanos& timeDelta) {

    return utils::transform(lightState, [](const auto & x) {
      return std::make_tuple(
          std::unique_ptr<TimerState>(new OffTimerState()),
          boost::optional<nanos>(std::chrono::nanoseconds(0)));
    })
        .value_or(
            std::make_tuple(std::unique_ptr<TimerState>(this), boost::none));
  }
};
}