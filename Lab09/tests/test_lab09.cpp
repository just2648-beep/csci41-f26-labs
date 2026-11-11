// Lab 09 PUBLIC self-check.
//
// NOT the grade. The grade comes from a larger private set built the same
// way against src/wordcount.cpp. But every KIND of case the private set
// checks appears here at least once -- the empty table, a tombstone in the
// middle of a probe sequence, tombstone reuse, growth, deep copies -- so a
// broken category shows up here before submission.
//
// The harness is tests/test_framework.hpp. TEST, CHECK and CHECK_EQ are
// written here; main() is written by the harness.
//
// A test that THROWS fails by itself and the run continues. A test that
// SEGFAULTS cannot be caught -- the last name printed is the one that died.

#include "test_framework.hpp"

#include "wordcount.h"

#include <algorithm>
#include <string>
#include <vector>

// keys() may return the words in any order, so comparisons sort first.
static std::vector<std::string> sortedKeys(const WordCount& w) {
    std::vector<std::string> k = w.keys();
    std::sort(k.begin(), k.end());
    return k;
}

typedef std::vector<std::string> VS;

// ---- the boundaries, first -------------------------------------------

TEST("a new table is empty and has at least one slot") {
    WordCount w;
    CHECK(w.empty());
    CHECK_EQ(w.size(), 0);
    CHECK_EQ(w.tombstones(), 0);
    CHECK(w.capacity() >= 1);
    CHECK(w.keys().empty());
}

TEST("EMPTY: nothing is found and the load factor is zero") {
    WordCount w;
    CHECK(!w.contains("cat"));
    CHECK_EQ(w.get("cat"), 0);
    CHECK(w.loadFactor() == 0.0);
}

TEST("EMPTY: erase reports that there was nothing to erase") {
    WordCount w;
    CHECK(!w.erase("cat"));
    CHECK(w.empty());
    CHECK_EQ(w.tombstones(), 0);
}

TEST("the capacity constructor is honoured, and clamped below 1") {
    WordCount a(23);
    CHECK_EQ(a.capacity(), 23);
    WordCount b(0);
    CHECK(b.capacity() >= 1);
    b.put("x", 1);          // must not divide by zero
    CHECK_EQ(b.get("x"), 1);
}

// ---- hashing and lookup ----------------------------------------------

TEST("put then get returns the stored count") {
    WordCount w;
    w.put("cat", 4);
    CHECK(w.contains("cat"));
    CHECK_EQ(w.get("cat"), 4);
    CHECK_EQ(w.size(), 1);
}

TEST("get on an absent word returns 0 and stores nothing") {
    WordCount w;
    w.put("cat", 4);
    CHECK_EQ(w.get("dog"), 0);
    CHECK(!w.contains("dog"));
    CHECK_EQ(w.size(), 1);
}

TEST("a stored count of 0 is still a stored word") {
    // This is why contains() exists separately from get(): get() cannot
    // distinguish "absent" from "present with count 0".
    WordCount w;
    w.put("cat", 0);
    CHECK(w.contains("cat"));
    CHECK_EQ(w.get("cat"), 0);
    CHECK_EQ(w.size(), 1);
}

TEST("put replaces an existing count without adding a second entry") {
    WordCount w;
    w.put("cat", 4);
    w.put("cat", 9);
    CHECK_EQ(w.get("cat"), 9);
    CHECK_EQ(w.size(), 1);
    CHECK(sortedKeys(w) == VS({"cat"}));
}

TEST("the empty string is a valid key") {
    WordCount w;
    w.put("", 3);
    CHECK(w.contains(""));
    CHECK_EQ(w.get(""), 3);
    CHECK_EQ(w.size(), 1);
}

TEST("words differing only in their last character are distinct") {
    // A hash that reads only the first character would still pass this,
    // because collisions are handled. It fails if the KEY COMPARISON is
    // wrong -- comparing only lengths, or only the first character.
    WordCount w;
    w.put("stop", 1);
    w.put("stow", 2);
    w.put("stoop", 3);
    CHECK_EQ(w.get("stop"), 1);
    CHECK_EQ(w.get("stow"), 2);
    CHECK_EQ(w.get("stoop"), 3);
    CHECK_EQ(w.size(), 3);
}

