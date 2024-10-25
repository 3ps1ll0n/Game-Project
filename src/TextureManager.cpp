#include "TextureManager.hpp"
#include "Game.hpp"
#include <SDL_image.h>

SDL_Texture* TextureManager::loadTexture(const char* filename)
{
    SDL_Surface* tempSur = IMG_Load(filename);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSur);

    SDL_FreeSurface(tempSur);

    return tex;
}