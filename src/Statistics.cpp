#include "Statistics.h"

void Statistics::addGame(int guessesUsed, bool wasSolved) {
    if (games == 0) {
        minGuesses = guessesUsed;
        maxGuesses = guessesUsed;
    } else {
        if (guessesUsed < minGuesses) minGuesses = guessesUsed;
        if (guessesUsed > maxGuesses) maxGuesses = guessesUsed;
    }

    games++;
    if (wasSolved) solved++;
    totalGuesses += guessesUsed;
    histogram[guessesUsed]++;
}

double Statistics::averageGuesses() const {
    if (games == 0) return 0.0;
    return static_cast<double>(totalGuesses) / games;
}

double Statistics::solvedRate() const {
    if (games == 0) return 0.0;
    return static_cast<double>(solved) / games;
}
