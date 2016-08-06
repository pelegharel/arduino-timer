#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <tuple>
#include <boost/optional.hpp>
#include <type_traits>
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

    if (!utils::transform(lastState, [state](const auto & x) {
      return std::get<1>(x) == state;
    }).value_or(false)) {
      std::rotate(states.begin(), states.end() - 1, states.end());
      std::get<0>(states) = std::make_tuple(timePassed, state);
    } else {
      std::get<0>(*lastState) += timePassed;
    }

    return *this;
  }

  auto begin() const { return states.cbegin(); }

  auto end() const {
    return std::find_if(states.cbegin(), states.cend(), [](auto a) {
      return a == boost::none;
    });
  }

  static boost::optional<int> getLitLight(const TimedState& state) {

    auto stateValue = std::get<1>(state);

    auto pos = std::find_if(stateValue.begin(), stateValue.end(), [](auto x) {
      return x;
    });

    if (pos == stateValue.end()) {
      return boost::none;
    }

    return pos - stateValue.begin();
  }

  /**
  * Returns lit light of each state.
  * If a state is none are there are no lit lights,
  * boost::none is returned.
  * Otherwise, index of lit light is returned.
  */
  std::array<boost::optional<int>, stateNum> getLitLights() const {

    decltype(getLitLights()) litLights;
    std::transform(states.begin(), states.end(), litLights.begin(),
                   [this](const auto & stateOption)->boost::optional<int> {
      return utils::transform(stateOption, [this](const auto & state) {
        return this->getLitLight(state);
      }).value_or(boost::none);
    });

    return litLights;
  }

  template <typename DurationUnitMin, typename DurationUnitMax>
  boost::optional<LightState> getState(DurationUnitMin minFlickerTime,
                                       DurationUnitMax maxFlickerTime) const {
    static_assert(
        std::is_convertible<DurationUnitMin, std::chrono::nanoseconds>::value &&
            std::is_convertible<DurationUnitMax,
                                std::chrono::nanoseconds>::value,
        "DurationUnit must be convertable to std::chrono::nanoseconds");

    bool isUnderFlickerTime =
        std::all_of(states.begin(), states.begin() + 2,
                    [minFlickerTime, maxFlickerTime](const auto & x) {
      return utils::transform(x,
                              [minFlickerTime, maxFlickerTime](const auto & x) {
        auto t = std::get<0>(x);
        return minFlickerTime <= t && t <= maxFlickerTime;
      }).value_or(false);
    });

    auto litLights = getLitLights();

    if (isUnderFlickerTime) {
      auto last0 = std::get<0>(litLights);
      auto last1 = std::get<1>(litLights);
      auto last2 = std::get<2>(litLights);

      if (last0 == last2) {
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
