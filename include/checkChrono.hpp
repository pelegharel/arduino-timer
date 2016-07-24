#pragma once

#include <chrono>

namespace argumentChecks {

template <typename T> struct is_chrono_duration {
  static constexpr bool value = false;
};

template <typename Rep, typename Period>
struct is_chrono_duration<std::chrono::duration<Rep, Period> > {
  static constexpr bool value = true;
};

}