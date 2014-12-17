#ifndef __BUILDING_ARCHERTOWER_H__
#define __BUILDING_ARCHERTOWER_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Definitions.h"
#include "BuildingDefense.h"
#include "ArcherOnTower.h"

class BuildingArchertower : public BuildingDefense
{
public:
    float minRange = 0;
    float maxRange = 10;
    
    static BuildingArchertower* create(Tmx* tmx, Vec2 coord) {
        auto p = new BuildingArchertower();
        if (p->init(tmx, coord)) {
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return nullptr;
    }
    
    // virtual pure 規定されたメソッド
    BuildingType getType()
    {
        return ArcherTower;
    }
    
    float getMaxRange()
    {
        return maxRange;
    }
    

protected:
    const int numberOfArchersOnTower = 3;
    void initArchersOnTower();
    Vector<ArcherOnTower*> archersOnTower = {};
    int attackingArcherIndex = 0;
    void updateAttacingArcherIndex();
    
    // Defense を上書き
    void attack();
    void shoot();

    void updateArchersDirection();
    
    Node* bullet;
    Compass direction;
    
};

#endif // __BUILDING_ARCHERTOWER_H__
