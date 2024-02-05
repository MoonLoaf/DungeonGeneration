#pragma once
#include <memory>
#include <vector>

#include "Tile.h"
#include "../utils/RoomType.h"

class Room
{
public:

    Room(std::vector<Tile*> tiles, RoomType type, const std::shared_ptr<std::vector<SDL_Surface*>>& images);

    std::vector<Tile*> GetRoomTiles();

    void SetRoomType(RoomType type);

    RoomType GetRoomType() const;

    Tile* GetRandomInnerTile() const;
    
    int GetMinX() const;
    int GetMaxX() const;
    int GetMinY() const;
    int GetMaxY() const;

    Tile* GetRandomWallTile() const;

    bool IsCornerTile(const Tile* tile) const;

    void SetIsConnected(bool connected);

    bool GetIsConnected() const;

    void DecorateRoom() const;

private:

    //Room specific decoration functions
    void DecorateNormalRoom() const;
    void DecorateBossRoom() const;
    void DecoratePickupRoom() const;
    
    std::vector<Tile*> RoomTiles;
    
    RoomType Type;

    bool IsConnected;

    std::shared_ptr<std::vector<SDL_Surface*>> Sprites;
};
