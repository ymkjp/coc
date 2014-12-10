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
    
    // 親クラスで virtual pure 規定された定義しなければならないメソッド
    UnitType getUnitType()
    {
        return Archer;
    }
    
    // 親クラスで virtual 規定された任意で上書きできるメソッド
    std::vector<Vec2> getSurroundedCoords(Vec2 targetCoord);
    
    // Unit を上書き
    void shoot();
    virtual void playStartAttackingVoice()
    {
        audioManager->playSE("arrow_shoot");
    };
    
    Node* arrow;
};

#endif // __UNIT_ARCHER_H__
