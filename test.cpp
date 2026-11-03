#include "src/CodeGenerator.h"
#include <iostream>

int main() { 
    auto codes = generateAllCodes(4, 6);

    std::cout << codes.size() << '\n';

    return 0;
}