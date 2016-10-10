#ifndef CAFCA_OPTIONAL_H
#define CAFCA_OPTIONAL_H

#include <boost/optional.hpp>
namespace cafca {
namespace util {

template<typename T, typename F>
auto bind(const boost::optional<T> &m, const F &f) -> decltype(f(*m)) {
  return m ? f(*m) : boost::none;
}

template<typename T, typename F>
auto map(const boost::optional<T> &m,
         const F &f) -> const boost::optional<decltype(f(*m))> {
  return m ? boost::make_optional(f(*m)) : boost::none;
}

}
}

#endif //CAFCA_OPTIONAL_H
