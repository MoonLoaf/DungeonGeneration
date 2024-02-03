#include "Room.h"

#include <utility>

#include "../utils/Random.h"

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

Tile* Room::GetRandomInnerTile() const
{
    std::vector<Tile*> nonWallTiles;

    for (Tile* tile : RoomTiles) {
        if (tile->GetTileType() != TileType::Wall) {
            nonWallTiles.push_back(tile);
        }
    }

    if (nonWallTiles.empty()) {
        // No non-wall tiles in the room
        return nullptr;
    }

    // Randomly select a non-wall tile
    int randomIndex = Random::GetRandom(0, nonWallTiles.size() - 1);
    return nonWallTiles[randomIndex];
}

int Room::GetMinX() const {
    int minX = std::numeric_limits<int>::max();
    for (const Tile* tile : RoomTiles) {
        minX = std::min(minX, tile->GetGridPos().x);
    }
    return minX;
}

int Room::GetMaxX() const {
    int maxX = std::numeric_limits<int>::min();
    for (const Tile* tile : RoomTiles) {
        maxX = std::max(maxX, tile->GetGridPos().x);
    }
    return maxX;
}

int Room::GetMinY() const {
    int minY = std::numeric_limits<int>::max();
    for (const Tile* tile : RoomTiles) {
        minY = std::min(minY, tile->GetGridPos().y);
    }
    return minY;
}

int Room::GetMaxY() const {
    int maxY = std::numeric_limits<int>::min();
    for (const Tile* tile : RoomTiles) {
        maxY = std::max(maxY, tile->GetGridPos().y);
    }
    return maxY;
}

Tile* Room::GetRandomWallTile() const
{
    std::vector<Tile*> wallTiles;
    for (Tile* tile : RoomTiles) {
        if (tile->GetTileType() == TileType::Wall && !IsCornerTile(tile)) {
            wallTiles.push_back(tile);
        }
    }

    if (!wallTiles.empty()) {
        int randomIndex = Random::GetRandom(0, wallTiles.size() - 1);
        return wallTiles[randomIndex];
    }

    return nullptr;  // No eligible wall tiles found
}

bool Room::IsCornerTile(Tile* tile) const
{
    // Check if the tile is one of the corner tiles
    vector2 tilePos = tile->GetGridPos();
    int minX = GetMinX();
    int maxX = GetMaxX();
    int minY = GetMinY();
    int maxY = GetMaxY();

    return (tilePos.x == minX && tilePos.y == minY) ||
           (tilePos.x == maxX && tilePos.y == minY) ||
           (tilePos.x == minX && tilePos.y == maxY) ||
           (tilePos.x == maxX && tilePos.y == maxY);
}



void Room::SetIsConnected(const bool connected)
{
    IsConnected = connected;
}

bool Room::GetIsConnected() const
{
    return IsConnected;
}
