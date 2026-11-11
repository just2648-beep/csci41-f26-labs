// CSCI 41 -- Lab 09: Hash Tables
//
// Implement every function below. Read include/wordcount.h first: it states
// the representation and exactly what each function must do.
//
// FIVE RULES THAT WILL SAVE AN EVENING
//
//   1. Write hashOf() and findSlot() FIRST. Everything else is written in
//      terms of them, and a bad hash makes every later bug harder to see.
//
//   2. hashOf must involve EVERY character. A hash that uses only the first
//      character puts every word beginning with 's' in one probe sequence.
//      Day 28's 02_distribution demonstrates what that costs.
//
//   3. A search may stop ONLY at EMPTY. Stopping at a tombstone hides every
//      word stored further along that probe sequence -- Day 29, Part Four.
//
//   4. put() may reuse the first tombstone it passes, but only after
//      continuing to an EMPTY slot to prove the word is not already there.
//      Reusing it early inserts a duplicate key.
//
//   5. The load factor counts tombstones, and put() decides whether to
//      grow by CALLING loadFactor(). Leave tombstones out of it and a
//      table that is repeatedly filled and emptied never grows: every
//      slot in it eventually becomes a tombstone, and then an insertion
//      finds no empty slot to use and silently stores nothing.
//
// Day 29 developed these ideas for a SET OF INTS. This is a MAP WITH STRING
// KEYS: slots carry a key and a value, the hash is over characters, and the
// interface differs. The reasoning transfers; the code does not.
//
// Build with sanitizers from the first function:
//
//   g++ -std=c++17 -g -Wall -Wextra -fsanitize=address -Iinclude
//       src/wordcount.cpp tests/test_lab09.cpp -o t && ./t
//
// That is how it is graded.
#include "wordcount.h"

#include <stdexcept>

WordCount::WordCount()
    : slots_(nullptr), capacity_(0), size_(0), tombstones_(0) {
    throw std::logic_error("not implemented: WordCount()");
}

WordCount::WordCount(int initialCapacity)
    : slots_(nullptr), capacity_(0), size_(0), tombstones_(0) {
    (void)initialCapacity;
    throw std::logic_error("not implemented: WordCount(int)");
}

WordCount::WordCount(const WordCount& other)
    : slots_(nullptr), capacity_(0), size_(0), tombstones_(0) {
    (void)other;
    throw std::logic_error("not implemented: copy constructor");
}

WordCount& WordCount::operator=(const WordCount& other) {
    (void)other;
    throw std::logic_error("not implemented: operator=");
}

WordCount::~WordCount() {
    // TODO: give the slot array back.
}

bool WordCount::empty() const {
    throw std::logic_error("not implemented: empty");
}

int WordCount::size() const {
    throw std::logic_error("not implemented: size");
}

int WordCount::capacity() const {
    throw std::logic_error("not implemented: capacity");
}

int WordCount::tombstones() const {
    throw std::logic_error("not implemented: tombstones");
}

double WordCount::loadFactor() const {
    throw std::logic_error("not implemented: loadFactor");
}

int WordCount::hashOf(const std::string& word) const {
    (void)word;
    throw std::logic_error("not implemented: hashOf");
}

int WordCount::findSlot(const std::string& word) const {
    (void)word;
    throw std::logic_error("not implemented: findSlot");
}

bool WordCount::contains(const std::string& word) const {
    (void)word;
    throw std::logic_error("not implemented: contains");
}

int WordCount::get(const std::string& word) const {
    (void)word;
    throw std::logic_error("not implemented: get");
}

void WordCount::put(const std::string& word, int count) {
    (void)word; (void)count;
    throw std::logic_error("not implemented: put");
}

void WordCount::increment(const std::string& word) {
    (void)word;
    throw std::logic_error("not implemented: increment");
}

bool WordCount::erase(const std::string& word) {
    (void)word;
    throw std::logic_error("not implemented: erase");
}

void WordCount::clear() {
    throw std::logic_error("not implemented: clear");
}

std::vector<std::string> WordCount::keys() const {
    throw std::logic_error("not implemented: keys");
}

void WordCount::rehash(int newCapacity) {
    (void)newCapacity;
    throw std::logic_error("not implemented: rehash");
}
