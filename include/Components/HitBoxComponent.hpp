#pragma once

#include "ECS.hpp"
#include "PositionComponent.hpp"
#include "Parameters.hpp"

#include <vector>
#include <map>
#include <memory>
#include <SDL.h>

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
    std::vector<HitBox> hitboxes;
public:
    HitBoxComponent(/* args */) {};
    ~HitBoxComponent() {};

    void init()
    {
        hitboxes = {};
    }
    void update(double dt)
    {
        for(HitBox& hb : hitboxes)
        {
            PositionComponent pos = entity->getComponent<PositionComponent>();
            hb.oldX = hb.x;
            hb.oldY = hb.y;
            hb.x = pos.getX() + hb.xOffset;
            hb.y = pos.getY() + hb.yOffset;
        }
        
    }
    void render(SDL_Renderer* renderer, Camera* cam)
    {
        if(Parameters::DebbugMod) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            for(auto& hb : hitboxes)
            {
                SDL_Rect r = hb.hitBoxToSDL_Rect();
                r.x -= cam->getX();
                r.y -= cam->getY();
                SDL_RenderDrawRect(renderer, &r);
            }
        }
    }

    void addHitBox(HitBox hitBox)
    {
        hitboxes.push_back(hitBox);
    }

    std::vector<HitBox> getHitBoxes()
    {
        return hitboxes;
    }

    HitBox* getHitBox(int index)
    {
        return &hitboxes[index];
    }

    void reajustEntityPosFromHitbox(HitBox* hb)
    {
        auto& pos = entity->getComponent<PositionComponent>();
        pos.setPos(hb->x + hb->xOffset, hb->y + hb->yOffset);
    }
};