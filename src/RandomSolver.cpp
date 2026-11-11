#include "RandomSolver.h"
#include <algorithm>
#include <random>
#include <stdexcept>

RandomSolver::RandomSolver(const GameConfig& config) {
    codes = generateAllCodes(
        config.codeLength,
        config.symbolCount,
        config.allowDuplicates
    );

    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(codes.begin(), codes.end(), gen);
}

Code RandomSolver::nextGuess() {
    if (currentIndex >= codes.size()) {
        throw std::runtime_error("No codes left");
    }

    return codes[currentIndex++];
}

void RandomSolver::update(
    const Code& guess,
    const GuessResult& result
) {
    // RandomSolver does not filter candidates.
}