#pragma once

#include "../utils/Vector2.h"

class Player {

public:
    
    Player(int startX, int startY, class Grid* grid);

    void Move(const vector2& direction);

    vector2 GetStartPos() const;
    
private:
    
    int x;
    int y;

    vector2 StartPos;
    
    Grid* gridRef;
};
