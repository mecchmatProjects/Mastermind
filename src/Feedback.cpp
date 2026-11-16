#include "Feedback.h"
#include <stdexcept>
#include <unordered_map>

GuessResult evaluate(const Code& secret, const Code& guess) {
    if (secret.size() != guess.size()) {
        throw std::invalid_argument("Secret and guess must have the same length");
    }

    int exact = 0;
    std::unordered_map<int, int> unmatchedSecret;
    for (Code::size_type i = 0; i < secret.size(); ++i) {
        if (secret[i] == guess[i]) {
            ++exact;
        } else {
            ++unmatchedSecret[secret[i]];
        }
    }

    int partial = 0;
    for (Code::size_type i = 0; i < guess.size(); ++i) {
        if (guess[i] == secret[i]) {
            continue;
        }
        auto remaining = unmatchedSecret.find(guess[i]);
        if (remaining != unmatchedSecret.end() && remaining->second > 0) {
            --remaining->second;
            ++partial;
        }
    }

    return {exact, partial};
}