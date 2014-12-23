#ifndef __BUILDING_CANON_H__
#define __BUILDING_CANON_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Definitions.h"
#include "BuildingDefense.h"

class BuildingCanon : public BuildingDefense
{
public:
    float maxRange = 9;
    int idNum;
    int times;
    Vec2 adjustedBulletPos = Vec2::ZERO;
    
    static BuildingCanon* create(Tmx* tmx, Vec2 coord) {
        auto p = new BuildingCanon();
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
        return Canon;
    }
    
    float getMaxRange()
    {
        return maxRange;
    }
    
    // Defense を上書き
    void attack();
    void expandAndShrink();
    void shoot();
    void initOwn();
    
protected:
    const float ADJUST_SMALL   = 10;
    const float ADJUST_REGULAR = 20;
    const float ADJUST_LARGE   = 30;
    const float ADJUST_HUGE    = 40;
    
    const std::map<int,Vec2> adjustingFirePos = {
        {0,  {0.0,   46.5}},
        {1,  {8.0,   47.5}},
        {2,  {17.0,  45.25}},
        {3,  {25.0,  42.25}},
        {4,  {32.0,  28.25}},
        {5,  {36.0,  33.25}},
        {6,  {38.0,  28.25}},
        {7,  {41.0,  21.25}},
        {8,  {41.0,  16.25}},
        {9,  {41.0,  13.75}},
        {10, {40.0,  5.75}},
        {11, {40.0,  -0.75}},
        {12, {37.0,  -7.25}},
        {13, {32.0,  -12.25}},
        {14, {29.0,  -18.25}},
        {15, {23.0,  -22.25}},
        {16, {17.0,  -24.25}},
        {17, {8.0,   -24.25}},
        {18, {0.0,   -26.25}},
    };
};

#endif // __BUILDING_CANON_H__
