#ifndef __BUILDING_WALL_H__
#define __BUILDING_WALL_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Definitions.h"
#include "Building.h"

class BuildingWall : public Building
{
public:
    static BuildingWall* create(Tmx* tmx, Vec2 coord) {
        auto p = new BuildingWall();
        if (p->init(tmx, coord)) {
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return nullptr;
    }
    
    // 親クラスで virtual pure 規定されたメソッド
    BuildingType getType()
    {
        return Wall;
    };
};

#endif // __BUILDING_WALL_H__
