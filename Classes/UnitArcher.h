#ifndef __UNIT_ARCHER_H__
#define __UNIT_ARCHER_H__

#include "Unit.h"

class UnitArcher : public Unit
{
public:
    static UnitArcher* create(Tmx* tmx, Vec2 coord) {
        auto p = new UnitArcher();
        if (p->init(tmx, coord)) {
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return nullptr;
    }
    
    UnitType getUnitType();

    __String createFilename();
    
    std::vector<Vec2> getSurroundedCoords(Vec2 targetCoord);
    
};

#endif // __UNIT_ARCHER_H__
