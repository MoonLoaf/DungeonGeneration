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
