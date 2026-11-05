//
// Created by Katya on 05.11.2026.
//
#include <iostream>
#include "src/GameConfig.h"

int main() {
    GameConfig config{4, 6, true, 10};
    std::cout << config.codeLength << '\n';
}