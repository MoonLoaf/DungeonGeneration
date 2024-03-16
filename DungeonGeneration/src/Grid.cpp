#include <random>

#include "Grid.h"
#include "../utils/TileMappings.h"
#include "Room.h"
#include "../utils/Random.h"

Grid::Grid(const int width, const int height, const int tileWidth, const int tileHeight, SDL_Renderer* renderer, std::vector<SDL_Surface*> images, const int inXOffset, const int inYOffset)
{
    Width = width;
    Height = height;
    TileWidth = tileWidth;
    TileHeight = tileHeight;
    Renderer = renderer;
    
    Sprites = std::make_shared<std::vector<SDL_Surface*>>(std::move(images));

    xOffset = inXOffset;
    yOffset = inYOffset;

    //Get random seed for each startup
    std::random_device rd;
    std::srand(rd());
}

/**
 * @brief Initializes the grid by creating tiles and generating rooms.
 * 
 * Initializes the grid by creating tiles, generating rooms, connecting them, 
 * generating doors between rooms, and decorating the outer world.
 * 
 * @param rooms The number of rooms to generate.
 */
void Grid::Initialize(const int rooms)
{
    RoomCount = rooms;
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

    for (int i = 0; i < RoomCount; i++)
    {
        GenerateRoom(5, 20);
    }
    GenerateDoors();
    for (const auto& room : Rooms)
    {
        room->DecorateRoom();
    }
    ConnectAllRooms();
    DecorateOuterWorld();
}

void Grid::RenderGrid() const
{
    for (const auto& row : GridTiles)
    {
        for (auto tile : row)
        {
            tile.Draw();
        }
    }
}

/**
 * @brief Generates a room within the grid.
 * 
 * Generates a room with random dimensions and position within the grid, 
 * assigns tiles to the room, and adds it to the list of rooms.
 * 
 * @param minRoomSize The minimum size of the room in tiles.
 * @param maxRoomSize The maximum size of the room in tiles.
 */
void Grid::GenerateRoom(const int minRoomSize, const int maxRoomSize) {

    const int roomWidth = Random::GetRandomRange(minRoomSize, maxRoomSize);
    const int roomHeight = Random::GetRandomRange(minRoomSize, maxRoomSize);

    if(GridTiles.size() - roomHeight * roomWidth <= 0)
    {
        //Room to big to generate
        printf("Skipped room, too many tiles");
        return;
    }
    
    const RoomType type = DecideRoomType();
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
            if (j == roomY || j == roomY + roomHeight - 1 || i == roomX || i == roomX + roomWidth - 1) {
                GridTiles[i][j].SetTileType(TileType::Wall);
                GridTiles[i][j].SetTexture(Sprites->at(PLAIN_WALL));
                roomTiles.push_back(&GridTiles[i][j]);
            } else {
                const int r = Random::GetRandomRange(GROUND_1, GROUND_2);
                GridTiles[i][j].SetTileType(TileType::Ground);
                GridTiles[i][j].SetTexture(Sprites->at(r));
                roomTiles.push_back(&GridTiles[i][j]);
            }
        }
    }
    
    Rooms.push_back(std::make_unique<Room>(roomTiles, type, Sprites, this));
}

void Grid::ConnectAllRooms() {
    for (size_t i = 0; i < Rooms.size() - 1; ++i) {
        ConnectTwoRooms(Rooms[i], Rooms[i + 1]);
    }
}

/**
 * @brief Connects two rooms with a path.
 * 
 * Connects two rooms by creating a path between them using a simple 
 * straight-line pathfinding algorithm.
 * 
 * @param room1 Pointer to the first room.
 * @param room2 Pointer to the second room.
 */
