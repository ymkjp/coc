#ifndef __UNIT_H__
#define __UNIT_H__

#include "cocos2d.h"
USING_NS_CC;

#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

#include "Definitions.h"
#include "AudioManager.h"
#include "CsbCacher.h"
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
    };
    
    // Z値が大きければ上に表示される
    enum LocalZOrder {
        ShadowOrder = 1,
        GhostOrder,
        MotionOrder,
        LifeGageOrder,
    };
    
    enum Tag {
        PlayingSequence = 1,
        ShadowTag,
        MotionTag,
        MotionActionTag,
        LifeGageTag,
        LifeGageActionTag,
        ArrowTag,
    };
    
    virtual bool init(Tmx* tmx, Vec2 coord);

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
    virtual Vec2 findNearestWallGoalCoord();
    virtual std::vector<Vec2> getSurroundedCoords(Vec2 targetCoord);
    virtual Node* getActingNode();
    virtual timeline::ActionTimeline* getActionTimeline();
    virtual std::string createFilename();
    virtual void shoot();
    
    UnitType type;
    __STATUS status;
    __ACTION action;
    Compass compass;
    Vec2 coord;
    Vec2 posDiff;
    
    void attacked(float damage);
    void finished();
    
protected:
    const Vec2 ERROR_COORD = Vec2(-1,-1);
    const float WAITING_SEC = 3;
    const float SOON = 0.01;
    float hitpoints;
    float damagePerAttack;
    float attackSpeed;
    
    Tmx* tmx;
    
    Building* targetBuilding;
    virtual bool isNextCoord(float num);
    virtual void setCompass(Vec2 prevCoord, Vec2 nextCoord);
    void updateDirection();
    
    void die();
    void finishBattle();
    void addGrave();
    
    virtual void playDeathVoice();
    
    virtual void playStartAttackingVoice();
    
    
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
    
    const std::map<UnitType, float> shadowPosYByType =
    {
        {Archer, -12},
        {Barbarian, -12},
        {Giant, -16},
        {Goblin, -8},
        {Wallbreaker, -8},
    };
    
    const std::map<UnitType, float> shadowScaleByType =
    {
        {Archer, 1.8},
        {Barbarian, 1.8},
        {Giant, 2.6},
        {Goblin, 1.8},
        {Wallbreaker, 1.8},
    };
    
    const std::map<UnitType, float> deployedExtensionXLengthByType =
    {
        {Archer, 0.6},
        {Barbarian, 0.6},
        {Giant, 0.8},
        {Goblin, 0.6},
        {Wallbreaker, 0.6},
    };
    
    const std::map<UnitType, float> deployedExtensionYLengthByType =
    {
        {Archer, 1.2},
        {Barbarian, 1.2},
        {Giant, 1.1},
        {Goblin, 1.2},
        {Wallbreaker, 1.2},
    };

    /*
     コメントは wiki に記載されている値
     @link http://clashofclans.wikia.com/wiki/Clash_of_Clans_Wiki
     **/
    const std::map<UnitType, float> movementSpeedByType =
    {
        {Archer, 0.5},      // 24
        {Barbarian, 0.65},  // 16
        {Giant, 0.8},       // 12
        {Goblin, 0.4},      // 32
        {Wallbreaker, 0.5}, // 24
    };
    
    const std::map<UnitType, float> lifeGageYPosByType =
    {
        {Archer, 26},
        {Barbarian, 26},
        {Giant, 40},
        {Goblin, 24},
        {Wallbreaker, 26},
    };
    
    float getFullHitPoints()
    {
        return hitpointsByType.at(type);
    }
    
    void pushTobuildingAttackRange(Vec2 coord);
    void testAdd(__String fileName, Vec2 pos);

    std::tuple<__ACTION,Compass> motionStatus;
    void updateMotionNode(bool b_loop);
    
    void updateLifeGage();
    
    void hideLifeGage(float frame)
    {
        auto node = this->getChildByTag(LifeGageTag);
        if (node) {
            node->setVisible(false);
        }
    };
    
    bool alreadyMarked = false;
    void putTargetMark();
    
    
};

#endif // __UNIT_H__
