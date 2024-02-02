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
    GenerateCorridors();
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

void Grid::GenerateCorridors()
{
    // Check if there are enough rooms to connect
    if (Rooms.size() < 2) {
        return;
    }

    for (size_t i = 0; i < Rooms.size() - 1; i++)
    {
        // Connect each room to the next one
        ConnectRooms(Rooms[i], Rooms[i + 1]);
    }
}

void Grid::ConnectRooms(Room* room1, Room* room2)
{
    // Choose a random tile from each room
    const Tile* tile1 = room1->GetRandomTile();
    const Tile* tile2 = room2->GetRandomTile();

    // Use DFS to connect the chosen tiles
    std::vector<std::vector<bool>> corridorMatrix(Width, std::vector<bool>(Height, false));
    ConnectTilesDFS(tile1->GetGridPos(), tile2->GetGridPos(), corridorMatrix);
}

void Grid::ConnectTilesDFS(const vector2& start, const vector2& end, std::vector<std::vector<bool>>& corridorMatrix)
{
    std::vector<std::vector<int>> distances(Width, std::vector<int>(Height, -1));
    std::vector<std::vector<vector2>> previous(Width, std::vector<vector2>(Height, { -1, -1 }));

    // Set the distance of the start position to 0
    distances[start.x][start.y] = 0;

    RecursiveDFS(start, end, corridorMatrix, distances, previous);
}


void Grid::RecursiveDFS(const vector2& current, const vector2& end, std::vector<std::vector<bool>>& corridorMatrix, std::vector<std::vector<int>>& distances, std::vector<std::vector<vector2>>& previous)
{
    if (current == end)
    {
        // Reconstruct the path
        std::vector<vector2> path;
        vector2 temp = end;

        while (temp != vector2{-1, -1})
        {
            path.push_back(temp);
            temp = previous[temp.x][temp.y];
        }

        // Update sprites for the path
        for (const auto& pos : path)
        {
            GridTiles[pos.x][pos.y].SetTileType(TileType::Ground);
            GridTiles[pos.x][pos.y].SetTexture(Sprites[GROUND]);
        }

        return;
    }

    if (corridorMatrix[current.x][current.y])
    {
        return; // Already part of the corridor
    }

    // Mark the tile as part of the corridor
    corridorMatrix[current.x][current.y] = true;

    // Update distances and previous for neighbors
    UpdateNeighbors(distances, previous, current);

    // Continue DFS for neighbors
    const int x = current.x;
    const int y = current.y;

    const std::vector<vector2> neighbors = {
        {x - 1, y},
        {x + 1, y},
        {x, y - 1},
        {x, y + 1}
    };

    for (const auto& neighbor : neighbors)
    {
        if (IsValidPosition(neighbor.x, neighbor.y) && distances[neighbor.x][neighbor.y] == -1)
        {
            RecursiveDFS(neighbor, end, corridorMatrix, distances, previous);
        }
    }
}

void Grid::UpdateNeighbors(std::vector<std::vector<int>>& distances, std::vector<std::vector<vector2>>& previous, const vector2& current)
{
    const int x = current.x;
    const int y = current.y;

    const std::vector<vector2> neighbors = {
        {x - 1, y},
        {x + 1, y},
        {x, y - 1},
        {x, y + 1}
    };

    for (const auto& neighbor : neighbors)
    {
        if (IsValidPosition(neighbor.x, neighbor.y) && distances[neighbor.x][neighbor.y] == -1)
        {
            distances[neighbor.x][neighbor.y] = distances[x][y] + 1;
            previous[neighbor.x][neighbor.y] = current;
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