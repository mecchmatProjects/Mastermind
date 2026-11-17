#include "GuiApp.h"

#include <SFML/Graphics.hpp>

void GuiApp::run() {
    sf::RenderWindow window(sf::VideoMode({960, 720}), "Mastermind");
    window.setFramerateLimit(60);

    sf::CircleShape peg(24.f);
    peg.setFillColor(sf::Color(101, 214, 174));
    peg.setPosition({456.f, 336.f});

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        if (window.isOpen()) {
            window.clear(sf::Color(19, 24, 37));
            window.draw(peg);
            window.display();
        }
    }
}