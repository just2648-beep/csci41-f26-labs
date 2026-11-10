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

Quack::Quack(const Quack& other)
    : data_(new int[other.capacity_]),
      capacity_(other.capacity_),
      size_(other.size_),
      frontIndex_(0) {
    for (int i = 0; i < size_; ++i) {
        data_[i] = other.data_[other.physicalIndex(i)];
    }
}

Quack& Quack::operator=(const Quack& other) {
    if (this == &other) {
        return *this;
    }

    int* newData = new int[other.capacity_];

    for (int i = 0; i < other.size_; ++i) {
        newData[i] = other.data_[other.physicalIndex(i)];
    }

    delete[] data_;
    data_ = newData;
    capacity_ = other.capacity_;
    size_ = other.size_;
    frontIndex_ = 0;

    return *this;
}

Quack::~Quack() {
    delete[] data_;
}

void Quack::clear() {
    size_ = 0;
    frontIndex_ = 0;
}