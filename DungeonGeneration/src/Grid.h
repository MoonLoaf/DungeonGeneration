#pragma once
#include <SDL_render.h>
#include <stack>
#include <vector>

#include "Room.h"
#include "Tile.h"

class Grid
{
public:
    Grid(int width, int height, int tileWidth, int tileHeight, SDL_Renderer* renderer, std::vector<SDL_Surface*> images, int inXOffset, int inYOffset);

    void Initialize(int rooms);
    
    void GenerateRoom();
    void ConnectRooms();

    bool IsRoomOverlap(int x, int y, int width, int height) const;

    int getRandom(int min, int max);

    std::vector<std::vector<Tile>> GetGridTiles();
    
private:
    void ConnectTwoRooms(Room* room1, Room* room2);
    bool IsValidPosition(int x, int y);

    int Width;
    int Height;
    int TileWidth;
    int TileHeight;
    int xOffset;
    int yOffset;
    
    SDL_Renderer* Renderer;
    
    std::vector<std::vector<Tile>> GridTiles;
    std::vector<SDL_Surface*> Sprites;
    std::vector<Room*> Rooms;
    
};
