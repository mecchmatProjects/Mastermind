#include "src/CodeGenerator.h"
#include "src/Feedback.h"
#include "src/EliminationSolver.h"
#include "src/RandomSolver.h"
#include <iostream>
#include <cassert>
#include <set>
#include <stdexcept>

int main() { 
    //auto codes = generateAllCodes(2, 3);
    //auto codes2 = generateAllCodes(4, 6);
    auto codes3 = generateAllCodes(2, 3, false);

    auto codesWithDuplicates = generateAllCodes(4, 6, true);
    auto codesWithoutDuplicates = generateAllCodes(4, 6, false);

    assert(codesWithDuplicates.size() == 1296);
    assert(codesWithoutDuplicates.size() == 360);
    
    auto impossibleCodes = generateAllCodes(5, 3, false);

    assert(impossibleCodes.empty());

    std::cout << "CodeGenerator impossible configuration test passed" << std::endl;

    std::cout << "CodeGenerator count tests passed" << std::endl;

    for (Code c : codes3) {
        for (int i : c) {
            std::cout << i << ' ';
        }
        std::cout << '\n';
    }

    //std::cout << codes.size() << '\n';
    //std::cout << codes2.size() << '\n';
    std::cout << codes3.size() << '\n';


    Code secret = {1, 2, 2, 4};
    Code guess = {2, 2, 3, 4};

    GuessResult result = evaluate(secret, guess);
    assert(result.exact == 2);
    assert(result.partial == 1);

    std::cout << "Correct: " << result.exact << '\n';
    std::cout << "Wrong: " << result.partial << '\n';


    Code secret2 = {7, 8, 2, 4};
    Code guess2 = {8, 7, 2, 5};

    GuessResult result2 = evaluate(secret2, guess2);
    assert(result2.exact == 1);
    assert(result2.partial == 2);

    std::cout << "Correct: " << result2.exact << std::endl;
    std::cout << "Wrong: " << result2.partial << std::endl;


    GameConfig config = {4, 6, true, 10};
    EliminationSolver solver(config);

    Code firstGuess = solver.nextGuess();
    assert(firstGuess == Code({1, 1, 1, 1}));

    solver.update(firstGuess, GuessResult{4, 0});

    Code next = solver.nextGuess();
    assert(next == firstGuess);

    std::cout << "EliminationSolver test passed" << std::endl;

    GameConfig config2 = {2, 3, true, 10};
    EliminationSolver solver2(config2);

    Code guess3 = {1, 1};
    GuessResult feedback3 = {0, 0};

    solver2.update(guess3, feedback3);

    Code next3 = solver2.nextGuess();
    assert(next3 == Code({2, 2}));

    std::cout << "Elimination filtering test passed" << std::endl;

    GameConfig randomConfig = {2, 3, false, 10};
    RandomSolver randomSolver(randomConfig);

    std::set<Code> usedCodes;

    for (int i = 0; i < 6; i++) {
        Code randomGuess = randomSolver.nextGuess();

        assert(randomGuess.size() == 2);

        for (int symbol : randomGuess) {
            assert(symbol >= 1 && symbol <= 3);
        }

        assert(randomGuess[0] != randomGuess[1]);
        assert(usedCodes.insert(randomGuess).second);

        randomSolver.update(randomGuess, GuessResult{0, 0});
    }

    assert(usedCodes.size() == 6);

    std::cout << "RandomSolver test passed" << std::endl;

    bool exceptionThrown = false;

    try {
        randomSolver.nextGuess();
    }
    catch (const std::runtime_error&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    std::cout << "RandomSolver exhaustion test passed" << std::endl;
    
    return 0;
}