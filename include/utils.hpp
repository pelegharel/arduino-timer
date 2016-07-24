#pragma once
#include <boost/optional.hpp>

namespace utils {

template <typename T, typename Func>
auto transform(boost::optional<T> inOption, Func callback)
    ->boost::optional<decltype(callback (*inOption))> {
  if (!inOption) {
    return boost::none;
  }
  return callback(*inOption);
}
}
