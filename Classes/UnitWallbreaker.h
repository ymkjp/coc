#ifndef __UNIT_WALLBREAKER_H__
#define __UNIT_WALLBREAKER_H__

#include "Unit.h"

class UnitWallbreaker : public Unit
{
public:
    static UnitWallbreaker* create(Tmx* tmx, Vec2 coord) {
        auto p = new UnitWallbreaker();
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
        return Wallbreaker;
    }
    
    // Unit クラスを上書き
    Vec2 findPointToGo() {
        return this->findNearestWallGoalPoint();
    }
    
    void playDeathVoice()
    {
        // deployのときと同じ音
        audioManager->playSE("unit_wallbreaker_deploy");
    };
    
    void startAttacking();
    
    

};

#endif // __UNIT_WALLBREAKER_H__
