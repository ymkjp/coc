#include "BuildingResource.h"

USING_NS_CC;

void BuildingResource::initDamagedEffectNode()
{
    damagedEffectNode = CSLoader::createNode(motionNameByType.at(type));
    damagedEffectAction = actionTimelineCache->createAction(motionNameByType.at(type));
    damagedEffectNode->setPositionY(5);
    adjustScale();
    damagedEffectNode->runAction(damagedEffectAction);
    buildingNode->addChild(damagedEffectNode);
    
    // remainingStorageAmmount も初期化
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
    int percentageFrame = hitpoints / getFullHitPoints() * 5;
    if (motionAction && 0 <= percentageFrame && percentageFrame <= 5) {
        motionAction->gotoFrameAndPause(percentageFrame);
    }
    
    audioManager->playSE(soundNameByType.at(type));
    
    // コイン・エリクサーが飛び散る
    damagedEffectAction->gotoFrameAndPlay(0,false);
}

void BuildingResource::increaseResourceScore(float damage)
{
//    CCLOG("damage(%f),remainingStorageAmmount(%f)",damage,remainingStorageAmmount);
    
    if (0 <= remainingStorageAmmount) {
        float earningScoreRatio = damage / getFullHitPoints();
        float earningScore = earningScoreRatio * storageCapacityByType.at(type);
        remainingStorageAmmount -= earningScore;
//        CCLOG("[BuildingGoldbank::increaseScore]earningScoreRatio(%f),earningScore(%f),remainingStorageAmmount(%f)",
//              earningScoreRatio,
//              earningScore,
//              remainingStorageAmmount
//              );
        tmx->increaseBattleScore(scoreTypeByBuildingType.at(type), earningScore);
    }
}
