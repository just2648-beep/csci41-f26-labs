#ifndef TEST_FRAMEWORK_HPP
#define TEST_FRAMEWORK_HPP

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <string>

inline void failTest(const std::string& message) {
    throw std::runtime_error(message);
}

inline void expectTrue(bool condition, const std::string& message) {
    if (!condition) {
        failTest(message);
    }
}

inline void expectFalse(bool condition, const std::string& message) {
    if (condition) {
        failTest(message);
    }
}

template <typename T>
inline void expectEq(const T& actual, const T& expected, const std::string& message) {
    if (!(actual == expected)) {
        std::ostringstream oss;
        oss << message;
        failTest(oss.str());
    }
}

#endif
