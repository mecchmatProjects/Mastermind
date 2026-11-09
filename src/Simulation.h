#pragma once
#include "Statistics.h"
#include "Metrics.h"

struct SimulationResult {
    Statistics stats;
    double totalMs = 0.0;
};

template <class MakeSecret, class MakeSolver, class Evaluate>
SimulationResult runSimulation(int games, int codeLength, int maxAttempts,
                               MakeSecret makeSecret, MakeSolver makeSolver, Evaluate evaluate) {
    SimulationResult result;
    Timer timer;

    for (int g = 0; g < games; g++) {
        auto secret = makeSecret();
        auto solver = makeSolver();

        int attempts = 0;
        bool solved = false;

        while (attempts < maxAttempts) {
            auto guess = solver.nextGuess();
            auto feedback = evaluate(secret, guess);
            attempts++;

            if (feedback.exact == codeLength) {
                solved = true;
                break;
            }
            solver.update(guess, feedback);
        }

        result.stats.addGame(attempts, solved);
    }

    result.totalMs = timer.elapsedMs();
    return result;
}
