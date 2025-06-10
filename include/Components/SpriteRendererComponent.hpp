#pragma once

#include "ECS.hpp"
#include "TextureManager.hpp"
#include "PositionComponent.hpp"

#include <SDL.h>

class SpriteRendererComponent : public Component
{
private:
    SDL_Texture* sprite;
    SDL_Rect* src_Rect;
public:
    SpriteRendererComponent(){};
    ~SpriteRendererComponent(){};

    void init()
    {
        sprite = nullptr;
        src_Rect = nullptr;
    }
    void update(){};
    void render(SDL_Renderer* renderer, Camera* cam)
    {
        SDL_Rect dst_Rect {(int)entity->getComponent<PositionComponent>().getX() - cam->getX(), (int)entity->getComponent<PositionComponent>().getY() - cam->getY(), 64, 64};
        SDL_RenderCopy(renderer, sprite, src_Rect, &dst_Rect);
    }

    void setSprite(const char* filename)
    {
        sprite = TextureManager::loadTexture(filename);
    }

    void setRenderParameters(SDL_Rect* src_Rect)
    {
        this->src_Rect = src_Rect;
    }

    void setSprite(SDL_Texture* sprite)
    {
        this->sprite = sprite;
    }

    SDL_Texture* getSprite()
    {
        return sprite;
    }

};