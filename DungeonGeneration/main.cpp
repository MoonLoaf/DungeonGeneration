#include <iostream>
#include <memory>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>

#include "Utilities/Consts.h"
#include "Utilities/SDLUtils.h"
#include "Window/Window.h"

using namespace std;

int main(int argc, char* args[])
{
    SDLUtils::InitializeSDL();
    
    // Create Window and Renderer
    const unique_ptr<Window> game_window(new Window(WINDOW_WIDTH, WINDOW_HEIGHT, WHITE ,"Dungeon Generation"));
    
    // Load image
    SDL_Surface* imageSurface = IMG_Load(IMG_TILES_URL);
    if (!imageSurface) {
        // Handle error if image loading fails
        cerr << "Failed to load image: " << IMG_GetError() << endl;
        return 1;
    }

    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(game_window->renderer, imageSurface);

    SDL_FreeSurface(imageSurface);
    
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
        SDL_RenderCopy(game_window->renderer, imageTexture, NULL, NULL);

        game_window->Present();

        SDL_Delay(REFRESH_RATE);
    }

    return 0;
}