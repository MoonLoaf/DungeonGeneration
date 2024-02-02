#pragma once
#include "Image.h"
#include "../utils/TileType.h"
#include "../utils/Vector2.h"

class Tile : public Image
{
public:
    Tile(vector2 position, vector2 scale, const char* newImageURL, SDL_Renderer* renderer, TileType tileType, vector2 gridPos);
    Tile(vector2 position, vector2 scale, SDL_Surface* Surface, SDL_Renderer* renderer, TileType tileType, vector2 gridPos);
    
    TileType GetTileType() const;

    void SetTileType(TileType newType);

    vector2 GetGridPos() const;
    
protected:
    TileType Type;

    vector2 GridPosition;
};
