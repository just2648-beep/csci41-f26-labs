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

CircularPlaylist::CircularPlaylist() : current_(nullptr), size_(0) {}

bool CircularPlaylist::empty() const {
    return size_ == 0;
}

int CircularPlaylist::size() const {
    return size_;
}

void CircularPlaylist::addAfterCurrent(const std::string& title) {
    Node* node = new Node{title, nullptr, nullptr};

    if (empty()) {
        node->prev = node;
        node->next = node;
        current_ = node;
    } else {
        Node* after = current_->next;
        node->prev = current_;
        node->next = after;
        current_->next = node;
        after->prev = node;
    }

    ++size_;
}

void CircularPlaylist::addBeforeCurrent(const std::string& title) {
    Node* node = new Node{title, nullptr, nullptr};

    if (empty()) {
        node->prev = node;
        node->next = node;
        current_ = node;
    } else {
        Node* before = current_->prev;
        node->prev = before;
        node->next = current_;
        before->next = node;
        current_->prev = node;
    }

    ++size_;
}

bool CircularPlaylist::removeCurrent() {
    if (empty()) {
        return false;
    }

    Node* removed = current_;

    if (size_ == 1) {
        current_ = nullptr;
    } else {
        current_ = removed->next;
        removed->prev->next = removed->next;
        removed->next->prev = removed->prev;
    }

    delete removed;
    --size_;
    return true;
}

bool CircularPlaylist::next() {
    if (empty()) {
        return false;
    }

    current_ = current_->next;
    return true;
}

bool CircularPlaylist::prev() {
    if (empty()) {
        return false;
    }

    current_ = current_->prev;
    return true;
}

std::string CircularPlaylist::current() const {
    return empty() ? "" : current_->data;
}
