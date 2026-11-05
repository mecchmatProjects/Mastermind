#include "src/Statistics.h"
#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    Statistics empty;
    assert(empty.averageGuesses() == 0.0);
    assert(empty.solvedRate() == 0.0);

    Statistics s;
    s.addGame(4, true);
    s.addGame(6, true);
    s.addGame(5, true);
    s.addGame(12, false);

    assert(s.games == 4);
    assert(s.solved == 3);
    assert(s.minGuesses == 4);
    assert(s.maxGuesses == 12);
    assert(std::fabs(s.averageGuesses() - 6.75) < 1e-9);
    assert(std::fabs(s.solvedRate() - 0.75) < 1e-9);
    assert(s.histogram[4] == 1 && s.histogram[12] == 1 && s.histogram[7] == 0);

    std::cout << "Statistics: all tests passed\n";
    return 0;
}
