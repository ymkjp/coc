#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "cocos2d.h"
USING_NS_CC;

enum BuildingType {
    ArcherTowr = 0,
    ElixerTank,
    GoldBank,
    TrenchMortar,
};

class Building : public Ref
{
public:
    Sprite* sprite;
    virtual bool init(TMXTiledMap* tiledMap, ValueMap objectInfo);
    static Building* create(TMXTiledMap* tiledMap, ValueMap objectInfo) {
        auto p = new Building();
        if (p->init(tiledMap, objectInfo)) {
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return nullptr;
    }
    virtual void addBuilding();

protected:
    TMXTiledMap* _tiledMap;
    SpriteFrameCache* cache;
    ValueMap _objectInfo;
    virtual Vec2 convertToIso(Vec2 pos);
};

#endif // __BUILDING_H__
