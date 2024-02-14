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
    const std::unique_ptr<Window> game_window(new Window(WINDOW_WIDTH, WINDOW_HEIGHT, DEEP_PURPLE ,"Dungeon Generation"));

    // Load image
    SDL_Surface* spriteSheet = IMG_Load(IMG_TILES_URL);
    const std::vector<SDL_Surface*> images = SpriteUtils::SliceSpriteSheet(spriteSheet, 12, 8, 8, 8);

    // Initialize the grid
    Grid grid(60, 60, WINDOW_WIDTH / 60, WINDOW_HEIGHT / 60, game_window->renderer, images, 2, 10);
    grid.Initialize(9);
    
    SDL_Event e;
    bool quit = false;

    // Game Loop (runs until quit)
    Uint32 lastInputTime = SDL_GetTicks();
    while (!quit)
    {
        SDL_GetTicks();

        // Loop through all pending events from Windows (OS)
        while (SDL_PollEvent(&e))
        {
            // Check, if it's an event we want to react to:
            switch (e.type) {
                case SDL_QUIT: {
                    quit = true;
                } break;
            default: ;
            }
        }

        // Handle player movement
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastInputTime >= INPUT_DELAY_MS) {
            const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
            if (currentKeyStates[SDL_SCANCODE_W]) {
                grid.GetPlayer()->moveUp();
            }
            if (currentKeyStates[SDL_SCANCODE_S]) {
                grid.GetPlayer()->moveDown();
            }
            if (currentKeyStates[SDL_SCANCODE_A]) {
                grid.GetPlayer()->moveLeft();
            }
            if (currentKeyStates[SDL_SCANCODE_D]) {
                grid.GetPlayer()->moveRight();
            }
            lastInputTime = currentTime; // Update the last input time
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