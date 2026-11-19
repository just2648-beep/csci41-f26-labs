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

WordCount::WordCount()
    : slots_(new Slot[1]), capacity_(1), size_(0), tombstones_(0) {}

WordCount::WordCount(int initialCapacity)
    : slots_(new Slot[initialCapacity < 1 ? 1 : initialCapacity]),
      capacity_(initialCapacity < 1 ? 1 : initialCapacity),
      size_(0),
      tombstones_(0) {}

WordCount::WordCount(const WordCount& other)
    : slots_(new Slot[other.capacity_]),
      capacity_(other.capacity_),
      size_(other.size_),
      tombstones_(other.tombstones_) {
    for (int i = 0; i < capacity_; ++i) {
        slots_[i] = other.slots_[i];
    }
}

WordCount& WordCount::operator=(const WordCount& other) {
    if (this == &other) {
        return *this;
    }

    Slot* replacement = new Slot[other.capacity_];

    for (int i = 0; i < other.capacity_; ++i) {
        replacement[i] = other.slots_[i];
    }

    delete[] slots_;

    slots_ = replacement;
    capacity_ = other.capacity_;
    size_ = other.size_;
    tombstones_ = other.tombstones_;

    return *this;
}

WordCount::~WordCount() {
    delete[] slots_;
}

bool WordCount::empty() const {
    return size_ == 0;
}

int WordCount::size() const {
    return size_;
}

int WordCount::capacity() const {
    return capacity_;
}

int WordCount::tombstones() const {
    return tombstones_;
}

double WordCount::loadFactor() const {
    return static_cast<double>(size_ + tombstones_) / capacity_;
}

int WordCount::hashOf(const std::string& word) const {
    unsigned long long hash = 0;

    for (unsigned char ch : word) {
        hash = hash * 31ULL + ch + 1ULL;
    }

    return static_cast<int>(
        hash % static_cast<unsigned long long>(capacity_)
    );
}

int WordCount::findSlot(const std::string& word) const {
    const int home = hashOf(word);

    for (int step = 0; step < capacity_; ++step) {
        const int index = (home + step) % capacity_;
        const Slot& slot = slots_[index];

        if (slot.state == EMPTY) {
            return -1;
        }

        if (slot.state == OCCUPIED && slot.word == word) {
            return index;
        }
    }

    return -1;
}

bool WordCount::contains(const std::string& word) const {
    return findSlot(word) != -1;
}

int WordCount::get(const std::string& word) const {
    const int index = findSlot(word);
    return index == -1 ? 0 : slots_[index].count;
}

void WordCount::put(const std::string& word, int count) {
    if (loadFactor() > 0.5) {
        rehash(2 * capacity_ + 1);
    }

    const int home = hashOf(word);
    int firstTombstone = -1;

    for (int step = 0; step < capacity_; ++step) {
        const int index = (home + step) % capacity_;
        Slot& slot = slots_[index];

        if (slot.state == OCCUPIED) {
            if (slot.word == word) {
                slot.count = count;
                return;
            }
        } else if (slot.state == TOMBSTONE) {
            if (firstTombstone == -1) {
                firstTombstone = index;
            }
        } else {
            const int target =
                firstTombstone == -1 ? index : firstTombstone;

            Slot& destination = slots_[target];

            if (destination.state == TOMBSTONE) {
                --tombstones_;
            }

            destination.state = OCCUPIED;
            destination.word = word;
            destination.count = count;
            ++size_;

            return;
        }
    }

    if (firstTombstone != -1) {
        Slot& destination = slots_[firstTombstone];

        destination.state = OCCUPIED;
        destination.word = word;
        destination.count = count;

        --tombstones_;
        ++size_;

        return;
    }

    rehash(2 * capacity_ + 1);
    put(word, count);
}

void WordCount::increment(const std::string& word) {
    const int index = findSlot(word);

    if (index == -1) {
        put(word, 1);
    } else {
        put(word, slots_[index].count + 1);
    }
}

bool WordCount::erase(const std::string& word) {
    const int index = findSlot(word);

    if (index == -1) {
        return false;
    }

    slots_[index].state = TOMBSTONE;
    slots_[index].word.clear();
    slots_[index].count = 0;

    --size_;
    ++tombstones_;

    return true;
}

void WordCount::clear() {
    for (int i = 0; i < capacity_; ++i) {
        slots_[i] = Slot();
    }

    size_ = 0;
    tombstones_ = 0;
}

std::vector<std::string> WordCount::keys() const {
    std::vector<std::string> result;
    result.reserve(size_);

    for (int i = 0; i < capacity_; ++i) {
        if (slots_[i].state == OCCUPIED) {
            result.push_back(slots_[i].word);
        }
    }

    return result;
}

void WordCount::rehash(int newCapacity) {
    if (newCapacity < 1) {
        newCapacity = 1;
    }

    Slot* oldSlots = slots_;
    const int oldCapacity = capacity_;

    slots_ = new Slot[newCapacity];
    capacity_ = newCapacity;
    size_ = 0;
    tombstones_ = 0;

    for (int i = 0; i < oldCapacity; ++i) {
        if (oldSlots[i].state != OCCUPIED) {
            continue;
        }

        const int home = hashOf(oldSlots[i].word);

        for (int step = 0; step < capacity_; ++step) {
            const int index = (home + step) % capacity_;

            if (slots_[index].state == EMPTY) {
                slots_[index].state = OCCUPIED;
                slots_[index].word = oldSlots[i].word;
                slots_[index].count = oldSlots[i].count;
                ++size_;
                break;
            }
        }
    }

    delete[] oldSlots;
}
