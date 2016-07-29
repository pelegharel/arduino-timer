#include "utils.hpp"
#include "states.hpp"
#include <chrono>
#include <iostream>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include "utils.hpp"
#include "checkChrono.hpp"

using namespace std::chrono;
using namespace arduinoTimer;
using namespace std;

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

  auto rrr = states.getState(1s, 2h);
  if (rrr) {
    cout << rrr->light << endl;
  } else {
    cout << "nones" << endl;
  }

  //  auto s = states.getState<seconds, 2>();

  // if (s) {
  //   std::cout << s->light << std::endl;
  // } else {
  //   std::cout << "none" << std::endl;
  // }
  return 0;
}