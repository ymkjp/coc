#ifndef __UNIT_H__
#define __UNIT_H__

#include "cocos2d.h"
USING_NS_CC;

#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

#include "Tmx.h"
#include "Building.h"
#include "MapNavigator.h"

class Unit : public Node
{
public:
    enum __STATUS {
        Alive = 0,
        Died,
    };
    enum __ACTION {
        NoActing = 0,
        Walking,
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
    
    Node* unitNode;
    virtual bool init(Tmx* tmx, UnitType unitType, Vec2 coord);
    static Unit* create(Tmx* tmx, UnitType unitType, Vec2 coord) {
        auto p = new Unit();
        if (p->init(tmx, unitType, coord)) {
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return nullptr;
    }
    const static std::map<Vec2, __COMPASS> compassByCoords;

    const std::map<UnitType, Building::__CATEGORY> attackType = {
        {Barbarian, Building::Melee},
        {Archer, Building::Melee},
        {Giant, Building::Defenses},
        {Goblin, Building::Resources},
        {Wallbreaker, Building::Walls},
    };

    Node* createAnimatedNode(Vec2 posDiff);
    void animateNode();
    void play(float frame);
    void startAttacking();
    void attack(float frame);
    void update( float frame );
    
    Building* findTargetBuilding(Vec2 pos);
    Vec2 getPointToGo();
    
    UnitType type;
    __STATUS status;
    __ACTION action;
    __COMPASS compass;
    Vec2 coord;
    Vec2 posDiff;
    
protected:
    int count = 0;
    Tmx* tmx;
    timeline::ActionTimeline* actionTimeline;
    timeline::ActionTimelineCache* actionTimelineCache;
    
    Building* targetBuilding;
    bool isNextCoord(float num);
    
    float damagePerSec = 0;
};

#endif // __UNIT_H__
