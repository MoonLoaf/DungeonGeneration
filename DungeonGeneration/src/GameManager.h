#pragma once

#include "../window/Window.h"

class GameManager
{
public:
    GameManager(Window* window);

    void StartGameLoop();

private:
    Window* GameWindow;

    bool quitGame;
};
