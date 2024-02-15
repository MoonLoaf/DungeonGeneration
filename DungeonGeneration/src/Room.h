#pragma once
#include <memory>
#include <vector>

#include "Tile.h"
#include "../utils/RoomType.h"

class Room
{
public:

    Room(std::vector<Tile*> tiles, const RoomType type, const std::shared_ptr<std::vector<SDL_Surface*>>& images, class Grid* grid);

    ~Room() = default; 

    std::vector<Tile*> GetRoomTiles();

    void SetRoomType(const RoomType type);

    [[nodiscard]] RoomType GetRoomType() const;

    [[nodiscard]] Tile* GetRandomInnerTile() const;


    [[nodiscard]] Tile* GetRandomWallTile() const;

    bool IsCornerTile(const Tile* tile) const;

    void DecorateRoom() const;
    
    [[nodiscard]] int GetMinX() const;
    [[nodiscard]] int GetMaxX() const;
    [[nodiscard]] int GetMinY() const;
    [[nodiscard]] int GetMaxY() const;

private:

    //Room specific decoration functions
    void DecorateNormalRoom() const;
    void DecorateBossRoom() const;
    void DecoratePickupRoom() const;
    void DecorateKeyRoom() const;
    
    std::shared_ptr<std::vector<SDL_Surface*>> Sprites;
    
    std::vector<Tile*> RoomTiles;

    RoomType Type;

    Grid* GridPtr;
};
