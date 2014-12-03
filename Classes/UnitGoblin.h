#ifndef __UNIT_GOBLIN_H__
#define __UNIT_GOBLIN_H__

#include "Unit.h"

class UnitGoblin : public Unit
{
public:
    static UnitGoblin* create(Tmx* tmx, Vec2 coord) {
        auto p = new UnitGoblin();
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
        return Goblin;
    }
    
    __String getUnitNameString()
    {
        return "Goblin";
    }
};

#endif // __UNIT_GOBLIN_H__
