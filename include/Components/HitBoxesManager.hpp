#pragma once

#include <vector>
#include <Camera.hpp>
#include <iostream>

#include "Parameters.hpp"
#include "Utilities.hpp"
#include "HitBoxComponent.hpp"

enum TEAM {
    PLAYER,
    ENEMIES
};

struct DamagingHitBox
{
    double x;
    double y;
    double radius;

    double power;
    double knockback;

    double lifepan;
    double lifetime = 0.0;

    bool enable = true;

    TEAM team;

    DamagingHitBox(double x, double y, double radius, double power, double knockback, double lifepan)
    :x(x), y(y), radius(radius), power(power), knockback(knockback), lifepan(lifepan)
    {}

    ~DamagingHitBox() = default;

    void update(double dt)
    {
        lifetime += dt;
        if(lifetime >= lifepan) enable = false;
    }
};

class HitBoxesManager
{
private:
    inline static std::vector<DamagingHitBox> damageCollision = {};
public:
    HitBoxesManager() 
    {}
    ~HitBoxesManager() {}
    static DamagingHitBox* createDamageHitBox(double x, double y, double radius, double power, double knockback, double lifepan)
    {
        damageCollision.push_back(DamagingHitBox(x, y, radius, power, knockback, lifepan));
        return &damageCollision[damageCollision.size() - 1];
    }

    void render(SDL_Renderer* renderer, Camera* cam) 
    {
        if(Parameters::DebbugMod){
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            for(DamagingHitBox& hb : damageCollision){
                DrawCircle(renderer, hb.x - cam->getX(), hb.y - cam->getY(), hb.radius);
            }
        }
    }

    void update(double dt)
    {
        for(int i = 0; i < damageCollision.size(); i++){
            DamagingHitBox* hb = &damageCollision[i];
            hb->update(dt);
            if(!hb->enable)
            {
                damageCollision.erase(damageCollision.begin() + i);
                i--;
            }
        }
    }
    
    bool detectCollision(Entity* entity)
    {
        if(!entity->hasComponent<HitBoxComponent>()) return false;
        
        auto hbc = &entity->getComponent<HitBoxComponent>();

        for(DamagingHitBox& dhb : damageCollision)
        {
            for(auto hb : hbc->getHitBoxes())
            {
                float testX = dhb.x;
                float testY = dhb.y;

                SDL_Rect rect = hb.hitBoxToSDL_Rect();

                if(dhb.x < rect.x) testX = rect.x;
                else if(dhb.x > rect.x + rect.w) testX = rect.x + rect.w;

                if(dhb.y < rect.y) testY = rect.y;
                else if(dhb.x > rect.y + rect.h) testY = rect.y + rect.h;

                float distX = dhb.x - testX;
                float distY = dhb.y - testY;
                float distance = sqrt((distX*distX) + (distY*distY));

                if(distance < dhb.radius)
                {
                    return true;
                }
            }
        }

        return false;
    }
};