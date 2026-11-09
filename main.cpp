//
// Created by Katya on 05.11.2026.
//
#include <iostream>
#include <sstream>
#include <string>

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
    if (!readConfig(config)) {
        return 0;
    }

    std::cout << "Code length: " << config.codeLength << '\n'
              << "Symbols: " << config.symbolCount << '\n'
              << "Duplicates: " << (config.allowDuplicates ? "yes" : "no") << '\n'
              << "Attempts: " << config.maxAttempts << '\n';

    Code guess;
    if (readGuess(guess, config)) {
        std::cout << "Guess accepted.\n";
    }
}