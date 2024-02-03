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

    Tile* GetRandomInnerTile() const;
    
    int GetMinX() const;
    int GetMaxX() const;
    int GetMinY() const;
    int GetMaxY() const;

    Tile* GetRandomWallTile() const;

    bool IsCornerTile(Tile* tile) const;

    void SetIsConnected(bool connected);

    bool GetIsConnected() const;
    
private:
    std::vector<Tile*> RoomTiles;
    
    RoomType Type;

    bool IsConnected;
};
