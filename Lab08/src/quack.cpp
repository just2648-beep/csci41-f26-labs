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

bool Quack::empty() const {
    return size_ == 0;
}

int Quack::size() const {
    return size_;
}

int Quack::capacity() const {
    return capacity_;
}

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

void Quack::clear() {
    size_ = 0;
    frontIndex_ = 0;
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
