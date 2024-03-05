#include "GameManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

#include "Grid.h"
#include "Tile.h"
#include "../utils/Consts.h"
#include "../utils/SpriteUtils.h"
#include "../window/Window.h"


GameManager::GameManager(Window* window)
{
    GameWindow = window;
}

void GameManager::StartGameLoop()
{
    quitGame = false;
    // Load image
    SDL_Surface* spriteSheet = IMG_Load(IMG_TILES_URL);
    const std::vector<SDL_Surface*> images = SpriteUtils::SliceSpriteSheet(spriteSheet, 12, 8, 8, 8);

    // Initialize the grid
    Grid grid(60, 60, WINDOW_WIDTH / 60, WINDOW_HEIGHT / 60, GameWindow->renderer, images, 2, 10);
    grid.Initialize(9);
    
    SDL_Event e;

    // Game Loop (runs until quit)
    Uint32 lastInputTime = SDL_GetTicks();
    while (!quitGame)
    {
        SDL_GetTicks();

        // Loop through all pending events from Windows (OS)
        while (SDL_PollEvent(&e))
        {
            // Check, if it's an event we want to react to:
            switch (e.type) {
                case SDL_QUIT: {
                    quitGame = true;
                } break;
            default: ;
            }
        }

        // Handle player movement
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastInputTime >= INPUT_DELAY_MS) {
            const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
            if (currentKeyStates[SDL_SCANCODE_W]) {
                grid.MovePlayer(vector2(0, -1)); // Move Up
            }
            if (currentKeyStates[SDL_SCANCODE_S]) {
                grid.MovePlayer(vector2(0, 1)); // Move Down
            }
            if (currentKeyStates[SDL_SCANCODE_A]) {
                grid.MovePlayer(vector2(-1, 0)); // Move Left
            }
            if (currentKeyStates[SDL_SCANCODE_D]) {
                grid.MovePlayer(vector2(1, 0)); // Move Right
            }
            lastInputTime = currentTime; // Update the last input time
        }
        
        GameWindow->Clear();

        // Render all tiles in the grid
        grid.RenderGrid();   

        GameWindow->Present();

        SDL_Delay(REFRESH_RATE);
    }
    return;
}