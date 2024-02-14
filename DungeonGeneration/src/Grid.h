#pragma once
#include <SDL_render.h>
#include <vector>

#include "Room.h"
#include "Tile.h"

class Grid
{
public:
    Grid(const int width, const int height, const int tileWidth, const int tileHeight, SDL_Renderer* renderer, const std::vector<SDL_Surface*> images, const int inXOffset, const int inYOffset);

    void Initialize(const int rooms);

    void GenerateRoom(const int minRoomSize, const int maxRoomSize);
    void ConnectRooms();

    [[nodiscard]] bool IsRoomOverlap(int x, int y, int width, int height) const;

    std::vector<std::vector<Tile>> GetGridTiles();
    
private:
    void ConnectTwoRooms(const Room* room1, const Room* room2);
    
    void GenerateDoors() const;

    void DecorateOuterWorld();

    [[nodiscard]] Room* GetRoomWithMinX() const;
    [[nodiscard]] Room* GetRoomWithMaxX() const;
    [[nodiscard]] Room* GetRoomWithMinY() const;
    [[nodiscard]] Room* GetRoomWithMaxY() const;

    [[nodiscard]] bool IsValidPosition(const int x, const int y) const;

    RoomType DecideRoomType();

    int Width;
    int Height;
    int TileWidth;
    int TileHeight;
    int xOffset;
    int yOffset;
    
    SDL_Renderer* Renderer;
    
    std::vector<std::vector<Tile>> GridTiles;
    std::shared_ptr<std::vector<SDL_Surface*>> Sprites;
    std::vector<Room*> Rooms;
};
