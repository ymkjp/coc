#ifndef __UNIT_BARBARIAN_H__
#define __UNIT_BARBARIAN_H__

#include "Unit.h"

class UnitBarbarian : public Unit
{
public:
    static UnitBarbarian* create(Tmx* tmx, Vec2 coord) {
        auto p = new UnitBarbarian();
        if (p->init(tmx, coord)) {
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return nullptr;
    }
    
    // 親クラスで virtual pure 規定された定義しなければならないメソッド
    UnitType getUnitType()
    {
        return Barbarian;
    }
    
    __String getUnitNameString()
    {
        return "Barbarian";
    }
};

#endif // __UNIT_BARBARIAN_H__
