#ifndef __BUILDING_TRENCHMORTAR_H__
#define __BUILDING_TRENCHMORTAR_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Definitions.h"
#include "BuildingDefense.h"

class BuildingTrenchmortar : public BuildingDefense
{
public:
    float minRange = 4;
    float maxRange = 11;
    
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
    
    
    float getMinRange()
    {
        return minRange;
    }
    
    float getMaxRange()
    {
        return maxRange;
    }
    
    // Building 上書き
    void attack();
    void expandAndShrink();
    void shoot();
    void initOwn();
    
protected:
    const float SPLASH_RANGE = 2;
    
    inline bool isImpactRange(Vec2 impactCoord, Vec2 unitCoord)
    {
        auto coordDiff = impactCoord - unitCoord;
        float coordDiffX = fabs(coordDiff.x);
        float coordDiffY = fabs(coordDiff.y);
        
        return (0 <= coordDiffX && coordDiffX <= SPLASH_RANGE)
        && (0 <= coordDiffY && coordDiffY <= SPLASH_RANGE);
    }
};

#endif // __BUILDING_TRENCHMORTAR_H__
