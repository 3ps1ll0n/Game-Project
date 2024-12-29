#pragma once

#include "ECS.hpp"
#include "Game.hpp"
#include "HitBoxComponent.hpp"
#include "Utilities.hpp"

#include <memory>
#include <vector>
#include <fstream>
#include <filesystem>
#include <string>
#include <iostream>
#include <json/json.h>
#include <algorithm>
#include <iterator>

std::string path("assets/TileMap/");

struct Tile
{
    char id = 0;
    bool canCollide = 0;

    Tile()
    :id(0), canCollide(false){}
};

enum Direction
{
    UNDEFINED = 0,
    NORTH = 1,
    SOUTH = -1,
    EAST = 2,
    WEST = -2
};

struct Door
{
    int x;
    int y;
    Direction facing;
};

using TileMap = std::vector<std::vector<Tile>>;

class TileMapComponent : public Component
{
private:
    TileMap tileMap;
    int tileSize =  64;
    int seed;
    TextureContainer textures;
    //std::vector<Entity> tileEntity;
public:
    TileMapComponent(/* args */) {}
    ~TileMapComponent() {}

    void init()
    {
        tileMap = std::vector<std::vector<Tile>>(10, std::vector<Tile>(10, Tile()));
        textures = TextureContainer();
        //tileEntity = {};
        seed = time(0);
        tileSize = 64;
        std::srand(seed);

        for(int i = 0; i<tileMap[0].size(); i++ )
        {
            tileMap[tileMap.size() - 1][i].canCollide = true;
            tileMap[tileMap.size() - 1][i].id = 1;
        }
        for(int i = 0; i<tileMap.size(); i++ )
        {
            tileMap[i][tileMap[i].size() - 1].canCollide = true;
             tileMap[i][tileMap[i].size() - 1].id = 1;
        }
    }

    void update(double dt){}

    void render(Camera* cam)
    {
        SDL_Rect dstRect = {0, 0, tileSize, tileSize};
        for(int i = cam->getY() / tileSize; i < (cam->getY() + cam->getH()) / tileSize; i++)
        {
            dstRect.y = (i * tileSize) - cam->getY();
            for(int j = cam->getX() / tileSize; j < (cam->getX() + cam->getW()) / tileSize + 1; j++)
            {
                auto tile = &tileMap[i][j];
                dstRect.x = (j * tileSize) - cam->getX();

                /*if(tile->id == 4) SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0 ,255);
                else if (tile->id == 2) SDL_SetRenderDrawColor(Game::renderer, 210, 180, 140, 255);
                else if (tile->id == 1) SDL_SetRenderDrawColor(Game::renderer, 210, 180, 0, 255);
                else if (tile->id == 5) SDL_SetRenderDrawColor(Game::renderer, 0, 0, 255, 255);
                else SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 0);
                if(tile->id != 0) SDL_RenderFillRect(Game::renderer, &dstRect);*/
                if(tile->id != 0)
                    SDL_RenderCopy(Game::renderer, textures.getTexture(tile->id - 1), NULL, &dstRect);

            }
        }
    }

    /*void applyCollider(std::vector<std::unique_ptr<Entity>> entities)
    {
        for(std::unique_ptr<Entity>& e : entities)
        {
            if(e->hasComponent<HitBoxComponent>())applyCollider(e.get());
        }
    }*/

    bool applyCollider(Entity* e)
    {
        if(!e->hasComponent<HitBoxComponent>()) return false;

        std::vector<HitBox*> entityHitboxes = e->getComponent<HitBoxComponent>().getHitBoxes(BODY);

        for(auto hb : entityHitboxes)
        {
            int leftTile = hb->x / tileSize;
            int rightTile = (hb->x + hb->w) / tileSize;
            int topTile = hb->y / tileSize;
            int bottomTile = (hb->y + hb->h) / tileSize;

            for(int i = leftTile; i <= rightTile; i++)
            {
                for(int j = topTile; j <= bottomTile; j++)
                {
                    Tile t = tileMap[j][i];
                    if(t.canCollide)
                    {
                        //hb->moveBack();
                        //e->getComponent<HitBoxComponent>().reajustEntityPosFromHitbox(hb);
                        return true;
                    }
                }
            }   
        }
        return false;
    }

