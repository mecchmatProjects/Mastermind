cmake_minimum_required(VERSION 3.20)
project(Mastermind LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Mastermind
    main.cpp
    src/SecretGenerator.cpp
    src/Validator.cpp
    src/Feedback.cpp
)

add_executable(ValidatorTests
    tests/ValidatorTests.cpp
    src/Validator.cpp
)

add_executable(SolverTests
    test.cpp
    src/CodeGenerator.cpp
    src/Feedback.cpp
    src/EliminationSolver.cpp
    src/RandomSolver.cpp
)

enable_testing()
add_test(NAME ValidatorTests COMMAND ValidatorTests)
add_test(NAME SolverTests COMMAND SolverTests)
