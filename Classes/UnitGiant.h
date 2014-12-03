#ifndef __UNIT_GIANT_H__
#define __UNIT_GIANT_H__

#include "Unit.h"

class UnitGiant : public Unit
{
public:
    static UnitGiant* create(Tmx* tmx, Vec2 coord) {
        auto p = new UnitGiant();
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
        return Giant;
    }
    

protected:

};

#endif // __UNIT_GIANT_H__
