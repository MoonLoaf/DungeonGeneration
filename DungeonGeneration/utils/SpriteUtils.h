#pragma once
#include <SDL_surface.h>
#include <vector>

class SpriteUtils
{
public:
    static std::vector<SDL_Surface*> SliceSpriteSheet(SDL_Surface* spriteSheet, int Rows, int Columns, int OutHeight, int OutWidth);
    
};
