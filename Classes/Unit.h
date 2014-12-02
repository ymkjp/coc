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
    virtual bool init(Tmx* tmx, Vec2 coord);
    const static std::map<Vec2, __COMPASS> compassByCoords;

    const std::map<UnitType, Building::__CATEGORY> attackType = {
        {Barbarian, Building::Melee},
//        {Barbarian, Building::TownHalls}, /** For debug */
        {Archer, Building::Melee},
        {Giant, Building::Defenses},
        {Goblin, Building::Resources},
        {Wallbreaker, Building::Walls},
    };
    virtual UnitType getUnitType() = 0;

    virtual void play(float frame);
    virtual void update( float frame );
    virtual void attack(float frame);
    virtual void startAttacking();
    
    
    // 必要に応じて子クラスで定義
    virtual Vec2 findPointToGo();
    virtual Vec2 findNearestWallGoalPoint();
    virtual std::vector<Vec2> getSurroundedCoords(Vec2 targetCoord);
    virtual Node* getActingNode();
    virtual timeline::ActionTimeline* getActionTimeline();
    
    // 子クラスで定義
    virtual __String createFilename() = 0;
    
    UnitType type;
    __STATUS status;
    __ACTION action;
    __COMPASS compass;
    Vec2 coord;
    Vec2 posDiff;
    
protected:
    Tmx* tmx;
    timeline::ActionTimeline* actionTimeline;
    timeline::ActionTimelineCache* actionTimelineCache;
    
    Building* targetBuilding;
    virtual bool isNextCoord(float num);
    virtual void setCompass(Vec2 prevCoord, Vec2 nextCoord);
    
    float damagePerSec = 0;
    
    std::vector<Vec2> getTargetCoords(Building::__CATEGORY category);
    
    void testAdd(__String fileName, Vec2 pos);
    void updateNode();
};

#endif // __UNIT_H__
