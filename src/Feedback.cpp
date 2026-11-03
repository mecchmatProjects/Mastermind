#include "Feedback.h"
#include <algorithm>

Feedback calculateFeedback(const Code& secret, const Code& guess) {
    int correctPosition = 0;

    for (int i = 0; i < secret.size(); i++) {
        if (secret[i] == guess[i]) {
            correctPosition++;
        }
    }

    std::vector<int> secretCount(7, 0);
    std::vector<int> guessCount(7, 0);

    for (int i : secret) {
        secretCount[i]++;
    }

    for (int i : guess) {
        guessCount[i]++;
    }

    int totalMatches = 0;
    for (int i = 1; i <= 6; i++) {
        totalMatches += std::min(secretCount[i], guessCount[i]);
    }

    int wrongPosition = totalMatches - correctPosition;

    return {correctPosition, wrongPosition};
}