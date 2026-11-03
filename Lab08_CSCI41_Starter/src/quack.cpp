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

Quack::Quack() : data_(nullptr), capacity_(0), size_(0), frontIndex_(0) {
    // TODO: start with a small buffer. capacity_ must be at least 1.
    throw std::logic_error("not implemented: Quack()");
}

Quack::Quack(int initialCapacity)
    : data_(nullptr), capacity_(0), size_(0), frontIndex_(0) {
    (void)initialCapacity;
    throw std::logic_error("not implemented: Quack(int)");
}

Quack::Quack(const Quack& other)
    : data_(nullptr), capacity_(0), size_(0), frontIndex_(0) {
    (void)other;
    throw std::logic_error("not implemented: copy constructor");
}

Quack& Quack::operator=(const Quack& other) {
    (void)other;
    throw std::logic_error("not implemented: operator=");
}

Quack::~Quack() {
    // TODO: give the buffer back.
}

bool Quack::empty() const {
    throw std::logic_error("not implemented: empty");
}

int Quack::size() const {
    throw std::logic_error("not implemented: size");
}

int Quack::capacity() const {
    throw std::logic_error("not implemented: capacity");
}

void Quack::pushFront(int value) {
    (void)value;
    throw std::logic_error("not implemented: pushFront");
}

void Quack::pushBack(int value) {
    (void)value;
    throw std::logic_error("not implemented: pushBack");
}

bool Quack::popFront(int& out) {
    (void)out;
    throw std::logic_error("not implemented: popFront");
}

bool Quack::popBack(int& out) {
    (void)out;
    throw std::logic_error("not implemented: popBack");
}

bool Quack::peekFront(int& out) const {
    (void)out;
    throw std::logic_error("not implemented: peekFront");
}

bool Quack::peekBack(int& out) const {
    (void)out;
    throw std::logic_error("not implemented: peekBack");
}

bool Quack::quack(int& frontOut, int& backOut) {
    (void)frontOut; (void)backOut;
    throw std::logic_error("not implemented: quack");
}

void Quack::clear() {
    throw std::logic_error("not implemented: clear");
}

std::vector<int> Quack::toVector() const {
    throw std::logic_error("not implemented: toVector");
}

int Quack::physicalIndex(int logicalOffset) const {
    (void)logicalOffset;
    throw std::logic_error("not implemented: physicalIndex");
}

void Quack::grow() {
    throw std::logic_error("not implemented: grow");
}
