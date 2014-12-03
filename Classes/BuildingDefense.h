#ifndef __BUILDING_DEFENSE_H__
#define __BUILDING_DEFENSE_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Definitions.h"
#include "Building.h"

class BuildingDefense : public Building
{
public:
    float minRange;
    float maxRange;
    
    static void updateAttackRangeGrid(Tmx* tmx);
    bool virtualInit();
    
    // 子クラスで再定義されてもよいメソッド
    virtual float getMinRange() {return 0;};
    virtual float getMaxRange() {return 0;};
    
    Vector<Unit*> targetUnits = {};
    
protected:
    float damagePerShot;
    float attackSpeed;
    void scan(float frame);
    void attack(Unit* unit);
    bool inAttackRange(Unit* unit);
    
    static float normalize(float number);
    
    const std::map<BuildingType, float> damagePerShotByType =
    {
        {Canon, 7.2},
        {TrenchMortar, 11},
        {ArcherTower, 11},
    };
    
    const std::map<BuildingType, float> attackSpeedByType =
    {
        {Canon, 0.8},
        {TrenchMortar, 1},
        {ArcherTower, 5},
    };
    
    float getDamagePerShot()
    {
        return damagePerShotByType.at(type);
    }
    
    float getAttackSpeed()
    {
        return attackSpeedByType.at(type);
    }

};

#endif // __BUILDING_DEFENSE_H__
