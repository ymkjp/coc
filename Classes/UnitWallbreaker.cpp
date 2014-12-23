#include "UnitWallbreaker.h"

USING_NS_CC;


void UnitWallbreaker::startAttacking()
{
//    CCLOG("UnitWallbreaker::attack!");
    if (status == Died) {
        return;
    }
    if (!this->targetBuilding || this->targetBuilding->status == Building::Died) {
        this->scheduleOnce(schedule_selector(Unit::play), SOON);
    }
    
    if (status == Alive && this->targetBuilding->status == Building::Alive) {
        // 攻撃
        this->targetBuilding->attacked(damagePerAttack);
        
        // 煙
        auto explosion = tmx->csLoader->createNode("res/Explosion.csb");
        auto explosionAction = tmx->actionTimelineCache->createAction("res/Explosion.csb");
        targetBuilding->addChild(explosion);
        explosion->runAction(explosionAction);
        explosionAction->gotoFrameAndPlay(0,false);
        
        // 攻撃したらすぐ死ぬ
        this->die();
    }
}

void UnitWallbreaker::playDeathVoice()
{
    // deployのときと同じ音
    tmx->playSE("wallbreaker_deploy");
};