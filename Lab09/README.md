# CSCI 41 - Lab 09: Hash Tables

A `WordCount`: a hash table mapping a word to a count, using **linear
probing with tombstones** and rehashing on growth. It counts how many times
each word appears.

```bash
mkdir -p build
cmake -S . -B build
cmake --build build
./build/lab09_tests     # the public self-check
./build/lab09           # the demo, not graded
```

## What you edit

`src/wordcount.cpp`, and nothing else. `include/wordcount.h` states the
representation and exactly what every function must do. Read it first.

## What Day 29 did and did not give you

Day 29 walked through a complete closed hash set of **ints**:
`d29/05_probing/hashset.cpp` in the demo bundle. The three ideas there are
the three ideas here:

- a slot is EMPTY, OCCUPIED or TOMBSTONE, and no key value is reserved to
  mean "empty";
- a search may stop only at EMPTY;
- an insertion remembers the first tombstone it passes but does not stop
  there until absence is proved.

This is a **map with string keys**, so none of that code transfers
unchanged. Slots carry a key *and* a value, the hash runs over characters
rather than one `%`, and the interface is `put`/`get`/`increment`/`erase`.
Reading the lecture version is worth doing. Copying it is not possible.

## Five rules that will save an evening

1. **Write `hashOf()` and `findSlot()` first.** Every other function is
   written in terms of them, and a bad hash makes every later bug harder to
   see.
2. **`hashOf` must involve every character.** A hash built from the first
   character alone puts every word starting with `s` into one probe
   sequence. Day 28's `02_distribution` demo shows what that costs.
3. **A search may stop only at EMPTY.** Stopping at a tombstone hides every
   word stored past it. That is Day 29's `03_deletion` demo, and the single
   test most submissions fail.
4. **`put()` may reuse the first tombstone it passed, but only after
   reaching an EMPTY slot** and so proving the word is not already stored
   further along. Reusing it early stores the same key twice, and from then
   on `erase` removes one copy and `get` finds the other.
5. **The load factor counts tombstones.** Leave them out and a table that
   is filled and emptied repeatedly never grows, while its probe sequences
   grow without bound. One public test is written to fail on exactly that.

## Build with sanitizers from the first function

```bash
g++ -std=c++17 -g -Wall -Wextra -D_GLIBCXX_ASSERTIONS
    -fsanitize=address -Iinclude
    src/wordcount.cpp tests/test_lab09.cpp -o t && ./t
```

**That is how it is graded.** This lab allocates with `new[]` and copies
slots holding `std::string`, so the leak and double-free checks matter.

## Grading

- The 34 public checks in `tests/test_lab09.cpp` are a self-check, not a
  grade. Every KIND of case the private tests look at appears there.
- Private tests: 25 tests, 4 points each, 100 total.
- An unimplemented function throws, which fails only its own tests.

## Where the hard parts are

| Part | Functions | Where it goes wrong |
|---|---|---|
| A | `hashOf`, `findSlot`, `contains`, `get` | stopping the search at a tombstone |
| B | `put`, `increment` | reusing a tombstone before proving absence |
| C | `erase` | emptying the slot instead of tombstoning it |
| D | `clear`, `keys`, `rehash`, Rule of Three | forgetting that `rehash` discards tombstones |

Parts A and C are short. Part B is the one worth drawing on paper before
writing: take the slot diagram from Friday's lecture, erase a middle
record, and insert a key whose home slot is before it.

## Submitting

The repository IS the submission. Put the work in a `lab09/` folder inside
the `csci41-f26-labs` repository, commit along the way, and push.

- At least three meaningful commits.
- Graded on what is PUSHED when the deadline passes.
- No `build/` folder in the repository.
