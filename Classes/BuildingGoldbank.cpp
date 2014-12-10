#include "BuildingGoldbank.h"

USING_NS_CC;


// res/CoinBubble.csb
void BuildingGoldbank::damagedEffect()
{
    int percentageFrame = hitpoints / getFullHitPoints() * 5;
    if (0 <= percentageFrame && percentageFrame <= 5) {
        motionAction->gotoFrameAndPause(percentageFrame);
    }
    
    audioManager->playSE("coin_steal");
    
    auto node = CSLoader::createNode("res/CoinBubble.csb");
    auto action = actionTimelineCache->createAction("res/CoinBubble.csb");
    node->setPositionY(5);
    node->runAction(action);
    action->gotoFrameAndPlay(0,false);
    buildingNode->addChild(node);
}