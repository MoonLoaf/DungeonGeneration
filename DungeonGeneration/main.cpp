#include <iostream>
#include <memory>
#include <SDL.h>
#include <string>
#include <vector>

#include "src/GameManager.h"
#include "utils/Consts.h"
#include "utils/SDLUtils.h"
#include "window/Window.h"

int main(int argc, char* args[])
{
    SDLUtils::InitializeSDL();
    
    // Create Window and Renderer
    Window* game_window(new Window(WINDOW_WIDTH, WINDOW_HEIGHT, DEEP_PURPLE ,"Dungeon Generation"));

    GameManager* gameManager = new GameManager(game_window);
    gameManager->StartGameLoop();

    return 0;
}
