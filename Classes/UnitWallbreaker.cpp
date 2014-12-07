#include "UnitWallbreaker.h"

USING_NS_CC;

void UnitWallbreaker::startAttacking()
{
    if (status == Alive && this->targetBuilding->status == Building::Alive) {
        this->action = Attacking;
        this->updateMotionNode();
        
        // 攻撃
        this->targetBuilding->attacked(damagePerAttack);
        
        
        // 攻撃したらすぐ死ぬ
        status = Died;
        this->die();
    }
}
