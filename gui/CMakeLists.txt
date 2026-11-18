find_package(SFML 3 REQUIRED COMPONENTS Graphics)

add_executable(MastermindGUI
        main.cpp
        GuiApp.cpp
        GameSession.cpp
        ../src/Difficulty.cpp
        ../src/SecretGenerator.cpp
        ../src/Validator.cpp
        ../src/Feedback.cpp
        ../src/CodeGenerator.cpp
        ../src/EliminationSolver.cpp
)

target_link_libraries(MastermindGUI PRIVATE SFML::Graphics)

target_compile_definitions(MastermindGUI PRIVATE
        MASTERMIND_FONT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/Barlow-SemiBold.ttf"
)