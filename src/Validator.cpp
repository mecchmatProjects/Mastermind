//
// Created by Katya on 07.11.2026.
//
#include "Validator.h"
#include <unordered_set>

bool isValidGuess(const Code& guess, const GameConfig& config) {
    if (!isValidConfig(config) ||
        guess.size() != static_cast<Code::size_type>(config.codeLength)) {
        return false;
        }

    std::unordered_set<int> used;

    for (int symbol : guess) {
        if (symbol < 1 || symbol > config.symbolCount) {
            return false;
        }

        if (!config.allowDuplicates && !used.insert(symbol).second) {
            return false;
        }
    }

    return true;
}