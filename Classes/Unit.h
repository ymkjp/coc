#ifndef __UNIT_H__
#define __UNIT_H__

#include "cocos2d.h"
USING_NS_CC;

#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

class Unit : public Ref
{
public:
    enum __TYPE {
        Archer = 0,
        Barbarian,
        Giant,
        Goblin,
        Wallbreaker,
    };
    enum __STATUS {
        Alive = 0,
        Died,
    };
    enum __ACTION {
        Walking = 0,
        Attacking,
        Damaged,
    };
    enum __COMPASS {
        North = 0,
        South,
        East,
        West,
    };
    
    Sprite* sprite;
    Node* unitNode;
    virtual bool init(TMXTiledMap* tiledMap, Unit::__TYPE unitType);
    static Unit* create(TMXTiledMap* tiledMap, Unit::__TYPE unitType) {
        auto p = new Unit();
        if (p->init(tiledMap, unitType)) {
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return nullptr;
    }
    
    virtual void run(Vec2 pos, Vec2 tileCoord);

protected:
    TMXTiledMap* _tiledMap;
    SpriteFrameCache* spriteFrameCache;
    Vec2 pos;

    virtual void addAnimation();
    virtual Vec2 convertToTile(Vec2 pos);
  
    __TYPE _type;
    __STATUS _status;
    __ACTION _action;
    __COMPASS _compass;
};

#endif // __UNIT_H__
