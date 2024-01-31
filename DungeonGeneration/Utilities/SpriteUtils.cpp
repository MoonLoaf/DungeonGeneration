#include "SpriteUtils.h"

#include <SDL_surface.h>
#include <vector>

std::vector<SDL_Surface*> SpriteUtils::SliceSpriteSheet(SDL_Surface* spriteSheet, int Rows, int Columns, int OutHeight, int OutWidth) {
    std::vector<SDL_Surface*> images;

    for (int row = 0; row < Rows; ++row) {
        for (int col = 0; col < Columns; ++col) {
            SDL_Rect srcRect = { col * OutWidth, row * OutHeight, OutWidth, OutHeight };

            SDL_Surface* image = SDL_CreateRGBSurface(0, OutWidth, OutHeight, 32, 0, 0, 0, 0);

            if (image) {
                SDL_BlitSurface(spriteSheet, &srcRect, image, nullptr);
                images.push_back(image);
            }
        }
    }

    return images;
}