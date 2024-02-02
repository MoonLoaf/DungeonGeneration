#pragma once
#include <SDL_render.h>

class Window
{
	//The window we'll be rendering to
	SDL_Window* window{};
	
	//The surface contained by the window
	SDL_Surface* screenSurface{};
	
	bool createdWindowSuccessfully;
	
	// Dependencies to Renderer & Image Loader
	class IImageLoader* imageLoader;
public:
	Window(int width, int height, SDL_Color windowColor, const char* windowTitle);
	~Window();

	SDL_Renderer* renderer;
	
	bool WasSuccessful() const { return createdWindowSuccessfully; }
	
	void Clear() const;
	void Present() const;
};

