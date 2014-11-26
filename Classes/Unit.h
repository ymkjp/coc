#ifndef __UNIT_H__
#define __UNIT_H__

#include "cocos2d.h"
USING_NS_CC;

#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

#include "Tmx.h"
#include "Building.h"
#include "MapNavigator.h"

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
        NorthEast,
        East,
        SouthEast,
        South,
        SouthWest,
        West,
        NorthWest,
    };
    
    Sprite* sprite;
    Node* unitNode;
    virtual bool init(Tmx* tmx, Unit::__TYPE unitType, Vec2 coord);
    static Unit* create(Tmx* tmx, Unit::__TYPE unitType, Vec2 coord) {
        auto p = new Unit();
        if (p->init(tmx, unitType, coord)) {
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return nullptr;
    }
    const static std::map<Vec2, __COMPASS> compassByCoords;

    Node* createAnimatedNode(Vec2 posDiff);
    void animateNode();
    void play();
    void attack();
    void update( float frame );
    
    virtual Vec2 findGoalCoord(Vec2 pos, Building::__CATEGORY targetType);
    
    __TYPE type;
    __STATUS status;
    __ACTION action;
    __COMPASS compass;
    Vec2 coord;
    Vec2 posDiff;
    
protected:
    Tmx* tmx;
    timeline::ActionTimeline* actionTimeline;

    
    bool isNextCoord(float num);
};

#endif // __UNIT_H__
