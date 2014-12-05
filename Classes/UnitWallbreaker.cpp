#include "UnitWallbreaker.h"

USING_NS_CC;

void UnitWallbreaker::startAttacking()
{
    this->action = Attacking;
    this->updateNode();
    
    // 攻撃
    this->targetBuilding->attacked(damagePerAttack);
    
    // 攻撃したらすぐ死ぬ
    status = Died;
    this->die();
}
