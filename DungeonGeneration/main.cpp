#include <iostream>
#include <memory>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>

#include "src/Grid.h"
#include "src/Tile.h"
#include "utils/Consts.h"
#include "utils/SDLUtils.h"
#include "utils/SpriteUtils.h"
#include "window/Window.h"

int main(int argc, char* args[])
{
    SDLUtils::InitializeSDL();
    
    // Create Window and Renderer
    const std::unique_ptr<Window> game_window(new Window(WINDOW_WIDTH, WINDOW_HEIGHT, NOKIA_GREEN ,"Dungeon Generation"));
    SDL_Surface* spriteSheet = IMG_Load(IMG_TILES_URL);
    // Load image
    const std::vector<SDL_Surface*> images = SpriteUtils::SliceSpriteSheet(spriteSheet, 5, 5, 8, 8);
    
    Grid grid(25, 25, WINDOW_WIDTH / 25, WINDOW_HEIGHT / 25, game_window->renderer, images);
    grid.Initialize();
    
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

        // Render all tiles in the grid
        for (std::vector<Tile>& row : grid.GetGridTiles()) {
            for (Tile& tile : row) {
                SDL_RenderCopy(game_window->renderer, tile.GetTexture(), nullptr, tile.GetRect());
            }
        }

        game_window->Present();

        SDL_Delay(REFRESH_RATE);
    }

    return 0;
}