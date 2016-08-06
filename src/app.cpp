#include "utils.hpp"
#include "states.hpp"
#include <chrono>
#include <iostream>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include "utils.hpp"

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

  auto rrr = states.getState(500ms, 2h);
  cout << transform(rrr, [](const auto & x) {
    return x.light;
  }) << endl;

  return 0;
}