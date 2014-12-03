#ifndef __MAP_NAVIGATOR_H__
#define __MAP_NAVIGATOR_H__

#include <iostream>
#include <thread>
#include <future>

#include "cocos2d.h"
USING_NS_CC;

#include <set>
#include <stack>
#include <array>

#include "Definitions.h"
#include "Tmx.h"

struct AStar
{
    enum __STATUS
    {
        NONE,
        OPEN,
        CLOSE,
        UNABLE,
    };
    __STATUS status;
    float cost;
    float distance;
    AStar *parent;
    Vec2 pos;
    float GetScore()
    {
        return cost + distance;
    }
};

class MapNavigator : public Ref
{
public:
    virtual bool init(Tmx* tiledMap);
    static MapNavigator* create(Tmx* tiledMap) {
        auto p = new MapNavigator();
        if (p->init(tiledMap)) {
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return nullptr;
    }
    virtual PathToGoal navigate(Vec2 startPoint, Vec2 goalPoint);
    bool isTravelable(float posX, float posY);

    AStar* path;
    bool isOverSteps = false;

protected:
    Tmx* tmx;
    std::mutex mtx;
    const Vec2Set surround {Vec2(-1,1),Vec2(-1,0),Vec2(-1,-1),Vec2(0,-1),Vec2(1,-1),Vec2(1,0),Vec2(1,1),Vec2(0,1)};
    PathToGoal pathToGoal;
    
    AStar worldGrid[WORLD_MAP_WIDTH][WORLD_MAP_HEIGHT] = {};
    Vec2Set openSet;
    
    AStar plainNode = {};
    AStar startNode = {};
    
    AStar* openedNode = nullptr;
    AStar* bestNode = nullptr;
};

#endif // __MAP_NAVIGATOR_H__
