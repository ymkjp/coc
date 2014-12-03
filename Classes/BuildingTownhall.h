#ifndef __BUILDING_TOWNHALL_H__
#define __BUILDING_TOWNHALL_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Definitions.h"
#include "Building.h"

class BuildingTownhall : public Building
{
public:
    static BuildingTownhall* create(Tmx* tmx, Vec2 coord) {
        auto p = new BuildingTownhall();
        if (p->init(tmx, coord)) {
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return nullptr;
    }
    
    // Building クラスで virtual pure 規定されたメソッド
    BuildingType getType()
    {
        return TownHall;
    }
};

#endif // __BUILDING_TOWNHALL_H__
