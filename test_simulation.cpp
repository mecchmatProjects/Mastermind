#include "src/Simulation.h"
#include <cassert>
#include <iostream>
#include <vector>

using Code = std::vector<int>;

struct GuessResult {
    int exact;
    int partial;
};

GuessResult fakeEvaluate(const Code& secret, const Code& guess) {
    int exact = 0;
    for (std::size_t i = 0; i < secret.size(); i++) {
        if (secret[i] == guess[i]) exact++;
    }
    return {exact, 0};
}

class SequentialSolver {
public:
    explicit SequentialSolver(std::vector<Code> codes) : codes(std::move(codes)), index(0) {}
    Code nextGuess() { return codes[index]; }
    void update(const Code&, const GuessResult&) { index++; }

private:
    std::vector<Code> codes;
    std::size_t index;
};

int main() {
    std::vector<Code> all = {{1, 1}, {1, 2}, {2, 1}, {2, 2}};

    int next = 0;
    auto makeSecret = [&]() { return all[next++ % all.size()]; };
    auto makeSolver = [&]() { return SequentialSolver(all); };

    SimulationResult r = runSimulation(4, 2, 10, makeSecret, makeSolver, fakeEvaluate);

    assert(r.stats.games == 4);
    assert(r.stats.solved == 4);
    assert(r.stats.minGuesses == 1);
    assert(r.stats.maxGuesses == 4);
    assert(r.stats.averageGuesses() == 2.5);
    assert(r.totalMs >= 0.0);

    next = 3;
    SimulationResult limited = runSimulation(1, 2, 2, makeSecret, makeSolver, fakeEvaluate);
    assert(limited.stats.solved == 0);
    assert(limited.stats.maxGuesses == 2);

    std::cout << "Simulation: all tests passed\n";
    return 0;
}
