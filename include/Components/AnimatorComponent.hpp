#pragma once

#include "ECS.hpp"
#include "SpriteRendererComponent.hpp"
#include "Utilities.hpp"
#include "TextureManager.hpp"

#include <map>
#include <vector>
#include <SDL.h>
#include <functional>

class SpriteSheet
{
private:
    SDL_Texture *spriteSheet;
    Cooldown frameTime;
    SDL_Rect src_Rect;

    int currentFrame;
    int nFrame;
    int wFrame;
    int hFrame;
    int wSpriteSheet;
public:
    SpriteSheet(SDL_Texture *spriteSheet, int nFrame, double timeBetweenFrame)
        : spriteSheet(spriteSheet), nFrame(nFrame)
    {
        currentFrame = 0;
        frameTime = {0, timeBetweenFrame};

        int w, h;
        SDL_QueryTexture(spriteSheet, NULL, NULL, &w, &h);
        wFrame = w / nFrame;
        hFrame = h;
        wSpriteSheet = w;
        src_Rect = {0, 0, wFrame, hFrame};
    }

    void update(double dt)
    {
        frameTime.update(dt);
        if(frameTime.execute())
        {
            src_Rect.x += wFrame;
            currentFrame++;
            if(src_Rect.x >= wSpriteSheet){
                src_Rect.x = 0;
                currentFrame = 0;
            }
        }
    }

    SDL_Texture* getTexture(){return spriteSheet;};

    SDL_Rect* passParameter()
    {
        return &src_Rect;
    }

    void setPlayTime(double time)
    {
        frameTime.cooldown = time / nFrame;
    }

    int getCurrentFrameIndex()
    {
        return currentFrame;
    }
};

class AnimatorComponent : public Component
{
private:
    friend class SpriteRendererComponent;
    friend class SpriteSheet;

    SpriteRendererComponent *sprite;
    std::map<std::string, SpriteSheet> spriteSheets;
    std::vector<std::pair<std::function<bool()>, std::string>> conditions;
    SpriteSheet *currentSpriteSheet;

public:
    AnimatorComponent() {};
    ~AnimatorComponent() {};

    void init()
    {
        sprite = &entity->getComponent<SpriteRendererComponent>();
        //sprite->setRenderParameters(currentSpriteSheet->passParameter());
        currentSpriteSheet = nullptr;
        spriteSheets = {};
    }

    void update(double dt)
    {
        for(auto condition : conditions) {
            if(condition.first()) setCurrentSpriteSheet(condition.second);
        }
        currentSpriteSheet->update(dt);
    }
    void render(SDL_Renderer* renderer, Camera* cam) {}

    void addSpriteSheet(const char* sheetLocation, std::string sheetName, int nFrame, double timeBetweenFrame)
    {
        spriteSheets.insert({sheetName, SpriteSheet(TextureManager::loadTexture(sheetLocation), nFrame, timeBetweenFrame)});
    }

    void addSpriteSheet(SDL_Renderer* renderer, const char* sheetLocation, std::vector<std::string> sheetNames, int nFramePerLine, double timeBetweenFrame, int h = -1, int w = -1)
    {
        SDL_Texture* texture = TextureManager::loadTexture(sheetLocation);

        if(h < 0){
            SDL_QueryTexture(texture, NULL, NULL, NULL, &h);
            h /= sheetNames.size();
        }
        if (w < 0)
        {
            SDL_QueryTexture(texture, NULL, NULL, &w, NULL);
        }

        for (int i = 0; i < sheetNames.size(); i++)
        {   
            SDL_Texture* nText = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
            SDL_SetRenderTarget(renderer, nText);

            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetTextureBlendMode(nText, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);

            SDL_Rect src_rect = {0, h * i, w, h};
            SDL_RenderCopy(renderer, texture, &src_rect, NULL);

            spriteSheets.insert({sheetNames[i], SpriteSheet(nText, nFramePerLine, timeBetweenFrame)});
        }
        SDL_SetRenderTarget(renderer, NULL);
    }

    void addSpriteSheet(SDL_Renderer* renderer, const char* sheetLocation, std::vector<std::string> sheetNames, std::vector<int> nFramePerLines, std::vector<double> timeBetweenFrames, int hFrame = -1, int wFrame = -1)
    {
        if(sheetNames.size() != nFramePerLines.size() || sheetNames.size() != timeBetweenFrames.size()) return;

        SDL_Texture* texture = TextureManager::loadTexture(sheetLocation);

        for(int i = 0;  i < nFramePerLines.size(); i++)
        {
            if(hFrame < 0){
                SDL_QueryTexture(texture, NULL, NULL, NULL, &hFrame);
                hFrame /= sheetNames.size();
            }
            if (wFrame < 0)
            {
                SDL_QueryTexture(texture, NULL, NULL, &wFrame, NULL);
            }
            SDL_Texture* nText = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, wFrame * nFramePerLines[i], hFrame);
            SDL_SetRenderTarget(renderer, nText);

            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetTextureBlendMode(nText, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);

            SDL_Rect src_rect = {0, hFrame * i, wFrame * nFramePerLines[i], hFrame};
            SDL_RenderCopy(renderer, texture, &src_rect, NULL);

            spriteSheets.insert({sheetNames[i], SpriteSheet(nText, nFramePerLines[i], timeBetweenFrames[i])});
        }
        SDL_SetRenderTarget(renderer, NULL);
    }

    void loadFolder(std::string folderLocation)
    {
        for(const auto& entry : std::filesystem::directory_iterator(folderLocation)) //Find all Tile Map file
        {
            auto pathStr = entry.path().string();
            SDL_Texture* spriteSheet = TextureManager::loadTexture(entry.path().string().c_str());
            std::string sheetName = pathStr.substr(pathStr.find_last_of("/") + 1, (pathStr.find_last_of(".") - 1) - pathStr.find_last_of("/"));

            int w, h;
            SDL_QueryTexture(spriteSheet, NULL, NULL, &w, &h);
            std::cout << sheetName  << " : " << w/h << std::endl;
            spriteSheets.insert({sheetName, SpriteSheet(spriteSheet, w/h, 0.100)});
        }
    }

    void addConditions(std::string sheetName, std::function<bool()> condition)
    {
        if(spriteSheets.find(sheetName) != spriteSheets.end()) conditions.push_back({condition, sheetName});
    }

    void setCurrentSpriteSheet(std::string sheetName)
    {
        currentSpriteSheet = &spriteSheets.find(sheetName)->second;
        sprite->setSprite(currentSpriteSheet->getTexture());
        sprite->setRenderParameters(currentSpriteSheet->passParameter());
    }

    void playAnimationIn(std::string sheetName, double time)
    {
        spriteSheets.find(sheetName)->second.setPlayTime(time);
    }

    int getCurrentFrameIndex()
    {
        return currentSpriteSheet->getCurrentFrameIndex();
    }
};