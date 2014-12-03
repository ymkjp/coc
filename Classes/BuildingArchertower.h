#ifndef __BUILDING_ARCHERTOWER_H__
#define __BUILDING_ARCHERTOWER_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Definitions.h"
#include "BuildingDefense.h"

class BuildingArchertower : public BuildingDefense
{
public:
    float minRange = 0;
    float maxRange = 0;
    Unit* targetUnit;
    
    static BuildingArchertower* create(Tmx* tmx, Vec2 coord) {
        auto p = new BuildingArchertower();
        if (p->init(tmx, coord)) {
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return nullptr;
    }
    
    // virtual pure 規定されたメソッド
    BuildingType getType()
    {
        return ArcherTower;
    }
};

#endif // __BUILDING_ARCHERTOWER_H__
