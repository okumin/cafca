#ifndef CAFCA_LOGGING_H
#define CAFCA_LOGGING_H

#include <iostream>

namespace cafca {
namespace util {

template<typename A>
inline void debug(const A &message) {
  std::cout << message << "\n";
}

template<typename A, typename B>
inline void debug(const A &message1, const B &message2) {
  std::cout << message1 << message2 << "\n";
}

template<typename A, typename B, typename C, typename D>
inline void debug(const A &message1, const B &message2, const C &message3, const D &message4) {
  std::cout << message1 << message2 << message3 << message4 << "\n";
}

}
}

#endif //CAFCA_LOGGING_H
