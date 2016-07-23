#include <algorithm>
#include <array>
#include <chrono>
#include <tuple>
#include <boost/optional.hpp>

namespace arduinoTimer {

enum class LightStates {
  on, off, flicker
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
  auto& update(std::chrono::nanoseconds timePassed, const LightsState& state) {
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

  LightState getState() const {}
};

}
