// CSCI 41 -- Lab 09 demo program. Not graded; here so the table can be
// watched filling, growing and accumulating tombstones.   ./build/lab09
#include "wordcount.h"

#include <algorithm>
#include <iostream>

static void stats(const WordCount& w, const char* label) {
    std::cout << label << "  size=" << w.size()
              << "  tombstones=" << w.tombstones()
              << "  capacity=" << w.capacity()
              << "  load=" << w.loadFactor() << "\n";
}

int main() {
    WordCount w(7);
    stats(w, "empty            ");

    const char* text[] = {"the", "cat", "sat", "on", "the", "mat",
                          "the", "cat", "ran"};
    for (const char* word : text) w.increment(word);

    stats(w, "after 9 words    ");
    std::cout << "  the = " << w.get("the") << "\n";
    std::cout << "  cat = " << w.get("cat") << "\n";
    std::cout << "  dog = " << w.get("dog") << "   (absent)\n";

    std::vector<std::string> k = w.keys();
    std::sort(k.begin(), k.end());
    std::cout << "  keys: ";
    for (std::size_t i = 0; i < k.size(); ++i) std::cout << k[i] << " ";
    std::cout << "\n\n";

    w.erase("sat");
    stats(w, "after erase(sat) ");
    std::cout << "  contains(sat) = " << (w.contains("sat") ? "yes" : "no") << "\n";
    std::cout << "  contains(mat) = " << (w.contains("mat") ? "yes" : "no")
              << "   <- still reachable past the tombstone\n\n";

    WordCount copy = w;
    copy.increment("the");
    std::cout << "  original the = " << w.get("the") << "\n";
    std::cout << "  copy     the = " << copy.get("the") << "\n";
    std::cout << "  the copy changed and the original did not: deep copy.\n\n";

    for (int i = 0; i < 40; ++i) w.increment("filler" + std::to_string(i));
    stats(w, "after growth     ");
    std::cout << "  the = " << w.get("the") << "   <- survived the rehash\n";
    return 0;
}
