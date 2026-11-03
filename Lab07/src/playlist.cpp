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

#include "playlist.h"

CircularPlaylist::CircularPlaylist()
    : current_(nullptr), size_(0) {}

CircularPlaylist::CircularPlaylist(const CircularPlaylist& other)
    : current_(nullptr), size_(0) {
    copyFrom(other);
}

CircularPlaylist& CircularPlaylist::operator=(const CircularPlaylist& other) {
    if (this != &other) {
        // Build the copy first so an allocation failure leaves this playlist unchanged.
        CircularPlaylist copy(other);

        clear();
        current_ = copy.current_;
        size_ = copy.size_;

        copy.current_ = nullptr;
        copy.size_ = 0;
    }

    return *this;
}

CircularPlaylist::~CircularPlaylist() {
    clear();
}

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
    if (empty()) {
        return "";
    }

    return current_->data;
}

bool CircularPlaylist::contains(const std::string& title) const {
    if (empty()) {
        return false;
    }

    Node* node = current_;
    do {
        if (node->data == title) {
            return true;
        }

        node = node->next;
    } while (node != current_);

    return false;
}

std::vector<std::string> CircularPlaylist::snapshotForward(int count) const {
    std::vector<std::string> result;

    if (empty() || count <= 0) {
        return result;
    }

    Node* node = current_;

    for (int i = 0; i < count; ++i) {
        result.push_back(node->data);
        node = node->next;
    }

    return result;
}

void CircularPlaylist::clear() {
    Node* node = current_;
    int remaining = size_;

    while (remaining > 0) {
        Node* nextNode = node->next;
        delete node;
        node = nextNode;
        --remaining;
    }

    current_ = nullptr;
    size_ = 0;
}

void CircularPlaylist::copyFrom(const CircularPlaylist& other) {
    if (other.empty()) {
        return;
    }

    Node* source = other.current_;

    try {
        do {
            Node* node = new Node{source->data, nullptr, nullptr};

            if (empty()) {
                node->prev = node;
                node->next = node;
                current_ = node;
            } else {
                Node* tail = current_->prev;

                node->prev = tail;
                node->next = current_;
                tail->next = node;
                current_->prev = node;
            }

            ++size_;
            source = source->next;
        } while (source != other.current_);
    } catch (...) {
        clear();
        throw;
    }
}