// CSCI 41 -- Lab 08 demo program. Not graded; here so you can watch the
// buffer wrap and grow.   ./build/lab08
#include "quack.h"

#include <iostream>

static void show(const Quack& q, const char* label) {
    std::cout << label << "  size=" << q.size()
              << "  capacity=" << q.capacity() << "   [";
    std::vector<int> v = q.toVector();
    for (std::size_t i = 0; i < v.size(); ++i)
        std::cout << v[i] << (i + 1 < v.size() ? " " : "");
    std::cout << "]\n";
}

int main() {
    Quack q(4);
    show(q, "empty            ");

    q.pushBack(1); q.pushBack(2); q.pushBack(3);
    show(q, "pushBack 1,2,3   ");

    q.pushFront(0);
    show(q, "pushFront 0      ");

    // This one has to grow, and the contents are already wrapped.
    q.pushFront(-1);
    show(q, "pushFront -1 (grew)");

    int a = 0, b = 0;
    q.quack(a, b);
    std::cout << "quack() took " << a << " off the front and "
              << b << " off the back\n";
    show(q, "after quack      ");

    Quack copy = q;
    copy.pushBack(99);
    show(q,    "original         ");
    show(copy, "the copy         ");
    std::cout << "the copy changed and the original did not: deep copy.\n";

    q.clear();
    show(q, "after clear      ");
    return 0;
}
