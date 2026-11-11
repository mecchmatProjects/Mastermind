#pragma once
#include "CodeGenerator.h"
#include "Feedback.h"
#include "GameConfig.h"

class EliminationSolver {

public:
    EliminationSolver(const GameConfig& config);
    Code nextGuess();
    void update(const Code& guess, const GuessResult& result);

private:
    std::vector<Code> candidates;

};