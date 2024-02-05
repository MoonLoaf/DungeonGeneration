#include "Room.h"

#include <utility>

#include "../utils/Random.h"
#include "../utils/TileMappings.h"

Room::Room(std::vector<Tile*> tiles, const RoomType type, const std::shared_ptr<std::vector<SDL_Surface*>>& images)
{
    RoomTiles = std::move(tiles);
    Type = type;
    Sprites = images;
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
    int randomIndex = Random::GetRandomRange(0, nonWallTiles.size() - 1);
    return nonWallTiles[randomIndex];
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
        int randomIndex = Random::GetRandomRange(0, wallTiles.size() - 1);
        return wallTiles[randomIndex];
    }

    return nullptr;  // No eligible wall tiles found
}

bool Room::IsCornerTile(const Tile* tile) const
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

void Room::DecorateRoom() const
{
    switch (Type) {
        case RoomType::Normal:
            DecorateNormalRoom();
            break;
        case RoomType::Boss:
            DecorateBossRoom();
            break;
        case RoomType::Pickup:
            DecoratePickupRoom();
            break;
    }
}

void Room::DecorateNormalRoom() const
{
    int decorAmount = Random::GetRandomRange(2, 5);

    for (int i = 0; i < decorAmount; i++) {
        Tile* tile = nullptr;
        int attempts = 0;

        // Keep trying until a valid ground tile is found or maximum attempts are reached
        while (attempts < 10) {
            tile = GetRandomInnerTile();
            
            // Check if a valid ground tile is found
            if (tile && tile->GetTileType() == TileType::Ground) {
                break;
            }

            attempts++;
        }

        // Check if a valid ground tile was found
        if (tile) {
            int r = Random::GetRandomRange(STONES_RANGE_START, STONES_RANGE_END);
            tile->SetTexture(Sprites->at(r));
            tile->SetTileType(TileType::Decor);
        }
    }
}

void Room::DecorateBossRoom() const
{
    int decorAmount = Random::GetRandomRange(1, 2);

    for (int i = 0; i < decorAmount; i++) {
        Tile* tile = nullptr;
        int attempts = 0;

        // Keep trying until a valid ground tile is found or maximum attempts are reached
        while (attempts < 10) {
            tile = GetRandomInnerTile();
            
            // Check if a valid ground tile is found
            if (tile && tile->GetTileType() == TileType::Ground) {
                break;
            }

            attempts++;
        }

        // Check if a valid ground tile was found
        if (tile) {
            int r = Random::GetRandomRange(ENEMY_START, ENEMY_END);
            tile->SetTexture(Sprites->at(r));
            tile->SetTileType(TileType::Boss);
        }
    }
}

void Room::DecoratePickupRoom() const
{
    int decorAmount = Random::GetRandomRange(1, 2);

    for (int i = 0; i < decorAmount; i++) {
        Tile* tile = nullptr;
        int attempts = 0;

        // Keep trying until a valid ground tile is found or maximum attempts are reached
        while (attempts < 10) {
            tile = GetRandomInnerTile();
            
            // Check if a valid ground tile is found
            if (tile && tile->GetTileType() == TileType::Ground) {
                break;
            }

            attempts++;
        }

        // Check if a valid ground tile was found
        if (tile) {
            if(Random::GetRandomRange(0, 1) == 0)
            {
                int r = Random::GetRandomRange(HEALTH_PICKUP_START, HEALTH_PICKUP_START);
                tile->SetTexture(Sprites->at(r));
                tile->SetTileType(TileType::Pickup);
            }
            else
            {
                int r = Random::GetRandomRange(KEY_PICKUP_START, KEY_PICKUP_END);
                tile->SetTexture(Sprites->at(r));
                tile->SetTileType(TileType::Pickup);
            }
        }
    }
}

void Room::SetIsConnected(const bool connected)
{
    IsConnected = connected;
}

bool Room::GetIsConnected() const
{
    return IsConnected;
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
