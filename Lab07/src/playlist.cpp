// CSCI 41 -- Lab 07: Linked Lists
//
// Implement every function below. Read include/playlist.h first: it states
// exactly what each one must do, including what happens on an empty
// playlist and on a playlist of one.
//
// Three rules that will save you an evening:
//
//   1. Test the EMPTY playlist and the playlist of ONE first. Almost every
//      circular-list bug lives in those two cases.
//   2. A ring has no nullptr, so you cannot stop a walk by looking for one.
//      Use do/while and stop when you are back at the node you started on.
//   3. Before you delete the node the cursor is on, move the cursor. A
//      cursor pointing at freed memory is the bug this lab is built to
//      teach you.
//
// Build it with sanitizers from the very first function:
//
//   g++ -std=c++17 -g -Wall -Wextra -fsanitize=address -Iinclude
//       src/playlist.cpp tests/test_lab07.cpp -o t && ./t
//
// That is how it is graded.
#include "playlist.h"

#include <stdexcept>

CircularPlaylist::CircularPlaylist() : current_(nullptr), size_(0) {
    // TODO: an empty playlist. This one may already be correct -- read the
    // header and decide.
}

CircularPlaylist::CircularPlaylist(const CircularPlaylist& other)
    : current_(nullptr), size_(0) {
    (void)other;
    throw std::logic_error("not implemented: copy constructor");
}

CircularPlaylist& CircularPlaylist::operator=(const CircularPlaylist& other) {
    (void)other;
    throw std::logic_error("not implemented: operator=");
}

CircularPlaylist::~CircularPlaylist() {
    // TODO: give every node back. Remember to cut the ring open first, or
    // you will come back round to a node you already deleted.
    //
    // Calling clear() here is reasonable -- but only once clear() works.
}

bool CircularPlaylist::empty() const {
    throw std::logic_error("not implemented: empty");
}

int CircularPlaylist::size() const {
    throw std::logic_error("not implemented: size");
}

void CircularPlaylist::addAfterCurrent(const std::string& title) {
    (void)title;
    throw std::logic_error("not implemented: addAfterCurrent");
}

void CircularPlaylist::addBeforeCurrent(const std::string& title) {
    (void)title;
    throw std::logic_error("not implemented: addBeforeCurrent");
}

bool CircularPlaylist::removeCurrent() {
    throw std::logic_error("not implemented: removeCurrent");
}

bool CircularPlaylist::next() {
    throw std::logic_error("not implemented: next");
}

bool CircularPlaylist::prev() {
    throw std::logic_error("not implemented: prev");
}

std::string CircularPlaylist::current() const {
    throw std::logic_error("not implemented: current");
}

bool CircularPlaylist::contains(const std::string& title) const {
    (void)title;
    throw std::logic_error("not implemented: contains");
}

std::vector<std::string> CircularPlaylist::snapshotForward(int count) const {
    (void)count;
    throw std::logic_error("not implemented: snapshotForward");
}

void CircularPlaylist::clear() {
    throw std::logic_error("not implemented: clear");
}

void CircularPlaylist::copyFrom(const CircularPlaylist& other) {
    (void)other;
    throw std::logic_error("not implemented: copyFrom");
}
