#pragma once

#include <chrono>
#include <memory>
#include <boost/optional.hpp>
#include <array>
#include "lightStates.hpp"
#include "utils.hpp"

namespace arduinoTimer {

using nanos = std::chrono::nanoseconds;

template <int lightNum> class TimerState {
 public:
  virtual boost::optional<nanos> nextTimerTime(
      const std::array<nanos, lightNum>& nanosForLigtht,
      const boost::optional<nanos>& lastTimerTime, const nanos& deltaTime) = 0;
};

template <int lightNum> class OffTimerState : public TimerState<lightNum> {

  boost::optional<nanos> nextTimerTime(
      const std::array<nanos, lightNum>& nanosForLigtht,
      const boost::optional<nanos>& lastTimerTime, const nanos& deltaTime) {
    return boost::none;
  }
};

template <int lightNum> class LightTimerState : public TimerState<lightNum> {
 protected:
  const int lightIndex;

 public:
  LightTimerState(int index) : lightIndex(index) {}
};

template <int lightNum>
class LightOnTimerState : public LightTimerState<lightNum> {
 public:
  LightOnTimerState(int index) : LightTimerState<lightNum>(index) {}

  boost::optional<nanos> nextTimerTime(
      const std::array<nanos, lightNum>& nanosForLigtht,
      const boost::optional<nanos>& lastTimerTime, const nanos& deltaTime) {
    return lastTimerTime.value_or(nanosForLigtht[this->lightIndex]);
  }

};

template <int lightNum>
class LightFlickerTimerState : public LightTimerState<lightNum> {
 public:
  LightFlickerTimerState(int index) : LightTimerState<lightNum>(index) {}
  boost::optional<nanos> nextTimerTime(
      const std::array<nanos, lightNum>& nanosForLigtht,
      const boost::optional<nanos>& lastTimerTime, const nanos& deltaTime) {
    return utils::transform(lastTimerTime, [deltaTime](const auto & t) {
      return t - deltaTime;
    }).value_or(nanosForLigtht[this->lightIndex]);
  }
};
}