TEST("anagrams are distinct keys") {
    WordCount w;
    w.put("abc", 1);
    w.put("cba", 2);
    w.put("bca", 3);
    CHECK_EQ(w.size(), 3);
    CHECK_EQ(w.get("abc"), 1);
    CHECK_EQ(w.get("cba"), 2);
    CHECK_EQ(w.get("bca"), 3);
}

// ---- increment --------------------------------------------------------

TEST("increment on an absent word inserts it with count 1") {
    WordCount w;
    w.increment("cat");
    CHECK_EQ(w.get("cat"), 1);
    CHECK_EQ(w.size(), 1);
}

TEST("increment on a present word adds one") {
    WordCount w;
    w.put("cat", 4);
    w.increment("cat");
    w.increment("cat");
    CHECK_EQ(w.get("cat"), 6);
    CHECK_EQ(w.size(), 1);
}

TEST("counting a short text") {
    WordCount w(7);
    const char* text[] = {"the", "cat", "sat", "on", "the", "mat",
                          "the", "cat", "ran"};
    for (int i = 0; i < 9; ++i) w.increment(text[i]);
    CHECK_EQ(w.size(), 6);
    CHECK_EQ(w.get("the"), 3);
    CHECK_EQ(w.get("cat"), 2);
    CHECK_EQ(w.get("ran"), 1);
    CHECK_EQ(w.get("dog"), 0);
}

// ---- tombstones: the part that is easy to get wrong -------------------

TEST("erase removes the word and leaves a tombstone behind") {
    WordCount w(11);
    w.put("cat", 1);
    CHECK(w.erase("cat"));
    CHECK(!w.contains("cat"));
    CHECK_EQ(w.size(), 0);
    CHECK_EQ(w.tombstones(), 1);
}

TEST("erase twice reports false the second time") {
    WordCount w(11);
    w.put("cat", 1);
    CHECK(w.erase("cat"));
    CHECK(!w.erase("cat"));
    CHECK_EQ(w.tombstones(), 1);     // and does not add a second tombstone
}

TEST("words stored past an erased one are still reachable") {
    // THE central test of this lab. Every even-numbered word is erased and
    // every odd-numbered one is then looked up. Any survivor that was
    // displaced from its home slot now has a tombstone somewhere ahead of
    // it in its probe sequence, and a search that stops at a tombstone
    // reports it missing.
    //
    // The word count has to be large for this to bite. At a load factor
    // under 0.5 most words sit at their home slot and are found without
    // probing at all; it takes a few hundred before enough of them are
    // displaced for the defect to show. Four hundred was measured against
    // a deliberately tombstone-blind implementation.
    const int N = 400;
    WordCount w(7);
    for (int i = 0; i < N; ++i) w.put("w" + std::to_string(i), i);
    for (int i = 0; i < N; i += 2) CHECK(w.erase("w" + std::to_string(i)));

    for (int i = 0; i < N; ++i) {
        const std::string word = "w" + std::to_string(i);
        const bool shouldBeThere = (i % 2) != 0;
        CHECK(w.contains(word) == shouldBeThere);
        if (shouldBeThere) CHECK_EQ(w.get(word), i);
    }
    CHECK_EQ(w.size(), N / 2);
}

TEST("the load factor counts tombstones") {
    WordCount w(10);
    w.put("a", 1);
    w.put("b", 2);
    CHECK(w.loadFactor() > 0.19 && w.loadFactor() < 0.21);   // 2/10
    w.erase("a");
    // size 1 + 1 tombstone over 10 slots is still 0.2, not 0.1. A
    // tombstone lengthens a probe sequence exactly as an occupied slot
    // does, so leaving it out of the load factor hides the cost.
    CHECK(w.loadFactor() > 0.19 && w.loadFactor() < 0.21);
}

TEST("reinserting an erased word reuses its tombstone") {
    WordCount w(11);
    w.put("cat", 1);
    w.erase("cat");
    CHECK_EQ(w.tombstones(), 1);
    w.put("cat", 5);
    CHECK_EQ(w.get("cat"), 5);
    CHECK_EQ(w.size(), 1);
    CHECK_EQ(w.tombstones(), 0);     // the tombstone was consumed, not kept
}

