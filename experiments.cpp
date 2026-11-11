#include "src/Simulation.h"
#include "src/EliminationSolver.h"
#include "src/RandomSolver.h"
#include "src/CodeGenerator.h"
#include "src/GameConfig.h"

#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

struct Experiment {
    GameConfig config;
    int games;
};

Code randomSecret(const std::vector<Code>& allCodes, std::mt19937& generator) {
    std::uniform_int_distribution<std::size_t> pick(0, allCodes.size() - 1);
    return allCodes[pick(generator)];
}

void printHeader() {
    std::cout << std::left
              << std::setw(4) << "N" << std::setw(4) << "K" << std::setw(8) << "Repeat"
              << std::setw(13) << "Solver" << std::setw(7) << "Games"
              << std::setw(9) << "Solved%" << std::setw(8) << "Avg" << std::setw(6) << "Max"
              << std::setw(12) << "Time(ms)" << '\n';
    std::cout << std::string(71, '-') << '\n';
}

void printRow(const GameConfig& c, const std::string& solverName, const SimulationResult& r) {
    std::cout << std::left << std::fixed << std::setprecision(2)
              << std::setw(4) << c.codeLength << std::setw(4) << c.symbolCount
              << std::setw(8) << (c.allowDuplicates ? "Yes" : "No")
              << std::setw(13) << solverName << std::setw(7) << r.stats.games
              << std::setw(9) << r.stats.solvedRate() * 100.0
              << std::setw(8) << r.stats.averageGuesses() << std::setw(6) << r.stats.maxGuesses
              << std::setw(12) << r.totalMs << '\n';
}

int main() {
    std::vector<Experiment> experiments = {
        {{3, 5, false, 12}, 200},
        {{4, 6, true, 12}, 200},
        {{5, 8, true, 12}, 30},
    };

    std::mt19937 generator(std::random_device{}());

    printHeader();

    for (const Experiment& e : experiments) {
        const GameConfig& c = e.config;
        std::vector<Code> allCodes = generateAllCodes(c.codeLength, c.symbolCount, c.allowDuplicates);

        auto makeSecret = [&]() { return randomSecret(allCodes, generator); };

        SimulationResult random = runSimulation(
            e.games, c.codeLength, c.maxAttempts, makeSecret,
            [&]() { return RandomSolver(c); }, evaluate);
        printRow(c, "Random", random);

        SimulationResult elimination = runSimulation(
            e.games, c.codeLength, c.maxAttempts, makeSecret,
            [&]() { return EliminationSolver(c); }, evaluate);
        printRow(c, "Elimination", elimination);
    }

    return 0;
}
