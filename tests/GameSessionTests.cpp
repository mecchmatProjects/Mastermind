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

    return 0;
}