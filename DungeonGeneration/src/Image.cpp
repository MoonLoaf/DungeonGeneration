#include "Image.h"
#include <SDL_image.h>

void Image::Draw()
{
    SDL_RenderCopy(Renderer, Texture, nullptr, Rect);
}

Image::Image(vector2 position, vector2 scale, const char* newImageURL, SDL_Renderer* renderer) {
    Rect = new SDL_Rect{position.x, position.y, scale.x, scale.y};
    Renderer = renderer;
    SetTexture(newImageURL);
    URL = newImageURL;
    Position = position;
    Scale = scale;
}

Image::Image(vector2 position, vector2 scale, SDL_Surface* Surface, SDL_Renderer* renderer) {
    Rect = new SDL_Rect{position.x, position.y, scale.x, scale.y};
    Renderer = renderer;
    SetTexture(Surface);
    Position = position;
    Scale = scale;
    URL = nullptr;
}

void Image::SetTexture(const char* ImgUrl) {
    SDL_Surface* loadedSurface = IMG_Load(ImgUrl);
    Texture = SDL_CreateTextureFromSurface(Renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
}

void Image::SetTexture(SDL_Surface* Surface)
{
    Texture = SDL_CreateTextureFromSurface(Renderer, Surface);
}

const char* Image::GetImageURL() const
{
    return URL;
}

SDL_Renderer* Image::GetRenderer() const
{
    return Renderer;
}

vector2 Image::GetPosition() const
{
    return Position;
}

vector2 Image::GetScale() const
{
    return Scale;
}

SDL_Texture* Image::GetTexture() const
{
    return Texture;
}

SDL_Rect* Image::GetRect() const
{
    return Rect;
}
