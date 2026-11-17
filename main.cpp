//
// Created by Katya on 05.11.2026.
//
#include <iostream>
#include <sstream>
#include <string>
#include "src/Difficulty.h"
#include "src/Feedback.h"
#include "src/SecretGenerator.h"
#include <optional>
#include "src/EliminationSolver.h"
#include "src/Validator.h"

bool readNumber(std::istringstream& input, int& number) {
    std::string token;
    if (!(input >> token)) {
        return false;
    }

    std::istringstream value(token);
    return (value >> number) && value.eof();
}

bool readConfig(GameConfig& config) {
    std::cout << "Enter length, symbol count, duplicates (0 or 1), and max attempts:\n";

    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream values(line);
        int duplicates;

        if (readNumber(values, config.codeLength) &&
            readNumber(values, config.symbolCount) &&
            readNumber(values, duplicates) &&
            readNumber(values, config.maxAttempts) &&
            (values >> std::ws).eof() &&
            (duplicates == 0 || duplicates == 1)) {
            config.allowDuplicates = duplicates == 1;

            if (isValidConfig(config)) {
                return true;
            }
        }

        std::cout << "Invalid settings. Try again.\n";
    }

    return false;
}

bool readGuess(Code& guess, const GameConfig& config) {
    std::cout << "Enter " << config.codeLength << " numbers from 1 to "
              << config.symbolCount << ", separated by spaces:\n";

    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream values(line);
        guess.clear();

        for (int i = 0; i < config.codeLength; ++i) {
            int symbol;
            if (!readNumber(values, symbol)) {
                break;
            }
            guess.push_back(symbol);
        }

        if ((values >> std::ws).eof() && isValidGuess(guess, config)) {
            return true;
        }

        std::cout << "Invalid guess. Try again.\n";
    }

    return false;
}

int main() {
    GameConfig config{};
    std::string line;

    int mode;

    while (true) {
        std::cout << "Choose game mode:\n"
                  << "1. You guess the code\n"
                  << "2. Computer guesses the code\n";

        if (!std::getline(std::cin, line)) {
            return 0;
        }

        std::istringstream input(line);
        if (readNumber(input, mode) && (input >> std::ws).eof() &&
            (mode == 1 || mode == 2)) {
            break;
            }

        std::cout << "Enter 1 or 2.\n";
    }

    while (true) {
        std::cout << "Choose difficulty:\n"
                  << "1. Easy\n"
                  << "2. Medium\n"
                  << "3. Hard\n";
        if (mode == 1) {
            std::cout << "0. Custom\n";
        }
        if (!std::getline(std::cin, line)) {
            return 0;
        }

        std::istringstream input(line);
        int choice;
        if (!readNumber(input, choice) || !(input >> std::ws).eof() ||
            choice < 0 || choice > 3) {
            std::cout << "Enter a number from 0 to 3.\n";
            continue;
            }

        if (choice == 0) {
            if (mode == 2) {
                std::cout << "Choose a difficulty from 1 to 3.\n";
                continue;
            }
            if (!readConfig(config)) {
                return 0;
            }
        } else if (choice == 1) {
            config = makeDifficultyConfig(DifficultyLevel::Easy);
        } else if (choice == 2) {
            config = makeDifficultyConfig(DifficultyLevel::Medium);
        } else {
            config = makeDifficultyConfig(DifficultyLevel::Hard);
        }

        break;
    }

    std::cout << "Code length: " << config.codeLength << '\n'
              << "Symbols: " << config.symbolCount << '\n'
              << "Duplicates: " << (config.allowDuplicates ? "yes" : "no") << '\n'
              << "Attempts: " << config.maxAttempts << '\n';

    std::optional<EliminationSolver> solver;
    if (mode == 2) {
        solver.emplace(config);
    }

    const Code secret = generateSecret(config);
    Code guess;

    for (int attemptsLeft = config.maxAttempts; attemptsLeft > 0; --attemptsLeft) {
        if (solver) {
            guess = solver->nextGuess();
            std::cout << "Computer guess:";
            for (int symbol : guess) {
                std::cout << ' ' << symbol;
            }
            std::cout << '\n';
        } else if (!readGuess(guess, config)) {
            return 0;
        }

        const GuessResult result = evaluate(secret, guess);
        std::cout << "Exact: " << result.exact << '\n'
                  << "Partial: " << result.partial << '\n'
                  << "Attempts remaining: " << attemptsLeft - 1 << '\n';

        if (result.exact == config.codeLength) {
            std::cout << (solver ? "Computer guessed the secret!\n"
                                 : "You guessed the secret!\n");
            return 0;
        }

        if (solver && attemptsLeft > 1) {
            solver->update(guess, result);
        }
    }

    std::cout << "No attempts left. The secret was:";
    for (int symbol : secret) {
        std::cout << ' ' << symbol;
    }
    std::cout << '\n';
}