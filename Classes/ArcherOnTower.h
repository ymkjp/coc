#ifndef __ARCHER_ON_TOWER_H__
#define __ARCHER_ON_TOWER_H__

#include "Definitions.h"
#include "AudioManager.h"
#include "Tmx.h"

class ArcherOnTower : public Node
{
public:
    enum __STATUS {
        Idle = 1,
        Attacking,
    } action;
    
    Unit* targetUnit = {};
    float damagePerShot;
    
    static ArcherOnTower* create(Tmx* tmx, Vec2 coord, float damagePerShot) {
        auto p = new ArcherOnTower();
        if (p->init(tmx, coord, damagePerShot)) {
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return nullptr;
    }
    
    bool init(Tmx* tmx, Vec2 coord, float damagePerShot);
    void setDirection(Compass direction);
    void updateMotionNode();
    void shoot();
    
protected:
    Tmx* tmx;
    Vec2 coord;
    Compass direction;
    timeline::ActionTimeline* motionAction;
    
    enum NodeTag {
        MotionTag,
        ShadowTag
    };
    
    enum LocalZOrder {
        ShadowOrder = 1,
        MotionOrder,
    };
    
    Node* arrow;
};

#endif // __ARCHER_ON_TOWER_H__
