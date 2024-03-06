#pragma once
#include <SDL_render.h>
#include <vector>

#include "Player.h"
#include "Room.h"
#include "Tile.h"
#include "../utils/ExitColor.h"

class Grid
{
public:
    Grid(const int width, const int height, const int tileWidth, const int tileHeight, SDL_Renderer* renderer, const std::vector<SDL_Surface*> images, const int inXOffset, const int inYOffset);

    void Initialize(const int rooms);

    void RenderGrid() const;
    
    [[nodiscard]] bool IsValidPosition(const int x, const int y) const;

    std::vector<std::vector<Tile>> GetGridTiles();

    void UpdatePlayerPosition(int prevX, int prevY, int newX, int newY);
    int GetHeight() const;
    int GetWidth() const;

    TileType GetTileTypeAt(const int x, const int y) const;

    void UnlockExit() const;

    ExitColor GetDungeonExitColor() const;

    void MovePlayer(vector2 direction) const;

private:
    
    void GenerateRoom(const int minRoomSize, const int maxRoomSize);
    void ConnectAllRooms();
    void ConnectTwoRooms(const Room* room1, const Room* room2);
    void GenerateDoors();
    void SpawnPlayerNearDoor(Room* room, const Tile* doorTile);
    bool IsKeyTile(const Tile* tile);
    RoomType DecideRoomType();
    
    [[nodiscard]] bool IsRoomOverlap(const int x, const int y, const int width, const int height) const;

    void DecorateOuterWorld();

    [[nodiscard]] Room* GetRoomWithMinX() const;
    [[nodiscard]] Room* GetRoomWithMaxX() const;
    [[nodiscard]] Room* GetRoomWithMinY() const;
    [[nodiscard]] Room* GetRoomWithMaxY() const;

    std::unique_ptr<Player> CurrentPlayer;

    int Width;
    int Height;
    int TileWidth;
    int TileHeight;
    int xOffset;
    int yOffset;
    int RoomCount;

    Tile* ExitDoorTile;
    ExitColor ExitDoorColor;
    
    SDL_Renderer* Renderer;
    
    std::vector<std::vector<Tile>> GridTiles;
    std::shared_ptr<std::vector<SDL_Surface*>> Sprites;
    std::vector<Room*> Rooms;
};
