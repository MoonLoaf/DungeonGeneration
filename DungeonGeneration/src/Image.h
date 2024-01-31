#pragma once
#include <SDL_render.h>

#include "../utils/Vector2.h"

class Image
{
protected:
    SDL_Renderer* Renderer;

    const char* URL;

    SDL_Texture* Texture;

    SDL_Rect* Rect;

    vector2 Position;
    
    vector2 Scale;
    
public:
    Image(vector2 position, vector2 scale, const char* newImageURL, SDL_Renderer* renderer);
    Image(vector2 position, vector2 scale, SDL_Surface* Surface, SDL_Renderer* renderer);

    void SetTexture(const char* ImgUrl);
    void SetTexture(SDL_Surface* Surface);

    const char* GetImageURL() const;

    SDL_Renderer* GetRenderer() const;

    vector2 GetPosition() const;

    vector2 GetScale() const;

    SDL_Texture* GetTexture() const;

    SDL_Rect* GetRect() const;
};
