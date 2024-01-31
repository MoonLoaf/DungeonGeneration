#pragma once
#include "Image.h"
#include "../utils/TileType.h"
#include "../utils/Vector2.h"

class Tile : public Image
{
public:
    Tile(vector2 position, vector2 scale, const char* newImageURL, SDL_Renderer* renderer, TileType tileType);
    Tile(vector2 position, vector2 scale, SDL_Surface* Surface, SDL_Renderer* renderer, TileType tileType);
    
    TileType GetTileType() const;

    void SetTileType(TileType newType);
    
protected:
    TileType Type;
};
