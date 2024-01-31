#include <iostream>
#include <memory>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>

#include "Utilities/Consts.h"
#include "Utilities/SDLUtils.h"
#include "Utilities/SpriteUtils.h"
#include "Window/Window.h"

using namespace std;

int main(int argc, char* args[])
{
    SDLUtils::InitializeSDL();
    
    // Create Window and Renderer
    const unique_ptr<Window> game_window(new Window(WINDOW_WIDTH, WINDOW_HEIGHT, NOKIA_GREEN ,"Dungeon Generation"));
    SDL_Surface* spriteSheet = IMG_Load(IMG_TILES_URL);
    // Load image
    std::vector<SDL_Surface*> images = SpriteUtils::SliceSpriteSheet(spriteSheet, 5, 5, 8, 8);

    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(game_window->renderer, images[1]);

    SDL_FreeSurface(images[1]);
    
    SDL_Event e;
    bool quit = false;

    // Game Loop (runs until quit)
    while (!quit)
    {
        // Can be used, to see, how much time in ms has passed since app start
        SDL_GetTicks();

        // Loop through all pending events from Windows (OS)
        while (SDL_PollEvent(&e))
        {
            // Check, if it's an event we want to react to:
            switch (e.type) {
                case SDL_QUIT: {
                    quit = true;
                } break;
            }
        }
        
        game_window->Clear();

        // Render all active game objects
        SDL_Rect rect = {WINDOW_CENTER_X, WINDOW_CENTER_Y, 64, 64};
        SDL_RenderCopy(game_window->renderer, imageTexture, NULL, &rect);

        game_window->Present();

        SDL_Delay(REFRESH_RATE);
    }

    return 0;
}