TEST("no duplicate key appears when a tombstone is reused") {
    // The trap: an insertion that stops at the FIRST tombstone it sees,
    // without continuing to an empty slot, writes a second copy of a word
    // that is already stored further along the same probe sequence.
    //
    // Nothing crashes when that happens. The table simply holds the word
    // twice, and from then on get() finds one copy while erase() removes
    // the other. keys() is the only place it shows.
    const int N = 1000;
    WordCount w(7);
    for (int i = 0; i < N; ++i) w.put("w" + std::to_string(i), i);
    for (int i = 0; i < N; i += 3) w.erase("w" + std::to_string(i));
    for (int i = 0; i < N; ++i) w.put("w" + std::to_string(i), 1000 + i);

    std::vector<std::string> k = sortedKeys(w);
    CHECK(std::adjacent_find(k.begin(), k.end()) == k.end());   // no repeats
    CHECK_EQ(static_cast<int>(k.size()), w.size());
    CHECK_EQ(w.size(), N);
    for (int i = 0; i < N; ++i)
        CHECK_EQ(w.get("w" + std::to_string(i)), 1000 + i);
}

TEST("erasing every word empties the table completely") {
    // The same defect seen from the other side. If any word is stored
    // twice, erasing each one once leaves the spare copies behind.
    const int N = 1000;
    WordCount w(7);
    for (int i = 0; i < N; ++i) w.put("w" + std::to_string(i), i);
    for (int i = 0; i < N; i += 3) w.erase("w" + std::to_string(i));
    for (int i = 0; i < N; ++i) w.put("w" + std::to_string(i), 1000 + i);
    for (int i = 0; i < N; ++i) CHECK(w.erase("w" + std::to_string(i)));

    CHECK_EQ(w.size(), 0);
    CHECK(w.empty());
    CHECK(w.keys().empty());
}

TEST("erase then increment behaves as a fresh insertion") {
    WordCount w(11);
    w.put("cat", 7);
    w.erase("cat");
    w.increment("cat");
    CHECK_EQ(w.get("cat"), 1);       // not 8: the old count is gone
    CHECK_EQ(w.size(), 1);
}

// ---- growth and rehashing ---------------------------------------------

TEST("the table grows rather than filling up") {
    WordCount w(3);
    for (int i = 0; i < 100; ++i) w.put("w" + std::to_string(i), i);
    CHECK(w.capacity() > 3);
    CHECK_EQ(w.size(), 100);
    // put() checks the load BEFORE inserting, as the lecture version does,
    // so the table may sit just over the threshold until the next call.
    CHECK(w.loadFactor() <= 0.6);
}

TEST("every word survives a rehash with its count") {
    WordCount w(3);
    for (int i = 0; i < 200; ++i) w.put("w" + std::to_string(i), i * 3);
    for (int i = 0; i < 200; ++i) {
        CHECK(w.contains("w" + std::to_string(i)));
        CHECK_EQ(w.get("w" + std::to_string(i)), i * 3);
    }
}

TEST("a rehash discards tombstones") {
    WordCount w(5);
    for (int i = 0; i < 4; ++i) w.put("w" + std::to_string(i), i);
    for (int i = 0; i < 4; ++i) w.erase("w" + std::to_string(i));
    CHECK(w.tombstones() > 0);
    for (int i = 0; i < 60; ++i) w.put("x" + std::to_string(i), i);
    CHECK_EQ(w.tombstones(), 0);
    CHECK_EQ(w.size(), 60);
}

TEST("a table that keeps a quarter of what it is given does not lose it") {
    // Every word is inserted and three out of four are erased again, which
    // is how a cache behaves. Tombstones therefore accumulate three times
    // faster than occupants do.
    //
    // A load factor that counts tombstones crosses 0.5, the table
    // rehashes, and the tombstones are cleared. One that counts only
    // occupants never reaches 0.5, so the table never grows; once every
    // slot holds either a record or a tombstone there is no empty slot for
    // an insertion to stop at, and put() stores nothing while reporting
    // nothing wrong.
    //
    // Measured against a tombstone-blind load factor: the table froze at
    // 23 slots and lost 4989 of 5000 records. Erasing only HALF does not
    // show this, because the occupant count alone then reaches 0.5 just in
    // time to trigger the rehash.
    const int N = 2000;
    WordCount w(11);
    for (int i = 0; i < N; ++i) {
        w.put("w" + std::to_string(i), i);
        if (i % 4 != 0) w.erase("w" + std::to_string(i));
    }
    int kept = 0;
    for (int i = 0; i < N; i += 4) {
        CHECK_EQ(w.get("w" + std::to_string(i)), i);
        ++kept;
    }
    CHECK_EQ(w.size(), kept);
}

