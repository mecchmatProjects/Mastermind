#include "GuiApp.h"
#include "GameSession.h"
#include "../src/Difficulty.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include <string>

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

        Button submit(font, "Submit guess", 568.f);
    Button next(font, "Next move", 568.f);
    Button menu(font, "Main menu", 637.f);

    sf::Text heading(font, "Mastermind", 54);
    centerText(heading, {480.f, 95.f});

    sf::Text text(font, "", 24);
    auto drawLine = [&](const std::string& line, float y) {
        text.setString(line);
        centerText(text, {480.f, y});
        window.draw(text);
    };

    auto formatCode = [](const Code& code) {
        std::string result;
        for (int digit : code) {
            result += std::to_string(digit) + " ";
        }
        return result;
    };

    std::optional<GameMode> mode;
    GameSession session;
    bool gameOpen = false;
    Code guess;
    std::string message;

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                break;
            }

            const auto& state = session.view();

            if (gameOpen && state.mode == GameMode::Play &&
                state.status == SessionStatus::Playing) {
                if (const auto* typed = event->getIf<sf::Event::TextEntered>()) {
                    if (typed->unicode >= U'1' &&
                        typed->unicode <= U'0' + state.config.symbolCount &&
                        guess.size() < static_cast<std::size_t>(state.config.codeLength)) {
                        guess.push_back(static_cast<int>(typed->unicode - U'0'));
                        message.clear();
                    }
                }

                if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                    if (key->code == sf::Keyboard::Key::Backspace && !guess.empty()) {
                        guess.pop_back();
                        message.clear();
                    }
                }
            }

            const auto* click = event->getIf<sf::Event::MouseButtonPressed>();
            if (!click || click->button != sf::Mouse::Button::Left) {
                continue;
            }

            const auto point = window.mapPixelToCoords(click->position);

            if (gameOpen) {
                if (menu.contains(point)) {
                    gameOpen = false;
                    mode.reset();
                } else if (state.status == SessionStatus::Playing) {
                    if (state.mode == GameMode::WatchComputer && next.contains(point)) {
                        session.stepComputer();
                    } else if (state.mode == GameMode::Play && submit.contains(point)) {
                        if (session.submitGuess(guess)) {
                            guess.clear();
                            message.clear();
                        } else {
                            if (guess.size() != static_cast<std::size_t>(state.config.codeLength)) {
                                message = "Enter " + std::to_string(state.config.codeLength) +
                                          " digits before submitting.";
                            } else {
                                message = "Use different digits on this difficulty.";
                            }
                        }
                    }
                }
            } else if (!mode) {
                if (play.contains(point)) {
                    mode = GameMode::Play;
                } else if (watch.contains(point)) {
                    mode = GameMode::WatchComputer;
                }
            } else {
                std::optional<DifficultyLevel> level;

                if (easy.contains(point)) {
                    level = DifficultyLevel::Easy;
                } else if (medium.contains(point)) {
                    level = DifficultyLevel::Medium;
                } else if (hard.contains(point)) {
                    level = DifficultyLevel::Hard;
                } else if (back.contains(point)) {
                    mode.reset();
                }

                if (level && session.start(*mode, makeDifficultyConfig(*level))) {
                    gameOpen = true;
                    guess.clear();
                    message.clear();
                }
            }
        }

        if (!window.isOpen()) {
            break;
        }

        window.clear(sf::Color(19, 24, 37));
        window.draw(heading);

        if (gameOpen) {
            const auto& state = session.view();
            drawLine("Attempts left: " + std::to_string(state.attemptsLeft), 177.f);

            float rowY = 224.f;
            for (const auto& turn : state.history) {
                drawLine(formatCode(turn.guess) + " | Exact: " +
                         std::to_string(turn.result.exact) + " | Partial: " +
                         std::to_string(turn.result.partial), rowY);
                rowY += 28.f;
            }

            if (state.status == SessionStatus::Playing) {
                if (state.mode == GameMode::Play) {
                    const auto prompt = "Type " + std::to_string(state.config.codeLength) +
                                        " digits (1-" + std::to_string(state.config.symbolCount) +
                                        "): " + formatCode(guess);
                    drawLine(prompt, 503.f);
                    if (!message.empty()) {
                        drawLine(message, 532.f);
                    }
                    submit.draw(window);
                } else {
                    next.draw(window);
                }
            } else {
                const std::string result = state.status == SessionStatus::Solved
                    ? "Code solved!" : "Out of attempts";
                drawLine(result, 532.f);

                if (state.revealedSecret) {
                    drawLine("Secret: " + formatCode(*state.revealedSecret), 574.f);
                }
            }

            menu.draw(window);
        } else if (!mode) {
            drawLine("Choose a game mode", 205.f);
            play.draw(window);
            watch.draw(window);
        } else {
            drawLine(*mode == GameMode::Play ? "Play / Choose a difficulty"
                                             : "Watch Computer / Choose a difficulty", 205.f);
            easy.draw(window);
            medium.draw(window);
            hard.draw(window);
            back.draw(window);
        }

        window.display();
    }
}