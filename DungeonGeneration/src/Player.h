#pragma once

class Player {

public:
    
    Player(int startX, int startY, class Grid* grid);

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    
private:
    
    int x;
    int y;
    Grid* gridRef;
};