    void Generate(int size, int nbreRoom)
    {
        tileMap = TileMap(size, std::vector<Tile>(size, Tile()));
        std::vector<Door> doorsToGenerate = {};

        std::ifstream roomFile (path + "default.json", std::ifstream::binary);
        Json::Value root;
        roomFile >> root;

        copyRoomToMap(getRoomFromJson(root), (size/2 - root["layers"][0]["width"].asInt()/2), (size/2 - root["layers"][0]["height"].asInt()/2), doorsToGenerate);

        std::vector<std::string> allFiles = {};

        for(const auto& entry : std::filesystem::directory_iterator(path)) //Find all Tile Map file
        {
            allFiles.push_back(path + entry.path().string().substr(path.find_last_of("/\\") + 1));
            Log::writeLog("debugLog.txt", allFiles[allFiles.size() - 1] + " - Initialized");
        }

        /*TileMap room;
            std::string roomPath = allFiles[2];
            roomFile = std::ifstream(roomPath, std::ifstream::binary);
            roomFile >> root;
            //Json::Value to TileMap
            room = getRoomFromJson(root);

        for(int i = 0; i < 4; i ++)
        {
            room = rotateRoom(room);
            copyRoomToMap(room, (size/2 - root["layers"][0]["width"].asInt()/2) + 25 * (i + 1), (size/2 - root["layers"][0]["height"].asInt()/2), doorsToGenerate);
        }*/
        
        for(int i = 1; i < nbreRoom; i++)
        {
            TileMap room;
            //Get a random room

            std::string roomPath = allFiles[std::rand() % allFiles.size()];
            roomFile = std::ifstream(roomPath, std::ifstream::binary);
            roomFile >> root;
            //Json::Value to TileMap
            room = getRoomFromJson(root);
            if(addRoomToDungeon(room, doorsToGenerate) == 1 && Game::debugMode) 
            {
                for(int j = 0; j < doorsToGenerate.size(); j++)
                    tileMap[doorsToGenerate[j].y][doorsToGenerate[j].x].id = 0;
                Log::writeLog("debugLog.txt", "ERROR WHILE GENERATING DUNGEON, INTERUPTION IN MID PROCESS");
                return;
            }
            Log::writeLog("debugLog.txt", roomPath + " - " + std::to_string(i) + "/" + std::to_string(nbreRoom) + " Generated");
        }
        for(int j = 0; j < doorsToGenerate.size(); j++) //Clear unused Doors
        {
            tileMap[doorsToGenerate[j].y][doorsToGenerate[j].x].id = 1;
            tileMap[doorsToGenerate[j].y][doorsToGenerate[j].x].canCollide = true;
        }

        resizeAllDoors();
        bridgeBetweenRooms();
    }

    TileMap getRoomFromJson(Json::Value root)
    {
        auto map = root["layers"][0]["data"];
        int h = root["layers"][0]["height"].asInt();
        int w = root["layers"][0]["width"].asInt();

        TileMap room = TileMap(h, std::vector<Tile>(w, Tile()));

        for(int i = 0; i < h; i++){
            for(int j = 0; j < w; j++)
            {
                room[i][j].id = map[j + (i*w)].asInt();
                if(room[i][j].id == 1 || room[i][j].id == 5) room[i][j].canCollide = true;
            }
        }

        return room;
    }

