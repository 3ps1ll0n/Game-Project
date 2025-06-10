#include "SpriteRendererComponent.hpp"
#include "Game.hpp"

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
