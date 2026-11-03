#include "src/CodeGenerator.h"
#include "src/Feedback.h"
#include <iostream>

int main() { 
    //auto codes = generateAllCodes(2, 3);
    //auto codes2 = generateAllCodes(4, 6);
    auto codes3 = generateAllCodes(2, 3, false);

    for (Code c : codes3) {
        for (int i : c) {
            std::cout << i << ' ';
        }
        std::cout << '\n';
    }

    //std::cout << codes.size() << '\n';
    //std::cout << codes2.size() << '\n';
    std::cout << codes3.size() << '\n';


    Code secret = {1, 2, 2, 4};
    Code guess = {2, 2, 3, 4};

    Feedback result = calculateFeedback(secret, guess);

    std::cout << "Correct: " << result.correctPosition << '\n';
    std::cout << "Wrong: " << result.wrongPosition << '\n';
    
    return 0;
}