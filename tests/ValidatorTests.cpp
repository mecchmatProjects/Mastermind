//
// Created by Katya on 07.11.2026.
//
#include <cassert>
#include <iostream>
#include "../src/Validator.h"

int main() {
    GameConfig config{4, 6, false, 10};

    assert(isValidGuess({1, 2, 3, 6}, config));
    assert(!isValidGuess({1, 2, 2, 4}, config));
    assert(!isValidGuess({1, 2, 3}, config));
    assert(!isValidGuess({1, 2, 3, 4, 5}, config));
    assert(!isValidGuess({0, 2, 3, 4}, config));
    assert(!isValidGuess({1, 2, 3, 7}, config));

    config.allowDuplicates = true;
    assert(isValidGuess({1, 1, 6, 6}, config));

    std::cout << "All validator tests passed";
}