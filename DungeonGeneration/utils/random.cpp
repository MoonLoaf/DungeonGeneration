#include "Random.h"

#include <cstdlib>

int Random::GetRandom(const int min, const int max) {
    return min + rand() % (max - min + 1);
}
