#pragma once

#include "ECS.hpp"
#include "Game.hpp"
#include "TextureManager.hpp"
#include "PositionComponent.hpp"

#include <SDL.h>

class SpriteRendererComponent : public Component
{
private:
    SDL_Texture* sprite;
public:
    SpriteRendererComponent(){};
    ~SpriteRendererComponent(){};

    void init()
    {
        sprite = nullptr;
    }
    void update(){};
    void render(Camera* cam)
    {
        SDL_Rect dst_Rect {(int)entity->getComponent<PositionComponent>().getX() - cam->getX(), (int)entity->getComponent<PositionComponent>().getY() - cam->getY(), 64, 64};
        SDL_RenderCopy(Game::renderer, sprite, NULL, &dst_Rect);
    }

    void setSprite(const char* filename)
    {
        sprite = TextureManager::loadTexture(filename);
    }

};