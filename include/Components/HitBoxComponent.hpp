#pragma once

#include "ECS.hpp"
#include "PositionComponent.hpp"
#include "Game.hpp"

#include <vector>
#include <map>
#include <memory>
#include <SDL.h>

enum HitBoxType
{
    BODY,
    DAMAGE,
    TRIGGER,
};

struct HitBox
{
    double x, y;
    double oldX, oldY;
    double xOffset, yOffset;
    int w, h;

    HitBox(double xOffset, double yOffset, int w, int h)
    :x(0), y(0), xOffset(xOffset), yOffset(yOffset), w(w), h(h) {}

    HitBox()
    :x(0), y(0), xOffset(0), yOffset(0), w(0), h(0) {}

    SDL_Rect hitBoxToSDL_Rect() { return {(int)x, int(y), w, h}; }
    void moveBack()
    {
        x = oldX;
        y = oldY;
    }
};

class HitBoxComponent : public Component
{
private:
    std::map<HitBoxType, std::vector<HitBox*>> hitboxes;
public:
    HitBoxComponent(/* args */) {};
    ~HitBoxComponent() {};

    void init()
    {
        hitboxes = {};
    }
    void update(double dt)
    {
        for(auto it = hitboxes.begin(); it != hitboxes.end(); it++)
        {
            std::vector<HitBox*> hitboxesSet = it->second;
            for(auto& hb : hitboxesSet)
            {
                PositionComponent pos = entity->getComponent<PositionComponent>();
                hb->oldX = hb->x;
                hb->oldY = hb->y;
                hb->x = pos.getX() + hb->xOffset;
                hb->y = pos.getY() + hb->yOffset;
            }
        }
    }
    void render(Camera* cam)
    {
        if(Game::debugMode) {
            SDL_SetRenderDrawColor(Game::renderer, 0, 255, 0, 255);
            for(auto it = hitboxes.begin(); it != hitboxes.end(); it++)
            {
                std::vector<HitBox*> hitboxesSet = it->second;
                for(auto& hb : hitboxesSet)
                {
                    SDL_Rect r = hb->hitBoxToSDL_Rect();
                    r.x -= cam->getX();
                    r.y -= cam->getY();
                    SDL_RenderDrawRect(Game::renderer, &r);
                }
            }
        }
    }

    void addHitBoxType(HitBoxType type)
    {
        if(hitboxes.find(type) == hitboxes.end()) hitboxes.insert(std::make_pair(type, std::vector<HitBox*>()));
    }

    HitBox* addHitBox(HitBoxType type)
    {
        auto it = hitboxes.find(type);
        if(it == hitboxes.end()) return nullptr;

        auto hitboxesVector = it->second;
        HitBox* hitBox = new HitBox();
        hitboxesVector.push_back(hitBox);

        return hitBox;
    }

    void addHitBox(HitBoxType type, HitBox* hitBox)
    {
        auto it = hitboxes.find(type);
        if(it == hitboxes.end()) return;

        it->second.push_back(hitBox);
    }

    std::vector<HitBox*> getHitBoxes(HitBoxType type)
    {
        return hitboxes.find(type)->second;
    }

    void reajustEntityPosFromHitbox(HitBox* hb)
    {
        auto& pos = entity->getComponent<PositionComponent>();
        pos.setPos(hb->x + hb->xOffset, hb->y + hb->yOffset);
    }
};