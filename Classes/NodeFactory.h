#ifndef __NODE_FACTORY_H__
#define __NODE_FACTORY_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Definitions.h"

class NodeFactory : public Ref
{
public:
    virtual bool init(Tmx* tmx);    
    static NodeFactory* create(Tmx* tmx) {
        auto p = new NodeFactory();
        if (p->init(tmx)) {
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return nullptr;
    }
    
    Building* createBuilding(BuildingType type, Vec2 coord);
    Unit* createUnit(UnitType type, Vec2 tileCoord);
    void updateAttackRangeGrid();
    
protected:
    Tmx* tmx;
};

#endif // __NODE_FACTORY_H__
