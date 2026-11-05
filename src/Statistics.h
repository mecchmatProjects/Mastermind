#pragma once
#include <map>

struct Statistics {
    int games = 0;
    int solved = 0;
    long long totalGuesses = 0;
    int minGuesses = 0;
    int maxGuesses = 0;
    std::map<int, int> histogram;

    void addGame(int guessesUsed, bool wasSolved);
    double averageGuesses() const;
    double solvedRate() const;
};
