find_package(SFML 3 REQUIRED COMPONENTS Graphics)

add_executable(MastermindGUI
        main.cpp
        GuiApp.cpp
)

target_link_libraries(MastermindGUI PRIVATE SFML::Graphics)