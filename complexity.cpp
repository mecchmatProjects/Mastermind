#include "src/CodeGenerator.h"
#include "src/EliminationSolver.h"
#include "src/Feedback.h"
#include "src/GameConfig.h"
#include "src/Metrics.h"
#include "src/SecretGenerator.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

unsigned long long theoreticalCount(int N, int K, bool allowDuplicates) {
    unsigned long long total = 1;
    for (int i = 0; i < N; i++) {
        if (allowDuplicates) {
            total *= static_cast<unsigned long long>(K);
        } else {
            total *= static_cast<unsigned long long>(K - i);
        }
    }
    return total;
}

void searchSpaceTable() {
    const unsigned long long generateLimit = 2000000;

    struct Case {
        int N;
        int K;
        bool dup;
    };

    std::vector<Case> cases = {
        {4, 6, false}, {4, 6, true},
        {5, 6, true},
        {5, 8, false}, {5, 8, true},
        {6, 8, true},
        {6, 10, false},
        {7, 10, true},
    };

    std::cout << "Search space\n";
    std::cout << std::left
              << std::setw(4) << "N" << std::setw(4) << "K" << std::setw(8) << "Repeat"
              << std::setw(12) << "Codes" << std::setw(14) << "Gen(ms)"
              << std::setw(14) << "Memory(MB)" << std::setw(14) << "Filter(ms)"
              << std::setw(12) << "Left" << '\n';
    std::cout << std::string(82, '-') << '\n';

    for (const Case& c : cases) {
        unsigned long long expected = theoreticalCount(c.N, c.K, c.dup);

        std::cout << std::left << std::fixed << std::setprecision(2)
                  << std::setw(4) << c.N << std::setw(4) << c.K
                  << std::setw(8) << (c.dup ? "Yes" : "No")
                  << std::setw(12) << expected;

        if (expected > generateLimit) {
            std::cout << std::setw(14) << "skipped" << std::setw(14)
                      << static_cast<double>(estimateCodesMemoryBytes(expected, c.N)) / (1024.0 * 1024.0)
                      << std::setw(14) << "-" << std::setw(12) << "-" << " (estimate only)\n";
            continue;
        }

        Timer timer;
        std::vector<Code> codes = generateAllCodes(c.N, c.K, c.dup);
        double genMs = timer.elapsedMs();

        double memoryMb = static_cast<double>(estimateCodesMemoryBytes(codes.size(), c.N)) / (1024.0 * 1024.0);

        Code guess = codes[0];
        GuessResult target = evaluate(codes[codes.size() / 2], guess);

        timer.reset();
        std::size_t remaining = 0;
        for (const Code& code : codes) {
            GuessResult r = evaluate(code, guess);
            if (r.exact == target.exact && r.partial == target.partial) {
                remaining++;
            }
        }
        double filterMs = timer.elapsedMs();

        std::cout << std::setw(14) << genMs << std::setw(14) << memoryMb
                  << std::setw(14) << filterMs << std::setw(12) << remaining << '\n';
    }
    std::cout << '\n';
}

void candidatesPerMove(const GameConfig& c, int games, int maxMoves) {
    std::vector<double> sum(maxMoves + 1, 0.0);
    std::vector<int> count(maxMoves + 1, 0);

    for (int g = 0; g < games; g++) {
        Code secret = generateSecret(c);
        EliminationSolver solver(c);

        sum[0] += static_cast<double>(solver.candidateCount());
        count[0]++;

        for (int move = 1; move <= maxMoves; move++) {
            Code guess = solver.nextGuess();
            GuessResult result = evaluate(secret, guess);
            if (result.exact == c.codeLength) {
                break;
            }
            solver.update(guess, result);
            sum[move] += static_cast<double>(solver.candidateCount());
            count[move]++;
        }
    }

    std::cout << "Candidates left (Elimination), N=" << c.codeLength << " K=" << c.symbolCount
              << " repeat=" << (c.allowDuplicates ? "yes" : "no") << ", average over " << games << " games\n";
    std::cout << std::left << std::setw(8) << "Move" << std::setw(14) << "Candidates" << std::setw(8) << "Games" << '\n';
    std::cout << std::string(30, '-') << '\n';

    for (int move = 0; move <= maxMoves; move++) {
        if (count[move] == 0) {
            break;
        }
        std::cout << std::left << std::fixed << std::setprecision(1)
                  << std::setw(8) << move << std::setw(14) << sum[move] / count[move]
                  << std::setw(8) << count[move] << '\n';
    }
    std::cout << '\n';
}

int main() {
    searchSpaceTable();

    candidatesPerMove({4, 6, true, 12}, 50, 8);
    candidatesPerMove({5, 8, true, 12}, 10, 8);

    std::size_t peak = peakMemoryBytes();
    if (peak == 0) {
        std::cout << "Peak memory of this process: not available on this platform\n";
    } else {
        std::cout << "Peak memory of this process: "
                  << static_cast<double>(peak) / (1024.0 * 1024.0) << " MB\n";
    }
    return 0;
}
