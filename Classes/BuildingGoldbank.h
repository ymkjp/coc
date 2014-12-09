#ifndef __BUILDING_GOLDBANK_H__
#define __BUILDING_GOLDBANK_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Definitions.h"
#include "BuildingResource.h"

class BuildingGoldbank : public BuildingResource
{
public:
    static BuildingGoldbank* create(Tmx* tmx, Vec2 coord) {
        auto p = new BuildingGoldbank();
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
        return GoldBank;
    };
    
    
    void damagedEffect();
};

#endif // __BUILDING_GOLDBANK_H__
