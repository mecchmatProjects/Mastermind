#include "InformationSolver.h"
#include <stdexcept>

#include <limits>
#include <cstddef>

InformationSolver::InformationSolver(const GameConfig& config) {
    candidates = generateAllCodes(
        config.codeLength,
        config.symbolCount,
        config.allowDuplicates
    );
}

void InformationSolver::update(
    const Code& guess,
    const GuessResult& result
) {
    std::vector<Code> remaining;

    for (const Code& candidate : candidates) {
        GuessResult actual = evaluate(candidate, guess);

        if (actual.exact == result.exact &&
            actual.partial == result.partial) {
            remaining.push_back(candidate);
        }
    }

    candidates = remaining;
}

Code InformationSolver::nextGuess() {
    if (candidates.empty()) {
        throw std::runtime_error("No candidates left");
    }

    if (candidates.size() == 1) {
        return candidates[0];
    }

    const std::size_t N = candidates[0].size();

    const std::size_t maxGuessesToCheck = 100;

    const std::size_t step =
        (candidates.size() + maxGuessesToCheck - 1) / maxGuessesToCheck;

    std::size_t bestScore = std::numeric_limits<std::size_t>::max();

    Code bestGuess = candidates[0];

    for (std::size_t i = 0; i < candidates.size(); i += step) {
        const Code& guess = candidates[i];

        std::vector<std::size_t> groups((N + 1) * (N + 1), 0);

        for (const Code& secret : candidates) {
            GuessResult result = evaluate(secret, guess);

            std::size_t index = result.exact * (N + 1) + result.partial;

            groups[index]++;
        }

        std::size_t score = 0;

        for (std::size_t count : groups) {
            score += count * count;
        }

        if (score < bestScore) {
            bestScore = score;
            bestGuess = guess;
        }
    }

    return bestGuess;
}