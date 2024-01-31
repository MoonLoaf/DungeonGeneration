#pragma once
#include <SDL_render.h>
#include <vector>

#include "Tile.h"

class Grid
{
public:
    Grid(int width, int height, int tileWidth, int tileHeight, SDL_Renderer* renderer, std::vector<SDL_Surface*> images);

    void Render() const;

    void Initialize();
    
    std::vector<std::vector<Tile>> GetGridTiles();
private:

    int Width;
    int Height;
    int TileWidth;
    int TileHeight;
    SDL_Renderer* Renderer;
    
    std::vector<std::vector<Tile>> GridTiles;
    std::vector<SDL_Surface*> Sprites;
    
};
