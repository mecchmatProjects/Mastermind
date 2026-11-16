#pragma once
#include "Code.h"

struct GuessResult {
    int exact; //correctPosition
    int partial; //wrongPosition
};

GuessResult evaluate(const Code& secret, const Code& guess);