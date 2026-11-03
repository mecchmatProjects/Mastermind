#include "src/CodeGenerator.h"
#include <iostream>

int main() { 
    //auto codes = generateAllCodes(2, 3);
    //auto codes2 = generateAllCodes(4, 6);
    auto codes3 = generateAllCodes(2, 3, false);

    //std::cout << codes.size() << '\n';
    //std::cout << codes2.size() << '\n';
    std::cout << codes3.size() << '\n';

    return 0;
}