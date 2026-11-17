#include "Metrics.h"
#include <vector>

#if defined(__unix__) || defined(__APPLE__)
#include <sys/resource.h>
#endif

Timer::Timer() : start(std::chrono::steady_clock::now()) {}

void Timer::reset() {
    start = std::chrono::steady_clock::now();
}

double Timer::elapsedMs() const {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(now - start).count();
}

std::size_t estimateCodesMemoryBytes(std::size_t codesCount, int codeLength) {
    std::size_t perCode = sizeof(std::vector<int>) + static_cast<std::size_t>(codeLength) * sizeof(int);
    return codesCount * perCode;
}

std::size_t peakMemoryBytes() {
#if defined(__unix__) || defined(__APPLE__)
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) != 0) return 0;
#if defined(__APPLE__)
    return static_cast<std::size_t>(usage.ru_maxrss);
#else
    return static_cast<std::size_t>(usage.ru_maxrss) * 1024;
#endif
#else
    return 0;
#endif
}
