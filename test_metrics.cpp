#include "src/Metrics.h"
#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    Timer t;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    double ms = t.elapsedMs();
    assert(ms >= 45.0 && ms < 500.0);

    t.reset();
    assert(t.elapsedMs() < 40.0);

    assert(estimateCodesMemoryBytes(0, 4) == 0);
    std::size_t one = estimateCodesMemoryBytes(1, 4);
    assert(one == sizeof(std::vector<int>) + 4 * sizeof(int));
    assert(estimateCodesMemoryBytes(1296, 4) == 1296 * one);

    std::vector<char> block(50 * 1024 * 1024, 1);
    std::size_t peak = peakMemoryBytes();
    assert(peak > 0);
    assert(block[0] == 1);

    std::cout << "Metrics: all tests passed (peak " << peak / (1024 * 1024) << " MB)\n";
    return 0;
}
