#pragma once

#include <chrono>
#include <boost/optional.hpp>
#include <array>
#include "lightStates.hpp"
#include "utils.hpp"

namespace arduinoTimer {

using nanos = std::chrono::nanoseconds;

class TimerState {

 public:
  virtual boost::optional<nanos> startTimerTime() = 0;

  virtual boost::optional<nanos> updateTimerTime(
      const boost::optional<nanos>& lastTimerTime, const nanos& deltaTime) = 0;
};

class OffTimerState : public TimerState {
 public:
  boost::optional<nanos> startTimerTime() { return boost::none; }
  boost::optional<nanos> updateTimerTime(
      const boost::optional<nanos>& lastTimerTime, const nanos& deltaTime) {
    return boost::none;
  }
};

template <int lightNum> class LightTimerState : public TimerState {
 protected:
  const int lightIndex;
  const std::array<nanos, lightNum> nanosForLigtht;

 public:
  boost::optional<nanos> startTimerTime() { return nanosForLigtht[lightIndex]; }

  LightTimerState(int lightIndex,
                  const std::array<nanos, lightNum>& nanosForLigtht)
      : nanosForLigtht(nanosForLigtht), lightIndex(lightIndex) {}
};

template <int lightNum>
class LightOnTimerState : public LightTimerState<lightNum> {
 public:
  LightOnTimerState(int index,
                    const std::array<nanos, lightNum>& nanosForLigtht)
      : LightTimerState<lightNum>(index, nanosForLigtht) {}

  boost::optional<nanos> updateTimerTime(
      const boost::optional<nanos>& lastTimerTime, const nanos& deltaTime) {
    return lastTimerTime.value_or(this->nanosForLigtht[this->lightIndex]);
  }

};

template <int lightNum>
class LightFlickerTimerState : public LightTimerState<lightNum> {
 public:
  LightFlickerTimerState(int index,
                         const std::array<nanos, lightNum>& nanosForLigtht)
      : LightTimerState<lightNum>(index, nanosForLigtht) {}
  boost::optional<nanos> updateTimerTime(
      const boost::optional<nanos>& lastTimerTime, const nanos& deltaTime) {
    return utils::transform(lastTimerTime, [deltaTime](const auto & t) {
      return t - deltaTime;
    }).value_or(this->nanosForLigtht[this->lightIndex]);
  }
};
}