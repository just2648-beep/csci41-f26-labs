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

    result.reserve(static_cast<std::size_t>(count));

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

CircularPlaylist::~CircularPlaylist() {
    clear();
}
