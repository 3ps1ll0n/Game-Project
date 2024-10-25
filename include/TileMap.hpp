#include "ECS.hpp"
#include "Components.hpp"

#include <vector>

class TileMap : public Entity
{
private:
    TileMapComponent* tileMap;
public:
    TileMap()
    {
        tileMap = &addComponent<TileMapComponent>();
    }
    ~TileMap() {}
};
