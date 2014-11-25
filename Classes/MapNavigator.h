#ifndef __MAP_NAVIGATOR_H__
#define __MAP_NAVIGATOR_H__

#include "cocos2d.h"
USING_NS_CC;

#include <set>
#include <stack>
#include <vector>
#include <math.h>

#include "Definitions.h"

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
    virtual bool init(TMXTiledMap* tiledMap);
    static MapNavigator* create(TMXTiledMap* tiledMap) {
        auto p = new MapNavigator();
        if (p->init(tiledMap)) {
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return nullptr;
    }
    virtual std::stack<Vec2>* navigate(Vec2 startPoint, Vec2 goalPoint);
protected:
    TMXLayer *domainTMXLayer;
    TMXLayer *wallTMXLayer;
    const std::set<Vec2> surround {Vec2(-1,1),Vec2(-1,0),Vec2(-1,-1),Vec2(0,-1),Vec2(1,-1),Vec2(1,0),Vec2(1,1),Vec2(0,1)};
    static float heuristicCost(Vec2 startPoint, Vec2 goalPoint);
    std::stack<Vec2> pathToGoal;
};

#endif // __MAP_NAVIGATOR_H__
