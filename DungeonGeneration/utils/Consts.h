#pragma once

constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 768;
constexpr int WINDOW_CENTER_X = WINDOW_WIDTH / 2;
constexpr int WINDOW_CENTER_Y = WINDOW_HEIGHT / 2;

constexpr float REFRESH_RATE = 1000.f * 1.f / 144.f;

constexpr SDL_Color WHITE {255, 255, 255};
constexpr SDL_Color BLACK {0, 0, 0};
constexpr SDL_Color RED {255, 0, 0};
constexpr SDL_Color YELLOW {255, 255, 0};
constexpr SDL_Color BLUE {0, 0, 255};
constexpr SDL_Color CYAN {0, 255, 255};
constexpr SDL_Color GREEN {0, 255, 0};
constexpr SDL_Color PINK {255, 0, 255};
constexpr SDL_Color NOKIA_GREEN{67, 82, 61};

const char* const IMG_TILES_URL{ "assets/img/8x8.png" };
