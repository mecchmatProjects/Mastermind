#pragma once

#include <optional>
#include <vector>
#include <functional>

#include "../src/EliminationSolver.h"
#include "../src/Feedback.h"
#include "../src/GameConfig.h"
#include "../src/SecretGenerator.h"

enum class GameMode { Play, WatchComputer };
enum class SessionStatus { NotStarted, Playing, Solved, OutOfAttempts };

struct Turn {
    Code guess;
    GuessResult result;
};

struct SessionView {
    GameMode mode = GameMode::Play;
    GameConfig config{};
    int attemptsLeft = 0;
    SessionStatus status = SessionStatus::NotStarted;
    std::vector<Turn> history;
    std::optional<Code> revealedSecret;
};

class GameSession {
public:
    explicit GameSession(
        std::function<Code(const GameConfig&)> secretGenerator = generateSecret
    );
    bool start(GameMode mode, GameConfig config);
    bool submitGuess(const Code& guess);
    bool stepComputer();
    const SessionView& view() const;

private:
    void recordTurn(const Code& guess);

    SessionView state;
    Code secret;
    std::optional<EliminationSolver> solver;

    std::function<Code(const GameConfig&)> secretGenerator;
};