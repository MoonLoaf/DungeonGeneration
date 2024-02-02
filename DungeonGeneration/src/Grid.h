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
    
    void GenerateCorridors();
    void ConnectRooms(Room* room1, Room* room2);
    void ConnectTilesDFS(const vector2& start, const vector2& end, std::vector<std::vector<bool>>& corridorMatrix);
    void RecursiveDFS(const vector2& current, const vector2& end, std::vector<std::vector<bool>>& corridorMatrix,
                      std::vector<std::vector<int>>& distances, std::vector<std::vector<vector2>>& previous);
    void AddNeighborsToStack(std::stack<vector2>& stack, const vector2& current,
                             std::vector<std::vector<bool>>& corridorMatrix, std::vector<std::vector<bool>>& visited,
                             std::vector<std::vector<vector2>>& previous);
    void UpdateNeighbors(std::vector<std::vector<int>>& distances, std::vector<std::vector<vector2>>& previous, const vector2& current);

    bool IsValidPosition(int x, int y);
    bool IsRoomOverlap(int x, int y, int width, int height) const;

    std::vector<std::vector<Tile>> GetGridTiles();
    int getRandom(int min, int max);
private:

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
