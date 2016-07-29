#pragma once
#include <boost/optional.hpp>

namespace utils {

template <typename T, typename Func>
auto transform(const boost::optional<T>& inOption, const Func& callback)
    ->boost::optional<decltype(callback (*inOption))> {
  if (!inOption) {
    return boost::none;
  }
  return callback(*inOption);
}
}
