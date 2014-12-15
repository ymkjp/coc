#include "BuildingResource.h"

USING_NS_CC;

void BuildingResource::initOwn()
{
    // ダメージエフェクトの初期化
    auto buildingNode = this->getChildByTag(BuildingNodeTag);
    auto node = CSLoader::createNode(motionNameByType.at(type));
    auto action = actionTimelineCache->createAction(motionNameByType.at(type));
    action->setTag(DamagedEffectActionTag);
    node->setPositionY(5);
    node->runAction(action);
    action->gotoFrameAndPause(60);
    if (type == ElixerTank) {
        node->setScale(1.6);
    }
    if (buildingNode) {
        buildingNode->addChild(node,1,DamagedEffectNodeTag);
    }
    
    // remainingStorageAmmount の初期化
    remainingStorageAmmount = storageCapacityByType.at(type);
    
    // 総アセットスコアに加算
    tmx->countUpRemainingScore(scoreTypeByBuildingType.at(type),storageCapacityByType.at(type));
}

float BuildingResource::getStorageCapacity()
{
    return storageCapacityByType.at(type);
}

void BuildingResource::damagedEffect()
{
    auto node = this->getChildByTag(BuildingNodeTag);
    if (!node) {
        return;
    }
    // コイン・エリクサーが減っていく
    int percentageFrame = hitpoints / getFullHitPoints() * 5;
    auto action = dynamic_cast<timeline::ActionTimeline*>(node->getActionByTag(BuildingNodeTag));
    if (action && 0 <= percentageFrame && percentageFrame <= 5) {
        action->gotoFrameAndPause(percentageFrame);
    }
    
    tmx->playSE(soundNameByType.at(type));
    
    // コイン・エリクサーが飛び散る
    auto damagedNode = this->getChildByTag(DamagedEffectNodeTag);
    if (!damagedNode) {
        return;
    }
    auto damagedEffectAction = dynamic_cast<timeline::ActionTimeline*>(damagedNode->getActionByTag(DamagedEffectActionTag));
    if (damagedEffectAction) {
        damagedEffectAction->gotoFrameAndPlay(0,false);
    }
}

void BuildingResource::increaseResourceScore(float damage)
{
//    CCLOG("damage(%f),remainingStorageAmmount(%f)",damage,remainingStorageAmmount);
    
    if (0 <= remainingStorageAmmount) {
        float earningScore = damage * storageCapacityByType.at(type) / getFullHitPoints();
        remainingStorageAmmount -= earningScore;
//        CCLOG("[BuildingGoldbank::increaseScore]earningScoreRatio(%f),earningScore(%f),remainingStorageAmmount(%f)",
//              earningScoreRatio,
//              earningScore,
//              remainingStorageAmmount
//              );
        tmx->increaseBattleScore(scoreTypeByBuildingType.at(type), earningScore);
    }
}