    int addRoomToDungeon(TileMap room, std::vector<Door>& doorsToGenerate)
    {
        std::vector<Door> ddd = {};
        std::copy(doorsToGenerate.begin(), doorsToGenerate.end(), std::back_inserter(ddd));
        bool roomHasBeenAdded = false;
        while(!roomHasBeenAdded){
            if(ddd.size() <= 0) return 1; // check if they are still some door availlable

            int rDoor = std::rand() % ddd.size(); 

            std::unique_ptr<Door> currentDoor = std::make_unique<Door>(ddd[ rDoor ]); //select a random door...
            ddd.erase(ddd.begin() + rDoor); //and erase it

            std::vector<Door> currentRoomDoors = {};

            room = randomlyRotateRoom(room);

            for(int k = 0; k < 4 && !roomHasBeenAdded; k++) //To test every possible rotattion
            {
                room = rotateRoom(room);
                currentRoomDoors.clear();
                getDoors(room, currentRoomDoors); 
                for(int i = 0; i < currentRoomDoors.size() && !roomHasBeenAdded; i++)
                {
                    if(currentDoor->facing == -currentRoomDoors[i].facing) 
                    {
                        int xPos = currentDoor->x - currentRoomDoors[i].x; //X position of the top left corner of the room //(room[0].size() - currentRoomDoors[i].x);
                        int yPos = currentDoor->y - currentRoomDoors[i].y; //Y position of the top left corner //(room.size() - currentRoomDoors[i].y)
                        
                        if(abs(currentDoor->facing) == 1) yPos -= 2*(currentDoor->facing);
                        else if(abs(currentDoor->facing) == 2) xPos += currentDoor->facing;
                        /*copyRoomToMap(room, xPos, yPos, doorsToGenerate); //Copy it to the tileMap
                        clearUsedDoor(doorsToGenerate);
                        std::cout << xPos << " | " << yPos << " - " << printFacing(currentDoor->facing) << " = " << printFacing(currentRoomDoors[i].facing) << std::endl;
                        return 0;*/

                        if(checkIfItsClear(room, xPos, yPos)) { //If room has place to generate...
                            copyRoomToMap(room, xPos, yPos, doorsToGenerate); //Copy it to the tileMap
                            roomHasBeenAdded = true;
                            break;
                        } //Else, retry
                    }
                }
            }
            clearUsedDoor(doorsToGenerate);
        } 
        return 0;
    }

    void copyRoomToMap(TileMap room, int xOffset, int yOffset, std::vector<Door>& doors)
    {
        for(int i = 0; i < room.size(); i++)
        {
            for(int j = 0; j < room[i].size(); j++)
            {
                if(room[i][j].id != 0)tileMap[i + yOffset][j + xOffset] = room[i][j];
            }
        }
        getDoors(room, doors, xOffset, yOffset);
    }

    TileMap randomlyRotateRoom(TileMap& room)
    {
        int nbre = std::rand() % 4;
        TileMap rotatedRoom = room;
        for(int i = 0; i < nbre; i++)
        {
            rotatedRoom = rotateRoom(rotatedRoom);
        }

        return rotatedRoom;
    }

    TileMap rotateRoom(TileMap& room)
    {
        TileMap rotatedRoom = TileMap(room[0].size(), std::vector<Tile>(room.size(), Tile()));
        for(int i = 0; i < room.size(); i++)
        {
            for(int j = 0; j < room[i].size(); j++)
            {
                rotatedRoom[rotatedRoom.size() - 1 - j][i] = room[i][j];
                //std::cout << rotatedRoom.size() - 1 - j << " | " << rotatedRoom[i].size() - 1 - i << std::endl;
            }
        }
        //printM(room);
        //printM(rotatedRoom);
        return rotatedRoom;
    }

    bool checkIfItsClear (TileMap& room, int x, int y)
    {
        for(int i = 1; i < room.size() - 1; i++)
        {
            for(int j = 1; j < room[i].size() - 1; j++)
            {
                if(tileMap[i + y][j + x].id != 0 && room[i][j].id != 0) return false;
            }
        }
        return true;
    }

    void getDoors(TileMap& room, std::vector<Door>& allDoors, int xOffset = 0, int yOffset = 0)
    {
        for (int i = 0; i < room.size(); i++)
        {
            for(int j = 0; j < room[i].size(); j++)
            {
                if(room[i][j].id == 4) 
                {
                    allDoors.push_back({j + xOffset, i + yOffset, UNDEFINED});
                    if( i-1 < 0 || room[i-1][j].id == 0) allDoors[allDoors.size() - 1].facing = NORTH;
                    else if(i+1 >= room.size() || room[i+1][j].id == 0) allDoors[allDoors.size() - 1].facing = SOUTH;
                    else if(j-1 < 0 || room[i][j-1].id == 0) allDoors[allDoors.size() - 1].facing = WEST;
                    else if(j+1 >= room[i].size() || room[i][j+1].id == 0) allDoors[allDoors.size() - 1].facing = EAST;
                }
            }
        }
    }

