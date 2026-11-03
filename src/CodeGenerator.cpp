#include "CodeGenerator.h"

void generate(int N, int K, Code& current, std::vector<Code>& allCodes) {

    if (current.size() == N) {
        allCodes.push_back(current);
        return;
    }

    for (int i = 1; i <= K; i++) {
        current.push_back(i);
        generate(N, K, current, allCodes);
        current.pop_back();
    }
}

std::vector<Code> generateAllCodes(int N, int K) {
    std::vector<Code> allCodes;
    Code current;

    generate(N, K, current, allCodes);

    return allCodes;
}
