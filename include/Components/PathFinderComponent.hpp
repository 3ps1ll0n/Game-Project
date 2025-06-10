#pragma once

#include "ECS.hpp"
#include "Utilities.hpp"
#include "HitBoxComponent.hpp"
#include "TileMapComponent.hpp"
#include "Parameters.hpp"

class PathFinderComponent : public Component
{
private:
    Point* targetPos;
    std::vector<Point> pathPoints; 
    Entity* tileMap;
    PositionComponent* entityPos;
public:
    PathFinderComponent() {}
    ~PathFinderComponent() {}

    void init() 
    {
        targetPos = new Point(0, 0);
        pathPoints = {};
        entityPos = &entity->getComponent<PositionComponent>();
    }

    void update(double dt)
    {
        
    }

    void render(SDL_Renderer* renderer, Camera* cam)
    {
        if(Parameters::DebbugMod)
        {
            HitBox* hb = entity->getComponent<HitBoxComponent>().getHitBox(0);
            for(int i = 1; i < pathPoints.size(); i++){
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderDrawLine(renderer, pathPoints[i].x - cam->getX(), pathPoints[i].y - cam->getY(), pathPoints[i - 1].x - cam->getX(), pathPoints[i- 1].y - cam->getY());
            }
            if(pathPoints.size() != 0)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderDrawLine(renderer, pathPoints[pathPoints.size() - 1].x - cam->getX(), pathPoints[pathPoints.size() - 1].y - cam->getY(), targetPos->x - cam->getX(), targetPos->y - cam->getY());
            }
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderDrawLine(renderer, entityPos->getX() + hb->w/2 - cam->getX(), entityPos->getY() + hb->h/2 - cam->getY(), getNextPoint()->x - cam->getX(), getNextPoint()->y - cam->getY());
        }
    }

    void setTileMap(Entity* tm)
    {
        tileMap = tm;
    }

    Point* getNextPoint()
    {
        if(pathPoints.size() > 0) return &pathPoints[0];
        else return targetPos;
    }

    void setTargetPos(Point* point)
    {
        targetPos = point;
    }

    void calculatePath()
    {
        pathPoints.clear();
        /*TileMapComponent* tm = &tileMap->getComponent<TileMapComponent>();
        Point tilePos {tm->getRelativePos(entityPos->getX() + tm->getTileSize()/2), tm->getRelativePos(entityPos->getY() + tm->getTileSize()/2)};
        Point targetPosOnGrid {tm->getRelativePos(targetPos->x + tm->getTileSize()/2), tm->getRelativePos(targetPos->y + tm->getTileSize()/2)};

        while (true)
        {
            //std::cout << tilePos.x << " : " << tilePos.y << " ---> ";
            //std::cout << tm->getRelativePos(entityPos->getX()) << " : " << tm->getRelativePos(entityPos->getY()) << std::endl;
            if(targetPosOnGrid.x - tilePos.x != 0 && targetPosOnGrid.y - tilePos.y != 0)
            { // Trace a diagonal line until it reach the same x or y coordinate as the target
                if(targetPosOnGrid.x - tilePos.x > 0)
                {
                    tilePos.x++;
                }
                else
                {
                    tilePos.x--;
                }

                if(targetPosOnGrid.y - tilePos.y > 0)
                {
                    tilePos.y++;
                }
                else
                {
                    tilePos.y--;
                }
            }

            else if ((targetPosOnGrid.x - tilePos.x != 0) != (targetPosOnGrid.y - tilePos.y != 0)){
                tilePos *= tm->getTileSize();
                tilePos += tm->getTileSize()/2;
                pathPoints.push_back(tilePos);
                break;
            }
            else{ 
                break;
            }
        }*/

        
    }
};