// ---- housekeeping -----------------------------------------------------

TEST("clear empties the table, drops the tombstones, and leaves it usable") {
    WordCount w(11);
    for (int i = 0; i < 20; ++i) w.put("w" + std::to_string(i), i);
    w.erase("w3");
    w.clear();
    CHECK(w.empty());
    CHECK_EQ(w.size(), 0);
    CHECK_EQ(w.tombstones(), 0);
    CHECK(w.keys().empty());
    CHECK(!w.contains("w5"));
    w.put("after", 1);
    CHECK_EQ(w.get("after"), 1);
    CHECK_EQ(w.size(), 1);
}

TEST("keys returns exactly the stored words") {
    WordCount w(11);
    w.put("cat", 1);
    w.put("dog", 2);
    w.put("emu", 3);
    w.erase("dog");
    CHECK(sortedKeys(w) == VS({"cat", "emu"}));
    CHECK_EQ(static_cast<int>(w.keys().size()), w.size());
}

TEST("keys never reports an empty or tombstoned slot") {
    WordCount w(31);
    w.put("cat", 1);
    w.erase("cat");
    CHECK(w.keys().empty());
}

// ---- the Rule of Three ------------------------------------------------

TEST("the copy constructor is deep") {
    WordCount a(11);
    a.put("cat", 1);
    a.put("dog", 2);
    WordCount b = a;
    b.increment("cat");
    b.put("emu", 3);
    CHECK_EQ(a.get("cat"), 1);
    CHECK_EQ(b.get("cat"), 2);
    CHECK_EQ(a.size(), 2);
    CHECK_EQ(b.size(), 3);
}

TEST("copying a table that has tombstones keeps them") {
    const int N = 400;
    WordCount a(7);
    for (int i = 0; i < N; ++i) a.put("w" + std::to_string(i), i);
    for (int i = 0; i < N; i += 2) a.erase("w" + std::to_string(i));
    WordCount b = a;
    CHECK_EQ(b.size(), a.size());
    CHECK_EQ(b.tombstones(), a.tombstones());
    for (int i = 1; i < N; i += 2) CHECK_EQ(b.get("w" + std::to_string(i)), i);
    for (int i = 0; i < N; i += 2) CHECK(!b.contains("w" + std::to_string(i)));
}

TEST("assignment is deep and discards what was there") {
    WordCount a(11);
    a.put("cat", 1);
    WordCount b(11);
    b.put("ignored", 99);
    b = a;
    CHECK_EQ(b.size(), 1);
    CHECK(!b.contains("ignored"));
    b.put("emu", 3);
    CHECK_EQ(a.size(), 1);          // the original was not touched
}

TEST("self-assignment survives") {
    WordCount a(11);
    a.put("cat", 1);
    a.put("dog", 2);
    WordCount& ref = a;
    a = ref;                        // written through a reference so the
    CHECK_EQ(a.size(), 2);          // compiler does not simply discard it
    CHECK_EQ(a.get("cat"), 1);
    CHECK_EQ(a.get("dog"), 2);
}

// ---- one larger run ---------------------------------------------------

TEST("one thousand words, inserted, counted, half erased, verified") {
    WordCount w(7);
    for (int pass = 0; pass < 3; ++pass)
        for (int i = 0; i < 1000; ++i) w.increment("w" + std::to_string(i));
    CHECK_EQ(w.size(), 1000);
    CHECK_EQ(w.get("w500"), 3);

    for (int i = 0; i < 1000; i += 2) CHECK(w.erase("w" + std::to_string(i)));
    CHECK_EQ(w.size(), 500);
    for (int i = 1; i < 1000; i += 2) CHECK_EQ(w.get("w" + std::to_string(i)), 3);
    CHECK_EQ(static_cast<int>(w.keys().size()), 500);
}
