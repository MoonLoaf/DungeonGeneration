#pragma once
#include <vector>

#include "Tile.h"
#include "../utils/RoomType.h"

class Room
{
public:

    Room(std::vector<Tile*> tiles, RoomType type);

    std::vector<Tile*> GetRoomTiles();

    void SetRoomType(RoomType type);

    RoomType GetRoomType() const;
    
private:

    std::vector<Tile*> RoomTiles;
    
    RoomType Type;
};
