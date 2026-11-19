#ifndef TEST_HELPERS_HPP
#define TEST_HELPERS_HPP

#include "AVLRepairInt.hpp"
#include "test_framework.hpp"

#include <optional>
#include <sstream>
#include <string>
#include <vector>

inline std::string intsToString(const std::vector<int>& values) {
    std::ostringstream oss;
    oss << "[";
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i > 0) {
            oss << ", ";
        }
        oss << values[i];
    }
    oss << "]";
    return oss.str();
}

inline std::string optionalsToString(const std::vector<std::optional<int>>& values) {
    std::ostringstream oss;
    oss << "[";
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i > 0) {
            oss << ", ";
        }
        if (values[i].has_value()) {
            oss << *values[i];
        } else {
            oss << "#";
        }
    }
    oss << "]";
    return oss.str();
}

inline void expectIntVectorEq(const std::vector<int>& actual,
                              const std::vector<int>& expected,
                              const std::string& message) {
    if (!(actual == expected)) {
        failTest(message + " actual=" + intsToString(actual) + " expected=" + intsToString(expected));
    }
}

inline void expectOptionalVectorEq(const std::vector<std::optional<int>>& actual,
                                   const std::vector<std::optional<int>>& expected,
                                   const std::string& message) {
    if (!(actual == expected)) {
        failTest(message + " actual=" + optionalsToString(actual) + " expected=" + optionalsToString(expected));
    }
}

inline void expectCaseEq(RotationCase actual, RotationCase expected, const std::string& message) {
    if (actual != expected) {
        failTest(message + " actual=" + rotationCaseToString(actual) + " expected=" + rotationCaseToString(expected));
    }
}

#endif
