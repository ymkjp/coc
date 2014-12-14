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


class MapNavigator : public Ref
{
public:
    virtual bool init();
    CREATE_FUNC(MapNavigator);
    virtual PathToGoal navigate(const WorldGrid* worldGrid, Vec2 startPoint, Vec2 goalPoint);
    
    std::map<std::array<Vec2,2>, PathToGoal> pathCache;
    
protected:
    std::mutex mtx;
    const Vec2Set surround {Vec2(-1,1),Vec2(-1,0),Vec2(-1,-1),Vec2(0,-1),Vec2(1,-1),Vec2(1,0),Vec2(1,1),Vec2(0,1)};
};

#endif // __MAP_NAVIGATOR_H__
