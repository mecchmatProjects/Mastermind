#pragma once

#include "GameConfig.h"

enum class DifficultyLevel {
    Easy,
    Medium,
    Hard
};

GameConfig makeDifficultyConfig(DifficultyLevel level);