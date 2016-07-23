#include "states.hpp"
#include <chrono>
#include <iostream>

using namespace std::chrono;
using namespace std;

int main(int argc, char** argv) {

    LightsStates<3,10> states;
    states.update(0s, {false,false,true});
    states.update(1s, {false,false,true});
    states.update(2s, {false,false,true});
    states.update(3s, {false,false,true});
    states.update(4s, {false,false,false});
    
    for(const auto& x:states) {
      auto timeStamp = duration_cast<seconds>(std::get<0>(*x));
      cout << timeStamp.count() << endl;
    }

    return 0;
}