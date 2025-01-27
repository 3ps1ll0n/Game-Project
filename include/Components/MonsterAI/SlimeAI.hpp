#pragma once

#include "ECS.hpp"

class SlimeAI : Component
{
private:
    /* data */
public:
    SlimeAI(/* args */);
    ~SlimeAI();
    void init();
    void render();
    void update();
};