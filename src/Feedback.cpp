#include "Feedback.h"

#include <stdexcept>
#include <unordered_map>

GuessResult evaluate(const Code& secret, const Code& guess) {
    if (secret.size() != guess.size()) {
        throw std::invalid_argument("Codes must have the same length");
    }

    GuessResult result{0, 0};
    std::unordered_map<int, int> remaining;

    for (Code::size_type i = 0; i < secret.size(); ++i) {
        if (secret[i] == guess[i]) {
            ++result.exact;
        } else {
            ++remaining[secret[i]];
        }
    }

    for (Code::size_type i = 0; i < guess.size(); ++i) {
        if (secret[i] == guess[i]) {
            continue;
        }

        auto symbol = remaining.find(guess[i]);
        if (symbol != remaining.end() && symbol->second > 0) {
            ++result.partial;
            --symbol->second;
        }
    }

    return result;
}