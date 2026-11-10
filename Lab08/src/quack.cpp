// CSCI 41 -- Lab 08: Stacks and Queues
//
// Implement every function below. Read include/quack.h first: it states the
// representation and exactly what each function must do.
//
// FOUR RULES THAT WILL SAVE YOU AN EVENING
//
//   1. Write physicalIndex() and toVector() FIRST. Everything else becomes
//      readable once you have them, and toVector() is how you will debug.
//
//   2. Test the EMPTY Quack and the Quack of ONE before anything else.
//
//   3. Watch the modulo on pushFront. Going backwards from index 0 needs
//      (frontIndex_ - 1 + capacity_) % capacity_, because in C++ a negative
//      operand to % does not wrap the way you want.
//
//   4. grow() is the hard one, and it is hard in exactly one way: when the
//      contents are WRAPPED they are in two pieces. Copy by LOGICAL
//      position, not by physical index, and set frontIndex_ to 0 afterwards.
//
// Build it with sanitizers from the very first function:
//
//   g++ -std=c++17 -g -Wall -Wextra -fsanitize=address -Iinclude
//       src/quack.cpp tests/test_lab08.cpp -o t && ./t
//
// That is how it is graded.
#include "quack.h"

#include <stdexcept>

#include "quack.h"

Quack::Quack() : Quack(4) {}

Quack::Quack(int initialCapacity)
    : data_(new int[initialCapacity]),
      capacity_(initialCapacity),
      size_(0),
      frontIndex_(0) {}

bool Quack::empty() const {
    return size_ == 0;
}

int Quack::size() const {
    return size_;
}

int Quack::capacity() const {
    return capacity_;
}

std::vector<int> Quack::toVector() const {
    std::vector<int> result;
    result.reserve(size_);

    for (int i = 0; i < size_; ++i) {
        result.push_back(data_[physicalIndex(i)]);
    }

    return result;
}

int Quack::physicalIndex(int logicalOffset) const {
    return (frontIndex_ + logicalOffset) % capacity_;
}
