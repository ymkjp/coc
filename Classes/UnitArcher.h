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
    
    void animateNode();
    UnitType getUnitType();
    
    Vec2 findPointToGo();
    
    Node* getActingNode();
    timeline::ActionTimeline* getActionTimeline();
};

#endif // __UNIT_ARCHER_H__
