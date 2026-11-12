#pragma once

#include "CodeGenerator.h"
#include "Feedback.h"
#include "GameConfig.h"

class RandomSolver {
public:
    RandomSolver(const GameConfig& config);

    Code nextGuess();

    void update(const Code& guess, const GuessResult& result);

private:
    std::vector<Code> codes;
    size_t currentIndex = 0;
};