#include "Difficulty.h"
#include <stdexcept>

GameConfig makeDifficultyConfig(DifficultyLevel level) {
    switch (level) {
        case DifficultyLevel::Easy:
            return {3, 4, false, 10};

        case DifficultyLevel::Medium:
            return {4, 6, true, 10};

        case DifficultyLevel::Hard:
            return {5, 8, true, 8};
    }

    throw std::invalid_argument("Unknown difficulty level");
}