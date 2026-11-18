#include "GameSession.h"

#include "../src/SecretGenerator.h"
#include "../src/Validator.h"

#include <utility>

bool GameSession::start(GameMode mode, GameConfig config) {
    if (!isValidConfig(config)) {
        return false;
    }

    state = SessionView{};
    state.mode = mode;
    state.config = config;
    state.attemptsLeft = config.maxAttempts;
    state.status = SessionStatus::Playing;

    secret = secretGenerator(config);

    solver.reset();

    if (mode == GameMode::WatchComputer) {
        solver.emplace(config);
    }

    return true;
}

bool GameSession::submitGuess(const Code& guess) {
    if (state.status != SessionStatus::Playing ||
        state.mode != GameMode::Play) {
        return false;
    }

    if (!isValidGuess(guess, state.config)) {
        return false;
    }

    recordTurn(guess);

    return true;
}

bool GameSession::stepComputer() {
    if (state.status != SessionStatus::Playing ||
        state.mode != GameMode::WatchComputer ||
        !solver.has_value()) {
        return false;
    }

    Code guess = solver->nextGuess();

    recordTurn(guess);

    if (state.status == SessionStatus::Playing) {
        solver->update(guess, state.history.back().result);
    }

    return true;
}

void GameSession::recordTurn(const Code& guess) {
    GuessResult result = evaluate(secret, guess);

    state.history.push_back({guess, result});
    --state.attemptsLeft;

    if (result.exact == state.config.codeLength) {
        state.status = SessionStatus::Solved;
        state.revealedSecret = secret;
    } else if (state.attemptsLeft == 0) {
        state.status = SessionStatus::OutOfAttempts;
        state.revealedSecret = secret;
    }
}

const SessionView& GameSession::view() const {
    return state;
}

GameSession::GameSession(
    std::function<Code(const GameConfig&)> generator
) : secretGenerator(std::move(generator)) {}