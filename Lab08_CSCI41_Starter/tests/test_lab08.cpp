// Lab 08 PUBLIC self-check.
//
// NOT your grade. Your grade comes from a larger private set built the same
// way against your src/quack.cpp. But every KIND of case the private set
// checks appears here at least once -- the empty Quack, the Quack of one,
// wrapping, GROWING WHILE WRAPPED, deep copies, self-assignment -- so a
// broken category shows up here before you submit.
//
// The harness is tests/test_framework.hpp. You write TEST, CHECK and
// CHECK_EQ; it writes main() for you.
//
// A test that THROWS fails by itself and the run continues. A test that
// SEGFAULTS cannot be caught -- the last name printed is the one that died.

#include "test_framework.hpp"

#include "quack.h"

#include <vector>

typedef std::vector<int> VI;

// ---- the boundaries, first -------------------------------------------

TEST("a new Quack is empty and has room") {
    Quack q;
    CHECK(q.empty());
    CHECK_EQ(q.size(), 0);
    CHECK(q.capacity() >= 1);
    CHECK(q.toVector().empty());
}

TEST("EMPTY: every pop and peek returns false") {
    Quack q;
    int x = 0;
    CHECK(!q.popFront(x));
    CHECK(!q.popBack(x));
    CHECK(!q.peekFront(x));
    CHECK(!q.peekBack(x));
    CHECK(q.empty());
}

TEST("EMPTY: quack() returns false") {
    Quack q;
    int a = 0, b = 0;
    CHECK(!q.quack(a, b));
    CHECK(q.empty());
}

TEST("ONE element: both ends see the same value") {
    Quack q;
    q.pushBack(7);
    int f = 0, b = 0;
    CHECK(q.peekFront(f));
    CHECK(q.peekBack(b));
    CHECK_EQ(f, 7);
    CHECK_EQ(b, 7);
    CHECK_EQ(q.size(), 1);
}

TEST("ONE element: quack() refuses, because it needs two") {
    Quack q;
    q.pushBack(7);
    int a = 0, b = 0;
    CHECK(!q.quack(a, b));
    CHECK_EQ(q.size(), 1);
}

TEST("ONE element: popping it empties the Quack") {
    Quack q;
    q.pushFront(7);
    int x = 0;
    CHECK(q.popBack(x));
    CHECK_EQ(x, 7);
    CHECK(q.empty());
    CHECK(!q.popFront(x));
}

// ---- pushing and popping ---------------------------------------------

TEST("pushBack appends, pushFront prepends") {
    Quack q;
    q.pushBack(2); q.pushBack(3);
    q.pushFront(1);
    CHECK(q.toVector() == VI({1, 2, 3}));
    CHECK_EQ(q.size(), 3);
}

TEST("popFront and popBack take from the right ends") {
    Quack q;
    q.pushBack(1); q.pushBack(2); q.pushBack(3);
    int f = 0, b = 0;
    CHECK(q.popFront(f));
    CHECK(q.popBack(b));
    CHECK_EQ(f, 1);
    CHECK_EQ(b, 3);
    CHECK(q.toVector() == VI({2}));
}

TEST("peek does not remove anything") {
    Quack q;
    q.pushBack(1); q.pushBack(2);
    int f = 0, b = 0;
    q.peekFront(f); q.peekBack(b);
    q.peekFront(f); q.peekBack(b);
    CHECK_EQ(f, 1);
    CHECK_EQ(b, 2);
    CHECK_EQ(q.size(), 2);
}

TEST("it works as a STACK from the back") {
    Quack q;
    for (int i = 1; i <= 4; ++i) q.pushBack(i);
    int x = 0;
    VI got;
    while (q.popBack(x)) got.push_back(x);
    CHECK(got == VI({4, 3, 2, 1}));
}

TEST("it works as a QUEUE across") {
    Quack q;
    for (int i = 1; i <= 4; ++i) q.pushBack(i);
    int x = 0;
    VI got;
    while (q.popFront(x)) got.push_back(x);
    CHECK(got == VI({1, 2, 3, 4}));
}

// ---- wrapping and growing --------------------------------------------

TEST("the buffer WRAPS without growing") {
    Quack q(4);
    q.pushBack(1); q.pushBack(2); q.pushBack(3);
    int x = 0;
    q.popFront(x); q.popFront(x);        // front has moved along
    q.pushBack(4); q.pushBack(5);        // these wrap round
    CHECK_EQ(q.capacity(), 4);
    CHECK(q.toVector() == VI({3, 4, 5}));
}

TEST("it GROWS when full, and keeps the order") {
    Quack q(2);
    const int before = q.capacity();
    for (int i = 1; i <= 8; ++i) q.pushBack(i);
    CHECK(q.capacity() > before);
    CHECK_EQ(q.size(), 8);
    CHECK(q.toVector() == VI({1, 2, 3, 4, 5, 6, 7, 8}));
}

