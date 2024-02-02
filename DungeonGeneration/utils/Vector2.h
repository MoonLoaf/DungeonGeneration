#pragma once

struct vector2 {
    int x;
    int y;

    // Equality operator
    bool operator==(const vector2& other) const {
        return x == other.x && y == other.y;
    }
};
