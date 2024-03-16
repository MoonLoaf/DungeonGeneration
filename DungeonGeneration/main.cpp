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
    auto gameWindow = std::make_shared<Window>(WINDOW_WIDTH, WINDOW_HEIGHT, DEEP_PURPLE, "Dungeon Generation");

    const auto gameManager = std::make_unique<GameManager>(gameWindow);
    gameManager->StartGameLoop();

    return 0;
}
