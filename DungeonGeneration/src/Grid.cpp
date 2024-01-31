#include "Grid.h"

#include <utility>
#include "../utils/Consts.h"

Grid::Grid(int width, int height, int tileWidth, int tileHeight, SDL_Renderer* renderer, std::vector<SDL_Surface*> images)
{
    Width = width;
    Height = height;
    TileWidth = tileWidth;
    TileHeight = tileHeight;
    Renderer = renderer;
    Sprites = std::move(images);
}

void Grid::Initialize()
{
    GridTiles.reserve(Width);

    for (int i = 0; i < Width; ++i) {
        GridTiles.push_back(std::vector<Tile>());
        GridTiles[i].reserve(Height);

        for (int j = 0; j < Height; ++j) {
            vector2 position(i * TileWidth, j * TileHeight);
            vector2 scale(TileWidth, TileHeight);
            TileType tileType = TileType::Ground;  // Replace with your desired tile type
            const int randomIndex = rand() % Sprites.size(); 
            GridTiles[i].emplace_back(position, scale, Sprites[randomIndex], Renderer, tileType);
        }
    }
}

std::vector<std::vector<Tile>> Grid::GetGridTiles()
{
    return GridTiles;
}
