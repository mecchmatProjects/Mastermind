#include "EliminationSolver.h"
#include <stdexcept>

EliminationSolver::EliminationSolver(const GameConfig& config) {
    candidates = generateAllCodes(config.codeLength, config.symbolCount, config.allowDuplicates);

}

Code EliminationSolver::nextGuess() {
    if (candidates.empty()) {
        throw std::runtime_error("No candidates left");
    }
    return candidates[0];
}


void EliminationSolver::update(
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