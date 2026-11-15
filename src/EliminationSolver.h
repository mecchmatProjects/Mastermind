#pragma once
#include <cstddef>
#include "CodeGenerator.h"
#include "Feedback.h"
#include "GameConfig.h"

class EliminationSolver {

public:
    EliminationSolver(const GameConfig& config);
    Code nextGuess();
    void update(const Code& guess, const GuessResult& result);
    std::size_t candidateCount() const;

private:
    std::vector<Code> candidates;

};