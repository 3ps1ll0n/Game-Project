#pragma once

#include "ECS.hpp"

class StatsComponent : public Component
{
private:
    double speed, maxHP, hp, def, atk;
public:
    StatsComponent() {};
    ~StatsComponent() {};

    void init() 
    {
        speed = 700;
        maxHP = 1;
        hp = maxHP;
        def = 1;
        atk = 1;
    };
    void update(double dt) {};
    void render(Camera* cam) {};

    double getSpeed(){return speed;}
    double getMaxHP(){return maxHP;}
    double getDef(){return def;}
    double getAtk(){return atk;}
};