TEST("it grows correctly WHILE WRAPPED -- the hard one") {
    Quack q(4);
    q.pushBack(1); q.pushBack(2); q.pushBack(3);
    int x = 0;
    q.popFront(x); q.popFront(x);        // {3}, front sitting at index 2
    q.pushBack(4); q.pushBack(5);        // {3,4,5}, now wrapped
    CHECK(q.toVector() == VI({3, 4, 5}));
    q.pushBack(6);                       // fills it
    q.pushBack(7);                       // forces a grow WHILE WRAPPED
    CHECK(q.toVector() == VI({3, 4, 5, 6, 7}));
    CHECK_EQ(q.size(), 5);
}

TEST("growing from pushFront while genuinely wrapped") {
    // Four pushFronts on a fresh Quack land frontIndex_ back on 0, which is
    // NOT wrapped. Start with a pushBack so the front really does wrap.
    Quack q(4);
    q.pushBack(1); q.pushBack(2);     // front at 0, size 2
    q.pushFront(0);                   // front wraps to index 3
    q.pushFront(-1);                  // front at 2, full, wrapped
    CHECK(q.toVector() == VI({-1, 0, 1, 2}));
    q.pushFront(-2);                  // forces a grow WHILE WRAPPED
    CHECK(q.toVector() == VI({-2, -1, 0, 1, 2}));
    CHECK_EQ(q.size(), 5);
}

TEST("a long mixed run stays in order") {
    Quack q(2);
    for (int i = 0; i < 50; ++i) {
        if (i % 2 == 0) q.pushBack(i);
        else            q.pushFront(i);
    }
    CHECK_EQ(q.size(), 50);
    VI v = q.toVector();
    CHECK_EQ(static_cast<int>(v.size()), 50);
    CHECK_EQ(v.front(), 49);
    CHECK_EQ(v.back(), 48);
}

// ---- quack() ----------------------------------------------------------

TEST("quack() takes both ends at once") {
    Quack q;
    for (int i = 1; i <= 4; ++i) q.pushBack(i);
    int a = 0, b = 0;
    CHECK(q.quack(a, b));
    CHECK_EQ(a, 1);
    CHECK_EQ(b, 4);
    CHECK(q.toVector() == VI({2, 3}));
}

TEST("quack() on exactly two empties it") {
    Quack q;
    q.pushBack(1); q.pushBack(2);
    int a = 0, b = 0;
    CHECK(q.quack(a, b));
    CHECK_EQ(a, 1);
    CHECK_EQ(b, 2);
    CHECK(q.empty());
}

// ---- clear -------------------------------------------------------------

TEST("clear empties it and it still works") {
    Quack q;
    for (int i = 0; i < 10; ++i) q.pushBack(i);
    q.clear();
    CHECK(q.empty());
    CHECK_EQ(q.size(), 0);
    CHECK(q.toVector().empty());
    q.pushBack(42);
    CHECK_EQ(q.size(), 1);
    CHECK(q.toVector() == VI({42}));
}

// ---- the Rule of Three -------------------------------------------------

TEST("the copy constructor makes a DEEP copy") {
    Quack a;
    a.pushBack(1); a.pushBack(2);
    Quack b = a;
    b.pushBack(3);
    CHECK(a.toVector() == VI({1, 2}));
    CHECK(b.toVector() == VI({1, 2, 3}));
}

TEST("copying a WRAPPED Quack keeps the order") {
    Quack a(4);
    a.pushBack(1); a.pushBack(2); a.pushBack(3);
    int x = 0;
    a.popFront(x); a.popFront(x);
    a.pushBack(4); a.pushBack(5);        // wrapped: {3,4,5}
    Quack b = a;
    CHECK(b.toVector() == VI({3, 4, 5}));
}

TEST("assignment makes a DEEP copy and discards the old contents") {
    Quack a;
    a.pushBack(1); a.pushBack(2);
    Quack b;
    b.pushBack(99);
    b = a;
    CHECK(b.toVector() == VI({1, 2}));
    b.pushBack(3);
    CHECK(a.toVector() == VI({1, 2}));
}

TEST("self-assignment does not destroy the Quack") {
    Quack a;
    a.pushBack(1); a.pushBack(2); a.pushBack(3);
    a = a;
    CHECK(a.toVector() == VI({1, 2, 3}));
    CHECK_EQ(a.size(), 3);
}

TEST("copying an EMPTY Quack is fine") {
    Quack a;
    Quack b = a;
    CHECK(b.empty());
    b.pushBack(1);
    CHECK(a.empty());
    CHECK_EQ(b.size(), 1);
}
