#ifndef __BUILDING_DEFENSE_H__
#define __BUILDING_DEFENSE_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Building.h"

class BuildingDefense : public Building
{
public:
    float minRange = 0;
    float maxRange = 0;
    Unit* targetUnit;
};

#endif // __BUILDING_DEFENSE_H__
