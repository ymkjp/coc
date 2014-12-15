#ifndef __BUILDING_ELIXERTANK_H__
#define __BUILDING_ELIXERTANK_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Definitions.h"
#include "BuildingResource.h"

class BuildingElixertank : public BuildingResource
{
public:
    static BuildingElixertank* create(Tmx* tmx, Vec2 coord) {
        auto p = new BuildingElixertank();
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
        return ElixerTank;
    };
    
};

#endif // __BUILDING_ELIXERTANK_H__
