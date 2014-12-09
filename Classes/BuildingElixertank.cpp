#include "BuildingElixertank.h"

USING_NS_CC;

// res/ElixerBubble.csb
void BuildingElixertank::damagedEffect()
{
    int percentageFrame = hitpoints / getFullHitPoints() * 5;
    if (0 <= percentageFrame && percentageFrame <= 5) {
        motionAction->gotoFrameAndPause(percentageFrame);
    }
    
    auto node = CSLoader::createNode("res/ElixerBubble.csb");
    auto action = actionTimelineCache->createAction("res/ElixerBubble.csb");
    node->setPositionY(5);
    node->setScale(1.6);
    node->runAction(action);
    action->gotoFrameAndPlay(0,false);
    buildingNode->addChild(node);
}
