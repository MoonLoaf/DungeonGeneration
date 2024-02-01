#include "Grid.h"

#include <random>
#include <utility>

Grid::Grid(int width, int height, int tileWidth, int tileHeight, SDL_Renderer* renderer, std::vector<SDL_Surface*> images, int inXOffset, int inYOffset)
{
    Width = width;
    Height = height;
    TileWidth = tileWidth;
    TileHeight = tileHeight;
    Renderer = renderer;
    Sprites = std::move(images);

    xOffset = inXOffset;
    yOffset = inYOffset;

    std::random_device rd;
    std::srand(rd());
}

void Grid::Initialize(int rooms)
{
    GridTiles.reserve(Width);

    for (int i = 0; i < Width; ++i) {
        GridTiles.emplace_back();
        GridTiles[i].reserve(Height);

        for (int j = 0; j < Height; ++j) {
            vector2 position(i * TileWidth + xOffset, j * TileHeight + yOffset);
            vector2 scale(TileWidth, TileHeight);
            TileType tileType = TileType::None;
            GridTiles[i].emplace_back(position, scale, Sprites[23], Renderer, tileType);
        }
    }

    for (int i = 0; i < rooms; i++)
    {
        GenerateRoom();
    }
}

void Grid::GenerateRoom() {
    const int minRoomSize = 5;
    const int maxRoomSize = 15;

    int roomWidth = getRandom(minRoomSize, maxRoomSize);
    int roomHeight = getRandom(minRoomSize, maxRoomSize);

    int roomX, roomY;
    do {
        // Generate a random position for the top-left corner of the room
        roomX = getRandom(0, Width - roomWidth);
        roomY = getRandom(0, Height - roomHeight);
    } while (IsRoomOverlap(roomX, roomY, roomWidth, roomHeight));

    // Assign ground tiles to the room
    for (int i = roomX; i < roomX + roomWidth; ++i) {
        for (int j = roomY; j < roomY + roomHeight; ++j) {
            const vector2 position(i * TileWidth + xOffset, j * TileHeight + yOffset);
            const vector2 scale(TileWidth, TileHeight);
            TileType tileType;

            // Assign ground tiles (Sprites[0-2]) to the inner part of the room
            if (i > roomX && i < roomX + roomWidth - 1 && j > roomY && j < roomY + roomHeight - 1) {
                const int randomIndex = getRandom(0, 1);
                tileType = TileType::Ground;
                GridTiles[i][j] = Tile(position, scale, Sprites[randomIndex], Renderer, tileType);
            }
            // Assign top wall tiles (Sprites[3]) to the top row of the room TODO: Need to account for l and r side of room
            else if (j == roomY) {
                tileType = TileType::Wall;
                GridTiles[i][j] = Tile(position, scale, Sprites[3], Renderer, tileType);
            }
            // Assign side wall tiles (Sprites[9]) to the sides of the room //TODO: also needs l and r specific sprites
            else if (i == roomX || i == roomX + roomWidth - 1) {
                tileType = TileType::Wall;
                GridTiles[i][j] = Tile(position, scale, Sprites[9], Renderer, tileType);
            }
        }
    }
}

bool Grid::IsRoomOverlap(int x, int y, int width, int height) const {
    for (int i = x; i < x + width; ++i) {
        for (int j = y; j < y + height; ++j) {
            // Check if the current cell is already occupied
            if (GridTiles[i][j].GetTileType() != TileType::None) {
                return true; // Overlapping
            }
        }
    }
    return false; // Not overlapping
}

std::vector<std::vector<Tile>> Grid::GetGridTiles()
{
    return GridTiles;
}

int Grid::getRandom(int min, int max) {
    return min + rand() % (max - min + 1);
}