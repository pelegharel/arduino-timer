#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <tuple>
#include <boost/optional.hpp>
#include "checkChrono.hpp"
#include "utils.hpp"

namespace arduinoTimer {

enum class LightStates {
  on, flicker
};

struct LightState {
  int light;
  LightStates state;
};

template <int lightNum, int stateNum> class LightsStates {
  using LightsState = std::array<bool, lightNum>;
  using TimedState = std::tuple<std::chrono::nanoseconds, LightsState>;
  std::array<boost::optional<TimedState>, stateNum> states;

 public:
  auto& update(const std::chrono::nanoseconds& timePassed,
               const LightsState& state) {
    auto& lastState = std::get<0>(states);

    if (!lastState || std::get<1>(*lastState) != state) {
      std::rotate(states.begin(), states.end() - 1, states.end());
      std::get<0>(states) = std::make_tuple(timePassed, state);
    } else {
      auto& lastTime = std::get<0>(*lastState);
      lastTime = timePassed + lastTime;
    }

    return *this;
  }

  auto begin() const { return states.cbegin(); }

  auto end() const {
    return std::find_if(states.cbegin(), states.cend(), [](auto a) {
      return a == boost::none;
    });
  }

  boost::optional<int> getLitLight(const TimedState& state) const {

    auto lastValue = std::get<1>(state);

    auto pos = std::find_if(lastValue.begin(), lastValue.end(), [](auto x) {
      return x;
    });

    if (pos == lastValue.end()) {
      return boost::none;
    }

    return pos - lastValue.begin();
  }

  template <typename DurationUnitMin, typename DurationUnitMax>
  boost::optional<LightState> getState(DurationUnitMin minFlickerTime,
                                       DurationUnitMax maxFlickerTime) const {

    static_assert(
        argumentChecks::is_chrono_duration<DurationUnitMin>::value &&
            argumentChecks::is_chrono_duration<DurationUnitMax>::value,
        "DurationUnit must be std::chrono::duration");

    std::array<boost::optional<int>, stateNum> litLights;
    std::transform(states.begin(), states.end(), litLights.begin(),
                   [this](const auto & stateOption) {
      auto res = utils::transform(stateOption, [this](const auto & state) {
        return this->getLitLight(state);
      });
      boost::optional<int> ret = res.value_or(boost::none);
      return ret;
    });

    bool isUnderFlickerTime =
        std::all_of(states.begin(), states.begin() + 2,
                    [minFlickerTime, maxFlickerTime](const auto & x) {
      return utils::transform(x,
                              [minFlickerTime, maxFlickerTime](const auto & x) {
        auto t = std::get<0>(x);
        return minFlickerTime <= t && t <= maxFlickerTime;
      }).value_or(false);
    });

    if (isUnderFlickerTime) {
      auto last0 = std::get<0>(litLights);
      auto last1 = std::get<1>(litLights);
      auto last2 = std::get<2>(litLights);

      if (last0 == last2 && last0 != last1) {
        if (last0 == boost::none) {
          return LightState { *last1, LightStates::flicker };
        } else if (last1 == boost::none) {
          return LightState { *last0, LightStates::flicker };
        }
      }
    }

    auto lastLit = std::get<0>(litLights);

    return utils::transform(lastLit, [](auto x) {
      return LightState { x, LightStates::on };
    });
  }
};

}