void Grid::ConnectTwoRooms(const std::unique_ptr<Room>& room1, const std::unique_ptr<Room>& room2) {
    const Tile* tile1 = room1->GetRandomInnerTile();
    const Tile* tile2 = room2->GetRandomInnerTile();

    const vector2 startPos = tile1->GetGridPos();
    const vector2 endPos = tile2->GetGridPos();

    // Simple pathfinding using a straight line
    std::vector<vector2> path;
    vector2 currentPos = startPos;
    
    while (currentPos != endPos) {
        // Calculate the horizontal and vertical distances between current position and end position
        const int dx = endPos.x - currentPos.x;
        const int dy = endPos.y - currentPos.y;

        // Determine the direction to move based on the absolute distances
        if (std::abs(dx) > std::abs(dy)) {
            // If horizontal distance is greater, move horizontally
            currentPos.x += (dx > 0) ? 1 : -1;
        } else {
            // If vertical distance is greater or equal, move vertically
            currentPos.y += (dy > 0) ? 1 : -1;
        }

        // Check if the current position is within the valid range
        if (IsValidPosition(currentPos.x, currentPos.y) && !IsKeyTile(&GridTiles[currentPos.x][currentPos.y])) {
            path.push_back(currentPos);  // Add the current position to the path
            GridTiles[currentPos.x][currentPos.y].SetTileType(TileType::Ground);
            GridTiles[currentPos.x][currentPos.y].SetTexture(Sprites->at(GROUND_2));
        } else {
            // If the current position is out of bounds, break the loop
            break;
        }
    }

    // Iterate through the path and update neighboring tiles to walls
    for (const vector2& pos : path) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                const int x = pos.x + i;
                const int y = pos.y + j;

                if (IsValidPosition(x, y) && GridTiles[x][y].GetTileType() == TileType::None) {
                    GridTiles[x][y].SetTileType(TileType::Wall);
                    GridTiles[x][y].SetTexture(Sprites->at(PLAIN_WALL));
                }
            }
        }
    }
}


/**
 * @brief Generates doors between connected rooms.
 * 
 * Generates entry and exit doors in the dungeon in the rooms with highest and lowest X or Y values in the grid.
 */
void Grid::GenerateDoors()
{
    // Choose whether to connect rooms with highest and lowest X or Y values
    const bool connectByX = Random::GetRandomRange(0, 1) == 0;

    // Find rooms with highest and lowest X or Y values
    std::unique_ptr<Room> room1;
    std::unique_ptr<Room> room2;

    if (connectByX) {
        room1 = std::move(GetRoomWithMinX());
        room2 = std::move(GetRoomWithMaxX());
    } else {
        room1 = std::move(GetRoomWithMinY());
        room2 = std::move(GetRoomWithMaxY());
    }

    // Replace a random wall tile in each room with a door sprite
    Tile* doorTile1 = room1->GetRandomWallTile();
    Tile* doorTile2 = room2->GetRandomWallTile();

    if (doorTile1 && doorTile2) {
        doorTile1->SetTileType(TileType::DoorEntry);
        doorTile1->SetTexture(Sprites->at(DOOR_1)); 

        doorTile2->SetTileType(TileType::DoorExitLocked);
        const int r = Random::GetRandomRange(DOOR_YELLOW, DOOR_RED);
        doorTile2->SetTexture(Sprites->at(r));
        ExitDoorTile = doorTile2;

        switch (r)
        {
        case DOOR_YELLOW:
            ExitDoorColor = ExitColor::YellowExit;
            break;
        case DOOR_BLUE:
            ExitDoorColor = ExitColor::BlueExit;
            break;
        case DOOR_RED:
            ExitDoorColor = ExitColor::RedExit;
            break;
        }

        SpawnPlayerNearDoor(std::move(room1), doorTile1);
    }
}

void Grid::SpawnPlayerNearDoor(std::unique_ptr<Room> room, const Tile* doorTile) {
    // Check adjacent tiles for ground tiles
    for (int xOffset = -1; xOffset <= 1; ++xOffset) {
        for (int yOffset = -1; yOffset <= 1; ++yOffset) {
            const int x = doorTile->GetGridPos().x + xOffset;
            const int y = doorTile->GetGridPos().y + yOffset;

            // Ensure the adjacent tile is within bounds and is a ground tile
            if (IsValidPosition(x, y) && GridTiles[x][y].GetTileType() == TileType::Ground) {
                // Set the player's position to the ground tile adjacent to the door

                CurrentPlayer = std::make_unique<Player>(x, y, this);
                
                GridTiles[x][y].SetTileType(TileType::Player);
                GridTiles[x][y].SetTexture(Sprites->at(PLAYER));

                return;
            }
        }
    }
}

bool Grid::IsKeyTile(const Tile* tile)
{
    return tile->GetTileType() == TileType::BlueKey || tile->GetTileType() == TileType::YellowKey || tile->GetTileType() == TileType::RedKey;
}

/**
 * @brief Decorates the outer world of the grid.
 * 
 * Adds decorative elements to the outer world of the grid.
 */
void Grid::DecorateOuterWorld()
{
    for (int i = 0; i < Width; ++i) {
        for (int j = 0; j < Height; ++j) {
            if (GridTiles[i][j].GetTileType() == TileType::None && Random::GetRandomRange(1, 100) <= 4) {
                const int r = Random::GetRandomRange(OUTER_WORLD_DECOR_START, OUTER_WORLD_DECOR_END);
                GridTiles[i][j].SetTexture(Sprites->at(r));
            }
        }
    }
}

