#include <random>
#include <utility>

#include "Grid.h"
#include "../utils/TileMappings.h"
#include "Room.h"
#include "../utils/Random.h"

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
            vector2 position {i * TileWidth + xOffset, j * TileHeight + yOffset};
            vector2 scale {TileWidth, TileHeight};
            TileType tileType = TileType::None;
            GridTiles[i].emplace_back(position, scale, Sprites[9], Renderer, tileType, vector2{i, j});
        }
    }

    for (int i = 0; i < rooms; i++)
    {
        GenerateRoom(6, 20);
    }
    ConnectRooms();
    GenerateDoors();
}

void Grid::GenerateRoom(const int minRoomSize, const int maxRoomSize) {

    RoomType type = RoomType::Normal;
    std::vector<Tile*> roomTiles;

    const int roomWidth = Random::GetRandom(minRoomSize, maxRoomSize);
    const int roomHeight = Random::GetRandom(minRoomSize, maxRoomSize);

    int roomX, roomY;
    do {
        // Generate a random position for the top-left corner of the room
        roomX = Random::GetRandom(0, Width - roomWidth);
        roomY = Random::GetRandom(0, Height - roomHeight);
    } while (IsRoomOverlap(roomX, roomY, roomWidth, roomHeight));

    // Assign tiles to the room
    for (int i = roomX; i < roomX + roomWidth; ++i) {
        for (int j = roomY; j < roomY + roomHeight; ++j) {

            if (i == roomX && j == roomY) {
                GridTiles[i][j].SetTileType(TileType::Wall);
                GridTiles[i][j].SetTexture(Sprites[TOP_LEFT_CORNER_SPRITE]);
                roomTiles.push_back(&GridTiles[i][j]);
            } else if (i == roomX + roomWidth - 1 && j == roomY) {
                GridTiles[i][j].SetTileType(TileType::Wall);
                GridTiles[i][j].SetTexture(Sprites[TOP_RIGHT_CORNER_SPRITE]);
                roomTiles.push_back(&GridTiles[i][j]);
            } else if (i == roomX && j == roomY + roomHeight - 1) {
                GridTiles[i][j].SetTileType(TileType::Wall);
                GridTiles[i][j].SetTexture(Sprites[BOTTOM_LEFT_CORNER_SPRITE]);
                roomTiles.push_back(&GridTiles[i][j]);
            } else if (i == roomX + roomWidth - 1 && j == roomY + roomHeight - 1) {
                GridTiles[i][j].SetTileType(TileType::Wall);
                GridTiles[i][j].SetTexture(Sprites[BOTTOM_RIGHT_CORNER_SPRITE]);
                roomTiles.push_back(&GridTiles[i][j]);
            } else if (j == roomY) {
                GridTiles[i][j].SetTileType(TileType::Wall);
                GridTiles[i][j].SetTexture(Sprites[TOP_WALL_SPRITE]);
                roomTiles.push_back(&GridTiles[i][j]);
            } else if (j == roomY + roomHeight - 1) {
                GridTiles[i][j].SetTileType(TileType::Wall);
                GridTiles[i][j].SetTexture(Sprites[BOTTOM_WALL_SPRITE]);
                roomTiles.push_back(&GridTiles[i][j]);
            } else if (i == roomX) {
                GridTiles[i][j].SetTileType(TileType::Wall);
                GridTiles[i][j].SetTexture(Sprites[LEFT_SIDE_WALL_SPRITE]);
                roomTiles.push_back(&GridTiles[i][j]);
            } else if (i == roomX + roomWidth - 1) {
                GridTiles[i][j].SetTileType(TileType::Wall);
                GridTiles[i][j].SetTexture(Sprites[RIGHT_SIDE_WALL_SPRITE]);
                roomTiles.push_back(&GridTiles[i][j]);
            } else {
                // Inner part of the room
                const int randomIndex = Random::GetRandom(6, 7);
                GridTiles[i][j].SetTileType(TileType::Ground);
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
    Tile* tile1 = room1->GetRandomInnerTile();
    Tile* tile2 = room2->GetRandomInnerTile();

    vector2 startPos = tile1->GetGridPos();
    vector2 endPos = tile2->GetGridPos();

    // Simple pathfinding using a straight line (you can replace this with a more sophisticated algorithm if needed)
    std::vector<vector2> path;
    vector2 currentPos = startPos;
    while (currentPos != endPos) {
        int dx = endPos.x - currentPos.x;
        int dy = endPos.y - currentPos.y;

        if (std::abs(dx) > std::abs(dy)) {
            currentPos.x += (dx > 0) ? 1 : -1;
        } else {
            currentPos.y += (dy > 0) ? 1 : -1;
        }

        // Check if the current position is within the valid range
        if (IsValidPosition(currentPos.x, currentPos.y)) {
            path.push_back(currentPos);  // Add the current position to the path
            GridTiles[currentPos.x][currentPos.y].SetTileType(TileType::Ground);
            GridTiles[currentPos.x][currentPos.y].SetTexture(Sprites[GROUND]);
        } else {
            // If the current position is out of bounds, break the loop
            break;
        }
    }

    // Iterate through the path and update neighboring tiles to walls
    for (const vector2& pos : path) {
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                int x = pos.x + i;
                int y = pos.y + j;

                if (IsValidPosition(x, y) && GridTiles[x][y].GetTileType() == TileType::None) {
                    GridTiles[x][y].SetTileType(TileType::Wall);
                    GridTiles[x][y].SetTexture(Sprites[24]);  // Replace WALL_SPRITE with the appropriate index
                }
            }
        }
    }
}

void Grid::GenerateDoors() const
{
    // Choose whether to connect rooms with highest and lowest X or Y values
    bool connectByX = Random::GetRandom(0, 1) == 0;

    // Find rooms with highest and lowest X or Y values
    Room* room1;
    Room* room2;

    if (connectByX) {
        room1 = GetRoomWithMinX();
        room2 = GetRoomWithMaxX();
    } else {
        room1 = GetRoomWithMinY();
        room2 = GetRoomWithMaxY();
    }

    // Replace a random wall tile in each room with a door sprite
    Tile* doorTile1 = room1->GetRandomWallTile();
    Tile* doorTile2 = room2->GetRandomWallTile();

    if (doorTile1 && doorTile2) {
        doorTile1->SetTileType(TileType::Door);
        doorTile1->SetTexture(Sprites[DOOR_1]);  // Replace DOOR_SPRITE with the appropriate index

        doorTile2->SetTileType(TileType::Door);
        doorTile2->SetTexture(Sprites[DOOR_2]);  // Replace DOOR_SPRITE with the appropriate index
    }
}

Room* Grid::GetRoomWithMinX() const {
    Room* roomMinX = nullptr;
    for (Room* room : Rooms) {
        if (!roomMinX || room->GetMinX() < roomMinX->GetMinX()) {
            roomMinX = room;
        }
    }
    return roomMinX;
}

Room* Grid::GetRoomWithMaxX() const {
    Room* roomMaxX = nullptr;
    for (Room* room : Rooms) {
        if (!roomMaxX || room->GetMaxX() > roomMaxX->GetMaxX()) {
            roomMaxX = room;
        }
    }
    return roomMaxX;
}

Room* Grid::GetRoomWithMinY() const {
    Room* roomMinY = nullptr;
    for (Room* room : Rooms) {
        if (!roomMinY || room->GetMinY() < roomMinY->GetMinY()) {
            roomMinY = room;
        }
    }
    return roomMinY;
}

Room* Grid::GetRoomWithMaxY() const {
    Room* roomMaxY = nullptr;
    for (Room* room : Rooms) {
        if (!roomMaxY || room->GetMaxY() > roomMaxY->GetMaxY()) {
            roomMaxY = room;
        }
    }
    return roomMaxY;
}



bool Grid::IsValidPosition(const int x, const int y) const
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

