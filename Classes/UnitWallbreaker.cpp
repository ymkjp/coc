#include "UnitWallbreaker.h"

USING_NS_CC;

void UnitWallbreaker::startAttacking()
{
    if (status == Alive && this->targetBuilding && this->targetBuilding->status == Building::Alive) {
        this->action = Attacking;
        this->updateMotionNode();
        
        // 攻撃
        this->targetBuilding->attacked(damagePerAttack);
        
        // 攻撃したらすぐ死ぬ
        this->die();
    }
    
    if (this->targetBuilding->status == Building::Died) {
        action = Walking;
        this->updateMotionNode();
        this->scheduleOnce(schedule_selector(Unit::play), 0);
        this->unschedule(schedule_selector(Unit::attack));
    }
}

void UnitWallbreaker::playDeathVoice()
{
    // deployのときと同じ音
    tmx->playSE("wallbreaker_deploy");
};