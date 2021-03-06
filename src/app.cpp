#include "utils.hpp"
#include "lightStates.hpp"
#include <chrono>
#include <iostream>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include "utils.hpp"
#include "timerState.hpp"
#include "stater.hpp"

using namespace std::chrono;
using namespace arduinoTimer;
using namespace std;
using namespace utils;

int main(int argc, char** argv) {
  LightsStates<3, 10> states;
  states.update(1s, {
    false, false, false
  });
  states.update(500ms, {
    false, true, false
  });
  states.update(400ms, {
    false, true, false
  });
  states.update(1s, {
    false, false, false
  });
  states.update(1h, {
    true, false, false
  });
  states.update(1h, {
    false, false, true
  });
  states.update(1h, {
    false, false, false
  });
  states.update(1h, {
    false, false, true
  });

  for (const auto& x : states.getLitLights()) {
    cout << x << endl;
  }

  LightOnTimerState<3> a(2, {
    120min, 100min, 50min
  });
  return 0;
}