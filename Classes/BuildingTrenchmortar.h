#ifndef __BUILDING_TRENCHMORTAR_H__
#define __BUILDING_TRENCHMORTAR_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Definitions.h"
#include "BuildingDefense.h"

class BuildingTrenchmortar : public BuildingDefense
{
public:
    float minRange = 0;
    float maxRange = 0;
    Unit* targetUnit;
    
    static BuildingTrenchmortar* create(Tmx* tmx, Vec2 coord) {
        auto p = new BuildingTrenchmortar();
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
        return TrenchMortar;
    }
};

#endif // __BUILDING_TRENCHMORTAR_H__
