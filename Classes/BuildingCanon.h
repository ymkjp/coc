#ifndef __BUILDING_CANON_H__
#define __BUILDING_CANON_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Definitions.h"
#include "BuildingDefense.h"

class BuildingCanon : public BuildingDefense
{
public:
    float maxRange = 9;
    
    Vec2 adjustedBulletPos = Vec2::ZERO;
    
    static BuildingCanon* create(Tmx* tmx, Vec2 coord) {
        auto p = new BuildingCanon();
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
        return Canon;
    }
    
    float getMaxRange()
    {
        return maxRange;
    }
    
    // Defense を上書き
    void attack();
    void expandAndShrink();
    void shoot();
    void initOwn();
    
protected:
    const float ADJUST_SMALL   = 20;
    const float ADJUST_REGULAR = 40;
    const float ADJUST_LARGE   = 60;
};

#endif // __BUILDING_CANON_H__
