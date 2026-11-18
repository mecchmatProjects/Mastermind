#include "src/Simulation.h"
#include "src/EliminationSolver.h"
#include "src/RandomSolver.h"
#include "src/InformationSolver.h"
#include "src/SecretGenerator.h"
#include "src/CodeGenerator.h"
#include "src/GameConfig.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

struct Experiment {
    GameConfig config;
    int games;
};

void printHeader() {
    std::cout << std::left
              << std::setw(4) << "N" << std::setw(4) << "K" << std::setw(8) << "Repeat"
              << std::setw(13) << "Solver" << std::setw(7) << "Games"
              << std::setw(9) << "Solved%" << std::setw(8) << "Avg" << std::setw(8) << "AvgWin" << std::setw(6) << "Max"
              << std::setw(12) << "Time(ms)" << '\n';
    std::cout << std::string(79, '-') << '\n';
}

void printRow(const GameConfig& c, const std::string& solverName, const SimulationResult& r) {
    std::cout << std::left << std::fixed << std::setprecision(2)
              << std::setw(4) << c.codeLength << std::setw(4) << c.symbolCount
              << std::setw(8) << (c.allowDuplicates ? "Yes" : "No")
              << std::setw(13) << solverName << std::setw(7) << r.stats.games
              << std::setw(9) << r.stats.solvedRate() * 100.0
              << std::setw(8) << r.stats.averageGuesses()
              << std::setw(8) << r.stats.averageGuessesSolved() << std::setw(6) << r.stats.maxGuesses
              << std::setw(12) << r.totalMs << '\n';
}

int main() {
    std::vector<Experiment> experiments = {
        {{3, 5, false, 12}, 200},
        {{4, 6, true, 12}, 100},
        {{5, 8, true, 12}, 15},
    };

    printHeader();

    for (const Experiment& e : experiments) {
        const GameConfig& c = e.config;
        std::vector<Code> secrets;
        for (int i = 0; i < e.games; i++) {
            secrets.push_back(generateSecret(c));
        }

        std::size_t next = 0;
        auto makeSecret = [&]() { return secrets[next++]; };

        next = 0;
        SimulationResult random = runSimulation(
            e.games, c.codeLength, c.maxAttempts, makeSecret,
            [&]() { return RandomSolver(c); }, evaluate);
        printRow(c, "Random", random);

        next = 0;
        SimulationResult elimination = runSimulation(
            e.games, c.codeLength, c.maxAttempts, makeSecret,
            [&]() { return EliminationSolver(c); }, evaluate);
        printRow(c, "Elimination", elimination);

        next = 0;
        SimulationResult information = runSimulation(
            e.games, c.codeLength, c.maxAttempts, makeSecret,
            [&]() { return InformationSolver(c); }, evaluate);
        printRow(c, "Information", information);
    }

    return 0;
}
