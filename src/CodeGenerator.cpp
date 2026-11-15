#include "CodeGenerator.h"
#include <stdexcept>

void generate(int N, int K, bool allowDuplicates, Code& current, std::vector<Code>& allCodes) {

    if (current.size() == N) {
        allCodes.push_back(current);
        return;
    }

    for (int i = 1; i <= K; i++) {
        bool exists = false;

        for (int x : current) {
            if (i == x) {
                exists = true;
            }
        }

        if (allowDuplicates || !exists) {
            current.push_back(i);
            generate(N, K, allowDuplicates, current, allCodes);
            current.pop_back();
        }
    }

}

std::vector<Code> generateAllCodes(int N, int K, bool allowDuplicates) {
    if (N<= 0 || K<= 0){
        throw std::invalid_argument("N and K must be positive");
    }
    if (!allowDuplicates && N > K) {
        throw std::invalid_argument("Impossible configuration");
    }

    std::vector<Code> allCodes;
    Code current;

    generate(N, K, allowDuplicates, current, allCodes);

    return allCodes;
}
