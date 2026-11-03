#pragma once
#include "CodeGenerator.h"

struct Feedback {
    int correctPosition;
    int wrongPosition;
};

Feedback calculateFeedback(const Code& secret, const Code& guess);