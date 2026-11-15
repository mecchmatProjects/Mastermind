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

add_executable(StatisticsTests
    test_statistics.cpp
    src/Statistics.cpp
)

add_executable(MetricsTests
    test_metrics.cpp
    src/Metrics.cpp
)

add_executable(SimulationTests
    test_simulation.cpp
    src/Statistics.cpp
    src/Metrics.cpp
)

add_executable(Experiments
    experiments.cpp
    src/Statistics.cpp
    src/Metrics.cpp
    src/CodeGenerator.cpp
    src/Feedback.cpp
    src/EliminationSolver.cpp
    src/RandomSolver.cpp
    src/SecretGenerator.cpp
)

add_test(NAME StatisticsTests COMMAND StatisticsTests)
add_test(NAME MetricsTests COMMAND MetricsTests)
add_test(NAME SimulationTests COMMAND SimulationTests)