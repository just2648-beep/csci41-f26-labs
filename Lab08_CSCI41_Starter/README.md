# CSCI 41 - Lab 08: Stacks and Queues

A `Quack`: a double-ended queue of ints on a **circular array buffer that
grows**. Push and pop at either end. It is a stack from either end and a
queue across, which is where the name comes from.

```bash
mkdir -p build
cmake -S . -B build
cmake --build build
./build/lab08_tests     # the public self-check
./build/lab08           # the demo, not graded
```

## What you edit

`src/quack.cpp`, and nothing else. `include/quack.h` states the
representation and exactly what every function must do. Read it first.

## Four rules that will save you an evening

1. **Write `physicalIndex()` and `toVector()` first.** Everything else
   becomes readable once you have them, and `toVector()` is how you will
   debug every other function.
2. **Test the empty Quack and the Quack of one before anything else.**
3. **Watch the modulo on `pushFront`.** Going backwards from index 0 needs
   `(frontIndex_ - 1 + capacity_) % capacity_` - in C++, `%` with a negative
   left operand does not wrap the way you want.
4. **`grow()` is the hard one**, and it is hard in exactly one way: when the
   contents are **wrapped** they sit in two pieces. Copy by *logical*
   position, not physical index, and set `frontIndex_` to 0 afterwards.

Day 25 set up that last problem and deliberately left it unsolved. Day 26
gave you the arithmetic. This is where you do it.

## Build with sanitizers from the first function

```bash
g++ -std=c++17 -g -Wall -Wextra -D_GLIBCXX_ASSERTIONS
    -fsanitize=address -Iinclude
    src/quack.cpp tests/test_lab08.cpp -o t && ./t
```

**That is how it is graded.**

## Grading

- The 24 public checks in `tests/test_lab08.cpp` are a self-check, not your
  grade. Every KIND of case the private tests look at appears there.
- Private tests: 25 tests, 4 points each, 100 total.
- An unimplemented function throws, which fails only its own tests.

## Part E - EXTRA CREDIT, genuinely optional

Templates were taught the morning this lab went out, so nothing above
requires them. If you want the practice:

Make a copy of your finished `quack.h` as `quack_t.h` and turn `Quack` into
`template <typename T> class Quack`. Then make it hold `std::string` and
check it still behaves.

Worth up to 10 points on top. Submit it as a separate file; it is marked by
hand, and a broken Part E cannot reduce your score on Parts A-D.

## Submitting

Your repository IS the submission. Put your work in a `lab08/` folder inside
your `csci41-f26-labs` repository, commit as you go, and push.

- At least three meaningful commits.
- Graded on what is PUSHED when the deadline passes.
- No `build/` folder in the repository.
