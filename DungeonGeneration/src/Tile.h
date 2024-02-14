#pragma once
#include "Image.h"
#include "../utils/TileType.h"
#include "../utils/Vector2.h"

class Tile : public Image
{
public:
    Tile(const vector2 position, const vector2 scale, const char* newImageURL, SDL_Renderer* renderer, const TileType tileType, const vector2 gridPos);
    Tile(const vector2 position, const vector2 scale, SDL_Surface* surface, SDL_Renderer* renderer, const TileType tileType, const vector2 gridPos);

    [[nodiscard]] TileType GetTileType() const;

    void SetTileType(const TileType newType);

    [[nodiscard]] vector2 GetGridPos() const;
    
protected:
    TileType Type;

    vector2 GridPosition;
};