void Grid::UpdatePlayerPosition(int prevX, int prevY, int newX, int newY) {
    // Set the previous position tile back to its original state
    GridTiles[prevX][prevY].SetTileType(TileType::Ground);
    GridTiles[prevX][prevY].SetTexture(Sprites->at(GROUND_2));

    // Set the new position tile to represent the player
    GridTiles[newX][newY].SetTileType(TileType::Player);
    GridTiles[newX][newY].SetTexture(Sprites->at(PLAYER));
}

std::unique_ptr<Room> Grid::GetRoomWithMinX() const {
    std::unique_ptr<Room> roomMinX = nullptr;
    for (const auto& room : Rooms) {
        if (!roomMinX || room->GetMinX() < roomMinX->GetMinX()) {
            roomMinX = std::make_unique<Room>(*room);
        }
    }
    return roomMinX;
}

std::unique_ptr<Room> Grid::GetRoomWithMaxX() const {
    std::unique_ptr<Room> roomMaxX = nullptr;
    for (const auto& room : Rooms) {
        if (!roomMaxX || room->GetMaxX() > roomMaxX->GetMaxX()) {
            roomMaxX = std::make_unique<Room>(*room);
        }
    }
    return roomMaxX;
}

std::unique_ptr<Room> Grid::GetRoomWithMinY() const {
    std::unique_ptr<Room> roomMinY = nullptr;
    for (const auto& room : Rooms) {
        if (!roomMinY || room->GetMinY() < roomMinY->GetMinY()) {
            roomMinY = std::make_unique<Room>(*room);
        }
    }
    return roomMinY;
}

std::unique_ptr<Room> Grid::GetRoomWithMaxY() const {
    std::unique_ptr<Room> roomMaxY = nullptr;
    for (const auto& room : Rooms) {
        if (!roomMaxY || room->GetMaxY() > roomMaxY->GetMaxY()) {
            roomMaxY = std::make_unique<Room>(*room);
        }
    }
    return roomMaxY;
}

/**
 * @brief Generates a random RoomType Enum, but makes sure that the dungeon has
 * at least one boss, one pickup-type room and a room containing the necessary key.
 */
RoomType Grid::DecideRoomType() {
    // Generate at least one KeyPickup room
    if (Rooms.empty() || std::ranges::none_of(Rooms, [](const std::unique_ptr<Room>& room) {
        return room->GetRoomType() == RoomType::KeyPickup;
    })) {
        return RoomType::KeyPickup;
    }
    // Generate at least one Boss room
    if (Rooms.empty() || std::ranges::none_of(Rooms, [](const std::unique_ptr<Room>& room) {
        return room->GetRoomType() == RoomType::Boss;
    })) {
        return RoomType::Boss;
    }
    // Generate at least one Pickup rooms
    if (Rooms.empty() || std::ranges::none_of(Rooms, [](const std::unique_ptr<Room>& room) {
        return room->GetRoomType() == RoomType::Pickup;
    })) {
        return RoomType::Pickup;
    }
    // 25% chance for any other room to be Boss, Pickup, or KeyPickup
    if (Random::GetRandomRange(1, 100) <= 25) {
        const int r = Random::GetRandomRange(1, 3);
        if (r == 1) {
            return RoomType::Boss;
        } else if (r == 2) {
            return RoomType::Pickup;
        } else {
            return RoomType::KeyPickup;
        }
    }
    // Else normal room
    return RoomType::Normal;
}

/**
 * @brief Checks if coordinate is valid and inside the Grid.
 * 
 */
bool Grid::IsValidPosition(const int x, const int y) const
{
    return x >= 0 && x < Width && y >= 0 && y < Height;
}

/**
 * @brief Checks if tiles in two rooms overlap.
 *
 * @param x starting x position of the room | RoomTiles[0][0].
 * @param y starting y position of the room | RoomTiles[0][0].
 * @param width width of the room waiting to get generated.
 * @param height height of the room waiting to get generated.
 */
bool Grid::IsRoomOverlap(const int x, const int y, const int width, const int height) const {
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

int Grid::GetHeight() const
{
    return Height;
}

int Grid::GetWidth() const
{
    return Width;
}

TileType Grid::GetTileTypeAt(const int x, const int y) const
{
    return GridTiles[x][y].GetTileType();
}

void Grid::UnlockExit() const
{
    ExitDoorTile->SetTileType(TileType::DoorExitOpen);
}

ExitColor Grid::GetDungeonExitColor() const
{
    return ExitDoorColor;
}

void Grid::MovePlayer(vector2 direction) const
{
    CurrentPlayer->Move(direction);
}
