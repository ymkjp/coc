#ifndef __MAP_NAVIGATOR_H__
#define __MAP_NAVIGATOR_H__

#include "cocos2d.h"
USING_NS_CC;

#include <set>
#include <stack>
#include <array>
#include <math.h>

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
    virtual std::stack<Vec2>* navigate(Vec2 startPoint, Vec2 goalPoint);
    virtual AStar* findLastNode(Vec2 startPoint, Vec2 goalPoint);
    AStar* openedNode = nullptr;

protected:
    Tmx* tmx;
    const std::set<Vec2> surround {Vec2(-1,1),Vec2(-1,0),Vec2(-1,-1),Vec2(0,-1),Vec2(1,-1),Vec2(1,0),Vec2(1,1),Vec2(0,1)};
    const std::array<std::string, BLOCK_LAYERS_NUM> blockLayers =  {"TownHall", "ElixerTank", "GoldBank", "Canon", "TrenchMortar", "ArcherTower", "Wall"};
    std::stack<Vec2> pathToGoal;
    bool isTravelable(float posX, float posY);
};

#endif // __MAP_NAVIGATOR_H__
