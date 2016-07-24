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
  states.update(1s, {
    false, true, false
  });
  states.update(1s, {
    false, false, false
  });

  boost::optional<int> a = 1;
  a = boost::none;
  auto b = utils::transform(a, [](auto a) {
    return "pozac";
  });

  std::cout << b << std::endl;
  auto rrr = states.getState<seconds, 1>();
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