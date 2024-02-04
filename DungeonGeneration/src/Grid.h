#pragma once
#include <SDL_render.h>
#include <vector>

#include "Room.h"
#include "Tile.h"

class Grid
{
public:
    Grid(int width, int height, int tileWidth, int tileHeight, SDL_Renderer* renderer, std::vector<SDL_Surface*> images, int inXOffset, int inYOffset);

    void Initialize(int rooms);
    
    void GenerateRoom(const int minRoomSize, const int maxRoomSize);
    void ConnectRooms();

    bool IsRoomOverlap(int x, int y, int width, int height) const;

    std::vector<std::vector<Tile>> GetGridTiles();
    
private:
    void ConnectTwoRooms(Room* room1, Room* room2);
    void GenerateDoors() const;
    Room* GetRoomWithMinX() const;
    Room* GetRoomWithMaxX() const;
    Room* GetRoomWithMinY() const;
    Room* GetRoomWithMaxY() const;
    bool IsValidPosition(int x, int y) const;
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
