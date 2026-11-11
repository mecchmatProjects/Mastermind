#include "Feedback.h"
#include <algorithm>
#include <stdexcept>

GuessResult evaluate(const Code& secret, const Code& guess) {
    if (secret.size() != guess.size()) {
        throw std::invalid_argument("Secret and guess must have the same length");
    }

    if (secret.empty() && guess.empty()) {
        return {0, 0};
    }

    int exact = 0;
    int maxSecret = *std::max_element(secret.begin(), secret.end());
    int maxGuess = *std::max_element(guess.begin(), guess.end());
    int maxValue = std::max(maxSecret, maxGuess);
    

    for (int i = 0; i < secret.size(); i++) {
        if (secret[i] == guess[i]) {
            exact++;
        }
    }

    std::vector<int> secretCount(maxValue + 1, 0);
    std::vector<int> guessCount(maxValue + 1, 0);

    for (int i : secret) {
        secretCount[i]++;
    }

    for (int i : guess) {
        guessCount[i]++;
    }

    int totalMatches = 0;
    for (int i = 1; i <= maxValue; i++) {
        totalMatches += std::min(secretCount[i], guessCount[i]);
    }

    int partial = totalMatches - exact;

    return {exact, partial};
}