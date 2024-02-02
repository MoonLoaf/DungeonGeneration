#include "Tile.h"
#include "../utils/Vector2.h"

Tile::Tile(vector2 position, vector2 scale, const char* newImageURL, SDL_Renderer* renderer, TileType tileType, vector2 gridPos)
    : Image(position, scale, newImageURL, renderer)
{
    Type = tileType;
    GridPosition = gridPos;
}

Tile::Tile(vector2 position, vector2 scale, SDL_Surface* Surface, SDL_Renderer* renderer, TileType tileType, vector2 gridPos)
    : Image(position, scale, Surface, renderer)
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
