#ifndef __UNIT_H__
#define __UNIT_H__

#include "cocos2d.h"
USING_NS_CC;

#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

#include "Definitions.h"
#include "AudioManager.h"
#include "Tmx.h"
#include "MapNavigator.h"
#include "Building.h"

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
    
    // Z値が大きければ上に表示される
    enum LocalZOrder {
        ShadowOrder = 1,
        GhostOrder,
        MotionOrder,
    };
    
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
    virtual void shoot();
    
    UnitType type;
    __STATUS status;
    __ACTION action;
    Compass compass;
    Vec2 coord;
    Vec2 posDiff;
    
    void attacked(float damage);
    
    enum NodeTag {
        ShadowTag = 1,
        MotionTag,
        LifeGageTag,
    };
    
protected:
    float hitpoints;
    float damagePerAttack;
    float attackSpeed;
    
    
    Node* motionNode;
    timeline::ActionTimeline* motionAction;
    
    Node* lifeGageNode;
    timeline::ActionTimeline* lifeGageAction;
    
    AudioManager* audioManager;
    
    Tmx* tmx;
    timeline::ActionTimelineCache* actionTimelineCache;
    
    Building* targetBuilding;
    virtual bool isNextCoord(float num);
    virtual void setCompass(Vec2 prevCoord, Vec2 nextCoord);
    
    void die();
    void addGrave();
    
    virtual void playDeathVoice()
    {
        audioManager->playSE("unit_" + unitSmallCaseNameByType.at(type) + "_death");
    };
    
    virtual void playStartAttackingVoice()
    {
        audioManager->playSE("unit_" + unitSmallCaseNameByType.at(type) + "_attack_hit");
    };
    
    
    std::vector<Vec2> getTargetCoords(BuildingCategory category);
    
    const std::map<UnitType, float> hitpointsByType =
    {
        {Archer, 20},
        {Barbarian, 45},
        {Giant, 300},
        {Goblin, 25},
        {Wallbreaker, 20},
    };
    
    const std::map<UnitType, float> damagePerAttackByType =
    {
        {Archer, 12},
        {Barbarian, 65},
        {Giant, 38},
        {Goblin, 19},
        {Wallbreaker, 960},
    };
    
    const std::map<UnitType, float> attackSpeedByType =
    {
        {Archer, 1},
        {Barbarian, 1},
        {Giant, 2},
        {Goblin, 1},
        {Wallbreaker, 1},
    };
    
    const std::map<UnitType, float> movementSpeedByType =
    {
        {Archer, 0.4},    // 24
        {Barbarian, 0.5}, // 16
        {Giant, 0.7},     // 12
        {Goblin, 0.3},     // 32
        {Wallbreaker, 0.4}, // 24
    };
    
    
    float getFullHitPoints()
    {
        return hitpointsByType.at(type);
    }
    
    void pushTobuildingAttackRange(Vec2 coord);
    void testAdd(__String fileName, Vec2 pos);
    void updateMotionNode();
    void updateLifeGage();
    
    void hideLifeGage(float frame)
    {
        lifeGageNode->setVisible(false);
    };
    
    bool alreadyMarked = false;
    void putTargetMark();
    
    
};

#endif // __UNIT_H__
