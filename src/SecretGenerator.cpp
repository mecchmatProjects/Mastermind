//
// Created by Katya on 05.11.2026.
//
#include "SecretGenerator.h"

#include <random>
#include <stdexcept>
#include <unordered_set>

Code generateSecret(const GameConfig& config) {
    if (!isValidConfig(config)) {
        throw std::invalid_argument("Invalid game configuration");
    }

    static std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> pick(1, config.symbolCount);

    Code secret;
    std::unordered_set<int> used;
    const auto length = static_cast<Code::size_type>(config.codeLength);

    while (secret.size() < length) {
        int symbol = pick(generator);

        if (config.allowDuplicates || used.insert(symbol).second) {
            secret.push_back(symbol);
        }
    }

    return secret;
}