#include "Room.h"

#include <utility>

Room::Room(std::vector<Tile*> tiles, const RoomType type)
{
    RoomTiles = std::move(tiles);
    Type = type;
}

std::vector<Tile*> Room::GetRoomTiles()
{
    return RoomTiles;
}

void Room::SetRoomType(const RoomType type)
{
    Type = type;
}

RoomType Room::GetRoomType() const
{
    return Type;
}

Tile* Room::GetRandomTile()
{
    if (!RoomTiles.empty()) {
        const int r = rand() % RoomTiles.size();
        return RoomTiles[r];
    } else {
        // Handle the case when RoomTiles is empty (return nullptr or handle accordingly)
        return nullptr;
    }
}