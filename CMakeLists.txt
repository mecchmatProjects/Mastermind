cmake_minimum_required(VERSION 3.20)
project(Mastermind LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(Mastermind
        main.cpp
        src/GameConfig.h
        src/Code.h
        src/SecretGenerator.h
        src/SecretGenerator.cpp
        src/Validator.h
        src/Validator.cpp
        src/Feedback.cpp)

add_executable(ValidatorTests
        tests/ValidatorTests.cpp
        src/Validator.cpp)