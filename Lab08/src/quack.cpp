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

void Quack::pushFront(int value) {
    if (size_ == capacity_) {
        grow();
    }

    frontIndex_ = (frontIndex_ - 1 + capacity_) % capacity_;
    data_[frontIndex_] = value;
    ++size_;
}

void Quack::pushBack(int value) {
    if (size_ == capacity_) {
        grow();
    }

    data_[physicalIndex(size_)] = value;
    ++size_;
}

bool Quack::popFront(int& out) {
    if (empty()) {
        return false;
    }

    out = data_[frontIndex_];
    frontIndex_ = (frontIndex_ + 1) % capacity_;
    --size_;
    return true;
}

bool Quack::popBack(int& out) {
    if (empty()) {
        return false;
    }

    out = data_[physicalIndex(size_ - 1)];
    --size_;
    return true;
}

bool Quack::peekFront(int& out) const {
    if (empty()) {
        return false;
    }

    out = data_[frontIndex_];
    return true;
}

bool Quack::peekBack(int& out) const {
    if (empty()) {
        return false;
    }

    out = data_[physicalIndex(size_ - 1)];
    return true;
}

bool Quack::quack(int& frontOut, int& backOut) {
    if (size_ < 2) {
        return false;
    }

    frontOut = data_[frontIndex_];
    backOut = data_[physicalIndex(size_ - 1)];
    frontIndex_ = (frontIndex_ + 1) % capacity_;
    size_ -= 2;
    return true;
}

void Quack::grow() {
    const int newCapacity = capacity_ * 2;
    int* newData = new int[newCapacity];

    for (int i = 0; i < size_; ++i) {
        newData[i] = data_[physicalIndex(i)];
    }

    delete[] data_;
    data_ = newData;
    capacity_ = newCapacity;
    frontIndex_ = 0;
}