#include "Tile.h"
#include "../utils/Vector2.h"

void Tile::Draw()
{
    //Same logic for now
    Image::Draw();
}

Tile::Tile(const vector2 position, const vector2 scale, const char* newImageURL, SDL_Renderer* renderer, const TileType tileType, const vector2 gridPos)
    : Image(position, scale, newImageURL, renderer)
{
    Type = tileType;
    GridPosition = gridPos;
}

Tile::Tile(const vector2 position, const vector2 scale, SDL_Surface* surface, SDL_Renderer* renderer, const TileType tileType, const vector2 gridPos)
    : Image(position, scale, surface, renderer)
{
    Type = tileType;
    GridPosition = gridPos;
}

TileType Tile::GetTileType() const
{
    return Type;
}

void Tile::SetTileType(const TileType newType)
{
    Type = newType;
}

vector2 Tile::GetGridPos() const
{
    return GridPosition;
}
