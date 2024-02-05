#include <random>

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
    
    Sprites = std::make_shared<std::vector<SDL_Surface*>>(std::move(images));

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
            GridTiles[i].emplace_back(position, scale, Sprites->at(9), Renderer, tileType, vector2{i, j});
        }
    }

    for (int i = 0; i < rooms; i++)
    {
        GenerateRoom(5, 20);
    }
    ConnectRooms();
    GenerateDoors();
}

void Grid::GenerateRoom(const int minRoomSize, const int maxRoomSize) {

    const int roomWidth = Random::GetRandomRange(minRoomSize, maxRoomSize);
    const int roomHeight = Random::GetRandomRange(minRoomSize, maxRoomSize);

    if(GridTiles.size() - roomHeight * roomWidth <= 0)
    {
        //Room to big to generate
        printf("Skipped room, too many tiles");
        return;
    }
    
    RoomType type = DecideRoomType();
    std::vector<Tile*> roomTiles;

    int roomX, roomY;
    int attempts = 0;
    do {
        // Generate a random position for the top-left corner of the room
        roomX = Random::GetRandomRange(0, Width - roomWidth);
        roomY = Random::GetRandomRange(0, Height - roomHeight);
        attempts++;

        if (attempts >= 100) //Change to however many attempts is reasonable
        {
            printf("Skipped room, could not find non-overlapping tiles");
            return;
        }
    } while (IsRoomOverlap(roomX, roomY, roomWidth, roomHeight));

    // Assign tiles to the room
    for (int i = roomX; i < roomX + roomWidth; ++i) {
        for (int j = roomY; j < roomY + roomHeight; ++j) {

            if (i == roomX && j == roomY) {
                GridTiles[i][j].SetTileType(TileType::Wall);
                GridTiles[i][j].SetTexture(Sprites->at(TOP_LEFT_CORNER_SPRITE));
                roomTiles.push_back(&GridTiles[i][j]);
            } else if (i == roomX + roomWidth - 1 && j == roomY) {
                GridTiles[i][j].SetTileType(TileType::Wall);
                GridTiles[i][j].SetTexture(Sprites->at(TOP_RIGHT_CORNER_SPRITE));
                roomTiles.push_back(&GridTiles[i][j]);
            } else if (i == roomX && j == roomY + roomHeight - 1) {
                GridTiles[i][j].SetTileType(TileType::Wall);
                GridTiles[i][j].SetTexture(Sprites->at(BOTTOM_LEFT_CORNER_SPRITE));
                roomTiles.push_back(&GridTiles[i][j]);
            } else if (i == roomX + roomWidth - 1 && j == roomY + roomHeight - 1) {
                GridTiles[i][j].SetTileType(TileType::Wall);
                GridTiles[i][j].SetTexture(Sprites->at(BOTTOM_RIGHT_CORNER_SPRITE));
                roomTiles.push_back(&GridTiles[i][j]);
            } else if (j == roomY) {
                GridTiles[i][j].SetTileType(TileType::Wall);
                GridTiles[i][j].SetTexture(Sprites->at(TOP_WALL_SPRITE));
                roomTiles.push_back(&GridTiles[i][j]);
            } else if (j == roomY + roomHeight - 1) {
                GridTiles[i][j].SetTileType(TileType::Wall);
                GridTiles[i][j].SetTexture(Sprites->at(BOTTOM_WALL_SPRITE));
                roomTiles.push_back(&GridTiles[i][j]);
            } else if (i == roomX) {
                GridTiles[i][j].SetTileType(TileType::Wall);
                GridTiles[i][j].SetTexture(Sprites->at(LEFT_SIDE_WALL_SPRITE));
                roomTiles.push_back(&GridTiles[i][j]);
            } else if (i == roomX + roomWidth - 1) {
                GridTiles[i][j].SetTileType(TileType::Wall);
                GridTiles[i][j].SetTexture(Sprites->at(RIGHT_SIDE_WALL_SPRITE));
                roomTiles.push_back(&GridTiles[i][j]);
            } else {
                // Inner part of the room
                const int randomIndex = Random::GetRandomRange(6, 7);
                GridTiles[i][j].SetTileType(TileType::Ground);
                GridTiles[i][j].SetTexture(Sprites->at(randomIndex));
                roomTiles.push_back(&GridTiles[i][j]);
            }
        }
    }
    Room* newRoom = new Room(roomTiles, type, Sprites);
    Rooms.push_back(newRoom);
    newRoom->DecorateRoom();
}

void Grid::ConnectRooms() {
    for (size_t i = 0; i < Rooms.size() - 1; ++i) {
        ConnectTwoRooms(Rooms[i], Rooms[i + 1]);
    }
}

void Grid::ConnectTwoRooms(const Room* room1, const Room* room2) {
    Tile* tile1 = room1->GetRandomInnerTile();
    Tile* tile2 = room2->GetRandomInnerTile();

    vector2 startPos = tile1->GetGridPos();
    vector2 endPos = tile2->GetGridPos();

    // Simple pathfinding using a straight line
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
            GridTiles[currentPos.x][currentPos.y].SetTexture(Sprites->at(GROUND));
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
                    GridTiles[x][y].SetTexture(Sprites->at(24));
                }
            }
        }
    }
}

void Grid::GenerateDoors() const
{
    // Choose whether to connect rooms with highest and lowest X or Y values
    bool connectByX = Random::GetRandomRange(0, 1) == 0;

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
        doorTile1->SetTexture(Sprites->at(DOOR_1)); 

        doorTile2->SetTileType(TileType::Door);
        doorTile2->SetTexture(Sprites->at(DOOR_2));
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

RoomType Grid::DecideRoomType() {
    // Generate at least one Boss room
    if (Rooms.empty() || std::ranges::none_of(Rooms, [](const Room* room) {
        return room->GetRoomType() == RoomType::Boss;
    })) {
        return RoomType::Boss;
    }
    // Generate at least one Pickup room
    if (Rooms.empty() || std::ranges::none_of(Rooms, [](const Room* room) {
        return room->GetRoomType() == RoomType::Pickup;
    })) {
        return RoomType::Pickup;
    }
    // 25% chance for any other room to be Boss or Pickup
    if (Random::GetRandomRange(1, 100) <= 25) {
        return Random::GetRandomRange(1, 2) == 1 ? RoomType::Boss : RoomType::Pickup;
    }
    // Else normal room
    return RoomType::Normal;
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

