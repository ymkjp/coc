#include "UnitWallbreaker.h"

USING_NS_CC;


void UnitWallbreaker::startAttacking()
{
//    CCLOG("UnitWallbreaker::attack!");
    if (status == Died) {
        return;
    }
    if (!this->targetBuilding || this->targetBuilding->status == Building::Died) {
        //        action = Walking;
        //        //        this->updateMotionNode();
        
        this->scheduleOnce(schedule_selector(Unit::play), 0);
        this->unschedule(schedule_selector(Unit::attack));
        
        
    }
    if (status == Alive && this->targetBuilding->status == Building::Alive) {
        //        this->action = Attacking;
        //        this->updateMotionNode();
        
        this->unschedule(schedule_selector(Unit::attack));
        
        // 攻撃
        this->targetBuilding->attacked(damagePerAttack);
        
        // 攻撃したらすぐ死ぬ
        this->die();
    }
}

void UnitWallbreaker::playDeathVoice()
{
    // deployのときと同じ音
    tmx->playSE("wallbreaker_deploy");
};