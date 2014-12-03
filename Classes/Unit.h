#ifndef __UNIT_H__
#define __UNIT_H__

#include "cocos2d.h"
USING_NS_CC;

#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

#include "Definitions.h"
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
    
    Node* unitNode;
    virtual bool init(Tmx* tmx, Vec2 coord);
    const static std::map<Vec2, Compass> compassByCoords;

    const std::map<UnitType, BuildingCategory> attackType = {
        {Barbarian, Melee},
//        {Barbarian, Building::TownHalls}, /** For debug */
        {Archer, Melee},
        {Giant, Defenses},
        {Goblin, Resources},
        {Wallbreaker, Walls},
    };
    virtual UnitType getUnitType() = 0;

    virtual void play(float frame);
    virtual void update( float frame );
    virtual void attack(float frame);
    virtual void startAttacking();
    
    
    // 必要に応じて子クラスで定義させる
    virtual Vec2 findPointToGo();
    virtual Vec2 findNearestWallGoalPoint();
    virtual std::vector<Vec2> getSurroundedCoords(Vec2 targetCoord);
    virtual Node* getActingNode();
    virtual timeline::ActionTimeline* getActionTimeline();
    virtual __String createFilename();
    
    
    UnitType type;
    __STATUS status;
    __ACTION action;
    Compass compass;
    Vec2 coord;
    Vec2 posDiff;
    
    void attacked(float damage);
    
    
protected:
    float hitpoints;
    float damagePerSec;
    
    Tmx* tmx;
    timeline::ActionTimeline* actionTimeline;
    timeline::ActionTimelineCache* actionTimelineCache;
    
    Building* targetBuilding;
    virtual bool isNextCoord(float num);
    virtual void setCompass(Vec2 prevCoord, Vec2 nextCoord);
    
    void die();
    
    std::vector<Vec2> getTargetCoords(BuildingCategory category);
    
    const std::map<UnitType, std::string> nameStringBytype =
    {
        {Archer, "Archer"},
        {Barbarian, "Barbarian"},
        {Giant, "Giant"},
        {Goblin, "Goblin"},
        {Wallbreaker, "Wallbreaker"},
    };
    
    const std::map<UnitType, float> hitpointsByType =
    {
        {Archer, 20},
        {Barbarian, 45},
        {Giant, 300},
        {Goblin, 25},
        {Wallbreaker, 20},
    };
    
    float getHitPoints()
    {
        return hitpointsByType.at(type);
    }
    
    void pushTobuildingAttackRange(Vec2 coord);
    void testAdd(__String fileName, Vec2 pos);
    void updateNode();
};

#endif // __UNIT_H__