    void resizeAllDoors()
    {
        std::vector<Door> doors = {};
        for(int i = 0; i < tileMap.size(); i++)
        {
            for(int j = 0; j < tileMap[i].size(); j++)
            {
                if(tileMap[i][j].id == 4) doors.push_back({j, i, UNDEFINED});
            }
        }

        for(auto d : doors)
        {   
            //std::cout << "HERE" << std::endl;
            auto tDown = &tileMap[d.y + 1][d.x];
            auto tRight = &tileMap[d.y][d.x + 1];
            if(tDown->id == 1) {
                tDown->id = 4;
                tDown->canCollide = false;
            } else if (tRight->id == 1)
            {
            tRight->id = 4;
            tRight->canCollide = false;
            }
        }
    }

    void bridgeBetweenRooms()
    {
        for(int i = 2; i < tileMap.size() - 2; i++)
        {
            for(int j = 2; j < tileMap[i].size() - 2; j++)
            {
                if(tileMap[i][j].id == 4)
                {
                    if(tileMap[i][j + 1].id == 0 && tileMap[i][j + 2].id == 4)
                    {
                        tileMap[i][j + 1].id = 2;
                    }
                    else if(tileMap[i][j - 1].id == 0 && tileMap[i][j - 2].id == 4)
                    {
                        tileMap[i][j - 1].id = 2;
                    }
                    else if(tileMap[i + 1][j].id == 0 && tileMap[i + 2][j].id == 4)
                    {
                        tileMap[i + 1][j].id = 2;
                    }
                    else if(tileMap[i - 1][j].id == 0 && tileMap[i - 2][j].id == 4)
                    {
                        tileMap[i - 1][j].id = 2;
                    }
                }
            }
        }

        for(int i = 2; i < tileMap.size() - 2; i++)
        {
            for(int j = 2; j < tileMap[i].size() - 2; j++)
            {
                if(tileMap[i][j].id == 0)
                {
                    if(tileMap[i + 1][j].id == 1 && tileMap[i - 1][j].id == 1 && (tileMap[i][j + 1].id == 2 || tileMap[i][j - 1].id == 2 ))
                    {
                        tileMap[i][j].id = 1;
                        tileMap[i][j].canCollide = true;
                    }
                    else if(tileMap[i][j + 1].id == 1 && tileMap[i][j - 1].id == 1 && (tileMap[i + 1][j].id == 2 || tileMap[i - 1][j].id == 2 ))
                    {
                        tileMap[i][j].id = 1;
                        tileMap[i][j].canCollide = true;
                    }
                }
            }
        }
    }

    void assignProperID()
    {
        
    }

    void clearUsedDoor(std::vector<Door>& allDoors)
    {
        for(int i = 0; i < allDoors.size(); i++)
        {
            int x = allDoors[i].x;
            int y = allDoors[i].y;
            if( (x-2 < 0 || tileMap[y][x-2].id != 0) &&
                (x+2 >= tileMap[0].size() || tileMap[y][x+2].id != 0) &&
                (y-2 < 0 || tileMap[y-2][x].id != 0) &&
                (y+2 >= tileMap.size() || tileMap[y+2][x].id != 0)
            ) 
            {
                allDoors.erase(allDoors.begin() + i);
                i--;
            }
        }
    }
    
    int getRelativePos(double nbre){return nbre / tileSize;}
    int getSize(){return tileMap.size();}
    int getTileSize(){return tileSize;}

    std::string printFacing(Direction d){
        switch (d)
        {
        case WEST:
            return "WEST";
            break;
        case EAST:
            return "EAST";
            break;
        case NORTH:
            return "NORTH";
            break;
        case SOUTH:
            return "SOUTH";
            break;
        default:
            break;
        }
        return "ERROR";
    }

    /*void printM(TileMap m)
    {
        for(auto r : m)
        {
            std::cout << "[ ";
            for(auto t : r)
            {
                int id = t.id;
                std::cout << (int)t.id << " ";
            }
            std::cout << "]" << std::endl;
        }
            std::cout << "" << std::endl;
    }*/
};
