#pragma once

#include <memory>

#include "../window/Window.h"

class GameManager
{
public:
    GameManager(std::shared_ptr<Window> window);

    void StartGameLoop();

private:
    std::shared_ptr<Window> GameWindow;

    bool quitGame;
};
