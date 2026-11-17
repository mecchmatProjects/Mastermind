#include "../gui/GameSession.h"

#include <cassert>
#include <iostream>

int main() {
    GameSession session;

    GameConfig config{4, 6, true, 10};

    assert(session.start(GameMode::Play, config));

    const SessionView& state = session.view();

    assert(state.status == SessionStatus::Playing);
    assert(state.mode == GameMode::Play);
    assert(state.attemptsLeft == 10);
    assert(state.history.empty());
    assert(!state.revealedSecret.has_value());

    std::cout << "GameSession start test passed\n";


    Code invalidGuess{1, 2, 3, 7};

    assert(!session.submitGuess(invalidGuess));
    assert(session.view().attemptsLeft == 10);
    assert(session.view().history.empty());

    Code validGuess{1, 2, 3, 4};

    assert(session.submitGuess(validGuess));
    assert(session.view().attemptsLeft == 9);
    assert(session.view().history.size() == 1);
    assert(session.view().history[0].guess == validGuess);

    std::cout << "GameSession submitGuess test passed\n";


    GameSession computerSession;
    GameConfig computerConfig{1, 2, false, 2};

    assert(computerSession.start(GameMode::WatchComputer, computerConfig));

    assert(computerSession.stepComputer());
    assert(computerSession.view().history.size() == 1);
    assert(computerSession.view().attemptsLeft == 1);

    if (computerSession.view().status == SessionStatus::Playing) {
        assert(computerSession.stepComputer());
    }

    assert(computerSession.view().status == SessionStatus::Solved);
    assert(computerSession.view().revealedSecret.has_value());
    assert(!computerSession.stepComputer());

    std::cout << "GameSession computer test passed\n";

    assert(session.start(GameMode::Play, config));

    assert(session.view().status == SessionStatus::Playing);
    assert(session.view().attemptsLeft == 10);
    assert(session.view().history.empty());
    assert(!session.view().revealedSecret.has_value());


    GameConfig invalidConfig{5, 3, false, 10};

    assert(!session.start(GameMode::Play, invalidConfig));

    std::cout << "GameSession restart and validation tests passed\n";

        GameSession loseSession([](const GameConfig&) {
        return Code{2};
    });

    GameConfig loseConfig{1, 2, false, 1};

    assert(loseSession.start(GameMode::Play, loseConfig));

    assert(loseSession.submitGuess(Code{1}));

    assert(loseSession.view().status == SessionStatus::OutOfAttempts);
    assert(loseSession.view().attemptsLeft == 0);
    assert(loseSession.view().history.size() == 1);
    assert(loseSession.view().revealedSecret.has_value());
    assert(loseSession.view().revealedSecret.value() == Code{2});

    assert(!loseSession.submitGuess(Code{1}));

    std::cout << "GameSession loss test passed\n";

        GameSession noDuplicatesSession([](const GameConfig&) {
        return Code{1, 2, 3};
    });

    GameConfig noDuplicatesConfig{3, 4, false, 5};

    assert(noDuplicatesSession.start(GameMode::Play, noDuplicatesConfig));

    Code duplicateGuess{1, 1, 2};

    assert(!noDuplicatesSession.submitGuess(duplicateGuess));
    assert(noDuplicatesSession.view().attemptsLeft == 5);
    assert(noDuplicatesSession.view().history.empty());

    std::cout << "GameSession no duplicates test passed\n";

    return 0;
}