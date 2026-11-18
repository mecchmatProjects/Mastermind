#include "GuiApp.h"
#include "GameSession.h"
#include "../src/Difficulty.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>

namespace {
void centerText(sf::Text& text, sf::Vector2f position) {
    const auto bounds = text.getLocalBounds();
    text.setOrigin(bounds.position + bounds.size / 2.f);
    text.setPosition(position);
}

struct Button {
    sf::RectangleShape box;
    sf::Text label;

    Button(const sf::Font& font, const char* title, float y)
        : box({340.f, 54.f}), label(font, title, 25) {
        box.setPosition({310.f, y});
        centerText(label, box.getPosition() + box.getSize() / 2.f);
    }

    bool contains(sf::Vector2f point) const {
        return box.getGlobalBounds().contains(point);
    }

    void draw(sf::RenderWindow& window, bool selected = false) {
        box.setFillColor(selected ? sf::Color(101, 214, 174)
                                  : sf::Color(42, 53, 73));
        label.setFillColor(selected ? sf::Color(19, 24, 37)
                                    : sf::Color(236, 242, 250));
        window.draw(box);
        window.draw(label);
    }
};
}

void GuiApp::run() {
    sf::Font font;
    if (!font.openFromFile(MASTERMIND_FONT_PATH)) {
        std::cerr << "Could not load the Mastermind font\n";
        return;
    }

    sf::RenderWindow window(sf::VideoMode({960, 720}), "Mastermind",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    Button play(font, "Play", 300.f);
    Button watch(font, "Watch Computer", 370.f);
    Button easy(font, "Easy", 265.f);
    Button medium(font, "Medium", 335.f);
    Button hard(font, "Hard", 405.f);
    Button back(font, "Back", 505.f);

    sf::Text heading(font, "Mastermind", 54);
    centerText(heading, {480.f, 140.f});

    sf::Text subtitle(font, "", 26);
    subtitle.setFillColor(sf::Color(101, 214, 174));

    std::optional<GameMode> mode;
    std::optional<DifficultyLevel> difficulty;

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                break;
            }

            const auto* click = event->getIf<sf::Event::MouseButtonPressed>();
            if (!click || click->button != sf::Mouse::Button::Left) {
                continue;
            }

            const auto point = window.mapPixelToCoords(click->position);
            if (!mode) {
                if (play.contains(point)) {
                    mode = GameMode::Play;
                } else if (watch.contains(point)) {
                    mode = GameMode::WatchComputer;
                }
            } else if (easy.contains(point)) {
                difficulty = DifficultyLevel::Easy;
            } else if (medium.contains(point)) {
                difficulty = DifficultyLevel::Medium;
            } else if (hard.contains(point)) {
                difficulty = DifficultyLevel::Hard;
            } else if (back.contains(point)) {
                mode.reset();
                difficulty.reset();
            }
        }

        if (!window.isOpen()) {
            break;
        }

        window.clear(sf::Color(19, 24, 37));
        window.draw(heading);

        if (!mode) {
            subtitle.setString("Choose a game mode");
            play.draw(window);
            watch.draw(window);
        } else {
            subtitle.setString(*mode == GameMode::Play
                                   ? "Play / Choose a difficulty"
                                   : "Watch Computer / Choose a difficulty");
            easy.draw(window, difficulty == DifficultyLevel::Easy);
            medium.draw(window, difficulty == DifficultyLevel::Medium);
            hard.draw(window, difficulty == DifficultyLevel::Hard);
            back.draw(window);
        }

        centerText(subtitle, {480.f, 205.f});
        window.draw(subtitle);
        window.display();
    }
}