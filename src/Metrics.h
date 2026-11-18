#pragma once
#include <chrono>
#include <cstddef>

class Timer {
public:
    Timer();
    void reset();
    double elapsedMs() const;

private:
    std::chrono::steady_clock::time_point start;
};

std::size_t estimateCodesMemoryBytes(std::size_t codesCount, int codeLength);
std::size_t peakMemoryBytes();
