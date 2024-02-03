#include "Grid.h"
#include "../utils/TileMappings.h"

#include <random>
#include <stack>
#include <utility>

#include "Room.h"

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
            GridTiles[i].emplace_back(position, scale, Sprites[9], Renderer, tileType, vector2{i, j});
        }
    }

    for (int i = 0; i < rooms; i++)
    {
        GenerateRoom();
    }
    ConnectRooms();
}

void Grid::GenerateRoom() {
    constexpr int minRoomSize = 5;
    constexpr int maxRoomSize = 15;

    RoomType type = RoomType::Normal;
    std::vector<Tile*> roomTiles;

    const int roomWidth = getRandom(minRoomSize, maxRoomSize);
    const int roomHeight = getRandom(minRoomSize, maxRoomSize);

    int roomX, roomY;
    do {
        // Generate a random position for the top-left corner of the room
        roomX = getRandom(0, Width - roomWidth);
        roomY = getRandom(0, Height - roomHeight);
    } while (IsRoomOverlap(roomX, roomY, roomWidth, roomHeight));

    // Assign tiles to the room
    for (int i = roomX; i < roomX + roomWidth; ++i) {
        for (int j = roomY; j < roomY + roomHeight; ++j) {
            TileType tileType;

            if (i == roomX && j == roomY) {
                tileType = TileType::Wall;
                GridTiles[i][j].SetTileType(tileType);
                GridTiles[i][j].SetTexture(Sprites[TOP_LEFT_CORNER_SPRITE]);
                roomTiles.push_back(&GridTiles[i][j]);
            } else if (i == roomX + roomWidth - 1 && j == roomY) {
                tileType = TileType::Wall;
                GridTiles[i][j].SetTileType(tileType);
                GridTiles[i][j].SetTexture(Sprites[TOP_RIGHT_CORNER_SPRITE]);
                roomTiles.push_back(&GridTiles[i][j]);
            } else if (i == roomX && j == roomY + roomHeight - 1) {
                tileType = TileType::Wall;
                GridTiles[i][j].SetTileType(tileType);
                GridTiles[i][j].SetTexture(Sprites[BOTTOM_LEFT_CORNER_SPRITE]);
                roomTiles.push_back(&GridTiles[i][j]);
            } else if (i == roomX + roomWidth - 1 && j == roomY + roomHeight - 1) {
                tileType = TileType::Wall;
                GridTiles[i][j].SetTileType(tileType);
                GridTiles[i][j].SetTexture(Sprites[BOTTOM_RIGHT_CORNER_SPRITE]);
                roomTiles.push_back(&GridTiles[i][j]);
            } else if (j == roomY) {
                tileType = TileType::Wall;
                GridTiles[i][j].SetTileType(tileType);
                GridTiles[i][j].SetTexture(Sprites[TOP_WALL_SPRITE]);
                roomTiles.push_back(&GridTiles[i][j]);
            } else if (j == roomY + roomHeight - 1) {
                tileType = TileType::Wall;
                GridTiles[i][j].SetTileType(tileType);
                GridTiles[i][j].SetTexture(Sprites[BOTTOM_WALL_SPRITE]);
                roomTiles.push_back(&GridTiles[i][j]);
            } else if (i == roomX) {
                tileType = TileType::Wall;
                GridTiles[i][j].SetTileType(tileType);
                GridTiles[i][j].SetTexture(Sprites[LEFT_SIDE_WALL_SPRITE]);
                roomTiles.push_back(&GridTiles[i][j]);
            } else if (i == roomX + roomWidth - 1) {
                tileType = TileType::Wall;
                GridTiles[i][j].SetTileType(tileType);
                GridTiles[i][j].SetTexture(Sprites[RIGHT_SIDE_WALL_SPRITE]);
                roomTiles.push_back(&GridTiles[i][j]);
            } else {
                // Inner part of the room
                const int randomIndex = getRandom(6, 7);
                tileType = TileType::Ground;
                GridTiles[i][j].SetTileType(tileType);
                GridTiles[i][j].SetTexture(Sprites[randomIndex]);
                roomTiles.push_back(&GridTiles[i][j]);
            }
        }
    }
    Room* newRoom = new Room(roomTiles, type);
    Rooms.push_back(newRoom);
}

void Grid::ConnectRooms() {
    for (size_t i = 0; i < Rooms.size() - 1; ++i) {
        ConnectTwoRooms(Rooms[i], Rooms[i + 1]);
    }
}

void Grid::ConnectTwoRooms(Room* room1, Room* room2) {
    Tile* tile1 = room1->GetRandomTile();
    Tile* tile2 = room2->GetRandomTile();

    vector2 startPos = tile1->GetGridPos();
    vector2 endPos = tile2->GetGridPos();

    // Simple pathfinding using a straight line (you can replace this with a more sophisticated algorithm if needed)
    while (startPos != endPos) {
        if (startPos.x < endPos.x) {
            startPos.x++;
        } else if (startPos.x > endPos.x) {
            startPos.x--;
        } else if (startPos.y < endPos.y) {
            startPos.y++;
        } else if (startPos.y > endPos.y) {
            startPos.y--;
        }

        TileType tileType = GridTiles[startPos.x][startPos.y].GetTileType();
        if (tileType == TileType::None) {
            GridTiles[startPos.x][startPos.y].SetTileType(TileType::Ground);
            GridTiles[startPos.x][startPos.y].SetTexture(Sprites[GROUND]);
            
        }
    }
}

bool Grid::IsValidPosition(int x, int y)
{
    return x >= 0 && x < Width && y >= 0 && y < Height;
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

int Grid::getRandom(const int min, const int max) {
    return min + rand() % (max - min + 1);
}