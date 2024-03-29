#include "Room.h"

#include <utility>

#include "Grid.h"
#include "../utils/ExitColor.h"
#include "../utils/Random.h"
#include "../utils/TileMappings.h"

Room::Room(std::vector<Tile*> tiles, const RoomType type, const std::shared_ptr<std::vector<SDL_Surface*>>& images, Grid* grid)
{
    RoomTiles = std::move(tiles);
    Type = type;
    Sprites = images;
    GridPtr = grid;  
}


/**
 * @brief Retrieves a random non-wall tile from the room.
 * 
 * @return Pointer to a Tile object representing a random non-wall tile, or nullptr if no such tile is found.
 */
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
    const int r = Random::GetRandomRange(0, nonWallTiles.size() - 1);
    return nonWallTiles[r];
}

/**
 * @brief Retrieves a random wall tile from the room.
 * 
 * @return Pointer to a Tile object representing a random wall tile, or nullptr if no such tile is found.
 */
Tile* Room::GetRandomWallTile() const
{
    std::vector<Tile*> wallTiles;
    for (Tile* tile : RoomTiles) {
        if (tile->GetTileType() == TileType::Wall && !IsCornerTile(tile)) {
            wallTiles.push_back(tile);
        }
    }

    if (!wallTiles.empty()) {
        const int r = Random::GetRandomRange(0, wallTiles.size() - 1);
        return wallTiles[r];
    }

    return nullptr;  // No eligible wall tiles found
}

/**
 * @brief Checks if a tile is located at one of the corners of the room.
 * 
 * @param tile Pointer to the Tile object to be checked.
 * @return True if the tile is a corner tile, false otherwise.
 */
bool Room::IsCornerTile(const Tile* tile) const
{
    // Check if the tile is one of the corner tiles
    const vector2 tilePos = tile->GetGridPos();
    const int minX = GetMinX();
    const int maxX = GetMaxX();
    const int minY = GetMinY();
    const int maxY = GetMaxY();

    return (tilePos.x == minX && tilePos.y == minY) ||
           (tilePos.x == maxX && tilePos.y == minY) ||
           (tilePos.x == minX && tilePos.y == maxY) ||
           (tilePos.x == maxX && tilePos.y == maxY);
}

/**
 * @brief Decorates the room based on its type.
 */
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
        case RoomType::KeyPickup:
            DecorateKeyRoom();
            break;
    }
}

void Room::DecorateNormalRoom() const
{
    const int decorAmount = RoomTiles.size() / 35;

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
                const int r = Random::GetRandomRange(ROOM_DECOR_START, ROOM_DECOR_END);
                tile->SetTexture(Sprites->at(r));
                tile->SetTileType(TileType::Decor);
            }
            else
            {
                const int r = Random::GetRandomRange(LIGHTING_DECOR_START, LIGHTING_DECOR_END);
                tile->SetTexture(Sprites->at(r));
                tile->SetTileType(TileType::Decor);
            }
        }
    }
}

void Room::DecorateBossRoom() const
{
    const int decorAmount = RoomTiles.size() / 35;

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
            const int r = Random::GetRandomRange(ENEMY_START, ENEMY_END);
            tile->SetTexture(Sprites->at(r));
            tile->SetTileType(TileType::Boss);
        }
    }
}

void Room::DecoratePickupRoom() const
{
    const int decorAmount = RoomTiles.size() / 35;

    for (int i = 0; i < decorAmount; i++) {
        Tile* tile = nullptr;
        int attempts = 0;

        // Keep trying until a valid ground tile is found or maximum attempts are reached
        while (attempts < 20) {
            tile = GetRandomInnerTile();
            
            // Check if a valid ground tile is found
            if (tile && tile->GetTileType() == TileType::Ground) {
                break;
            }

            attempts++;
        }

        if (tile) {
            if (Random::GetRandomRange(0, 1) == 0) {
                // Decorate with health pickups
                const int r = Random::GetRandomRange(HEALTH_PICKUP_START, HEALTH_PICKUP_END);
                tile->SetTexture(Sprites->at(r));
                tile->SetTileType(TileType::Pickup);
            } else {
                ExitColor exitColor = GridPtr->GetDungeonExitColor();
                int keyTextureIndex;

                // Randomly select a key color that is different from the exit color
                do {
                    keyTextureIndex = Random::GetRandomRange(KEY_RED, KEY_YELLOW);
                } while (keyTextureIndex == static_cast<int>(exitColor));
                
                tile->SetTexture(Sprites->at(keyTextureIndex));
                tile->SetTileType(TileType::Pickup);
            }
        }
    }
}

void Room::DecorateKeyRoom() const
{
    Tile* keyTile = nullptr;
    int attempts = 0;

    // Keep trying until a valid ground tile is found or maximum attempts are reached
    while (attempts < 50) {
        keyTile = GetRandomInnerTile();
        
        // Check if a valid ground tile is found
        if (keyTile && keyTile->GetTileType() == TileType::Ground) {
            break;
        }

        attempts++;
    }

    // Check if a valid ground tile was found
    if (keyTile) {
        ExitColor exitColor = GridPtr->GetDungeonExitColor();

        // Assign the key texture index based on the exit color
        switch (exitColor)
        {
            case ExitColor::RedExit:
                keyTile->SetTexture(Sprites->at(KEY_RED));
                keyTile->SetTileType(TileType::RedKey);
                break;
            case ExitColor::BlueExit:
                keyTile->SetTexture(Sprites->at(KEY_BLUE));
                keyTile->SetTileType(TileType::BlueKey);
                break;
            case ExitColor::YellowExit:
                keyTile->SetTexture(Sprites->at(KEY_YELLOW));
                keyTile->SetTileType(TileType::YellowKey);
                break;
        }
    }

    // Decorate the room with other pickups and decorations
    const int decorAmount = RoomTiles.size() / 35;

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
            // Decorate the tile with pickups or other decorations
            if (Random::GetRandomRange(0, 1) == 0) {
                const int r = Random::GetRandomRange(HEALTH_PICKUP_START, HEALTH_PICKUP_END);
                tile->SetTexture(Sprites->at(r));
                tile->SetTileType(TileType::Pickup);
            } else {
                const int r = Random::GetRandomRange(ROOM_DECOR_START, ROOM_DECOR_END);
                tile->SetTexture(Sprites->at(r));
                tile->SetTileType(TileType::Pickup);
            }
        }
    }
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


//Gets min and max X and Y values of the tiles in the room instance.

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
