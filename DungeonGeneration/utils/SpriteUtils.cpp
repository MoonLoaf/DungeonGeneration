#include "SpriteUtils.h"

#include <SDL_surface.h>
#include <vector>

/**
 * @brief Slices a sprite sheet into individual sprites in a vector.
 * 
 * This function takes a sprite sheet and divides it into smaller sprites based on the specified
 * number of rows and columns. Each smaller sprite has the specified output height and width.
 * 
 * @param spriteSheet Pointer to the SDL_Surface containing the sprite sheet.
 * @param Rows Number of rows in the sprite sheet.
 * @param Columns Number of columns in the sprite sheet.
 * @param OutHeight Height of each output sprite.
 * @param OutWidth Width of each output sprite.
 * @return A vector containing pointers to the individual sprites.
 */
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