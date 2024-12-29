#pragma once

#include "SDL.h"
#include <math.h>
#include <iostream>
#include "SpriteRendererComponent.hpp"
#include "TextureManager.hpp"

struct Vector2D{
    double x;
    double y;

    void reset()
    {
        x = 0.0;
        y = 0.0;
    }

    void diplay(SDL_Renderer* r, int xPos, int yPos, double factor, SDL_Color color)
    {
        SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(r, xPos, yPos, xPos + (x * factor), yPos + (y * factor));
    }

    void normalize()
    {
        if(x == 0 && y == 0) return;
        double nX =  x / (sqrt(pow(x, 2) + pow(y, 2)));
        double nY =  y / (sqrt(pow(x, 2) + pow(y, 2)));       

        x = nX;
        y = nY;
    }

    Vector2D operator *=(double m)
    {
        x *= m;
        y *= m;
        return *this;
    }
};

class Texture : public Entity 
{
public:
    Texture()
    {
        addComponent<SpriteRendererComponent>();
    }
    Texture(SDL_Texture* tex)
    {
        addComponent<SpriteRendererComponent>();
        getComponent<SpriteRendererComponent>().setSprite(tex);
    }

    void setTexture(SDL_Texture* tex)
    {
        getComponent<SpriteRendererComponent>().setSprite(tex);
    }

    SDL_Texture* getTexture()
    {
        return getComponent<SpriteRendererComponent>().getSprite();
    }
};

class TextureContainer
{
private:
    std::vector<Texture> textures;
public:
    TextureContainer()
    {
        auto tileSheet = TextureManager::loadTexture("assets/tileSheet.png");
        int w, h;
        SDL_QueryTexture(tileSheet, NULL, NULL, &w, &h);

        SDL_Rect srcRect = {0, 0, 32, 32};

        for(int i = 0; i < h/srcRect.h; i++)
        {
            srcRect.y = i*srcRect.h;
            for(int j = 0; j < w/srcRect.w; j++)
            {
                srcRect.x = j*srcRect.w;

                SDL_Texture* tex = SDL_CreateTexture(Game::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, srcRect.w, srcRect.h);

                SDL_SetRenderTarget(Game::renderer, tex);

                SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_BLEND);
                SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 0);
                SDL_RenderClear(Game::renderer);

                SDL_RenderCopy(Game::renderer, tileSheet, &srcRect, NULL);

                textures.push_back(Texture(tex));
            }
        }
        SDL_SetRenderTarget(Game::renderer, NULL);
    }

    SDL_Texture* getTexture(int index)
    {
        return textures[index].getTexture();
    }
};

struct Cooldown{
    double dt;
    double cooldown;

    void update(double dt)
    {
        this->dt += dt;
    }

    bool canExecute()
    {
        if(dt >= cooldown) 
        {
            dt = 0;
            return true;
        }
        return false;
    }
};
