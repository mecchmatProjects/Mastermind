//
// Created by Katya on 05.11.2026.
//

#pragma once

struct GameConfig {
    int codeLength;
    int symbolCount;
    bool allowDuplicates;
    int maxAttempts;
};

inline bool isValidConfig(const GameConfig& config) {
    if (config.codeLength <= 0 || config.symbolCount <= 0 ||
        config.maxAttempts <= 0) {
        return false;
        }

    return config.allowDuplicates || config.codeLength <= config.symbolCount;
}
