#include "BuildingTownhall.h"

USING_NS_CC;


void BuildingTownhall::damagedEffect()
{
    auto node = this->getChildByTag(BuildingNodeTag);
    
    // res/ElixerBubble.csb
    auto elixerNode = tmx->csLoader->createNode("res/ElixerBubble.csb");
    auto elixerAction = tmx->actionTimelineCache->createAction("res/ElixerBubble.csb");
    elixerNode->setPositionY(82);
    elixerNode->setScale(1.4);
    elixerNode->runAction(elixerAction);
    elixerAction->gotoFrameAndPlay(0,false);
    if (node) {
        node->addChild(elixerNode);
    }
    
    // res/CoinBubble.csb
    auto coinNode = tmx->csLoader->createNode("res/CoinBubble.csb");
    auto coinAction = tmx->actionTimelineCache->createAction("res/CoinBubble.csb");
    coinNode->setPositionY(82);
    coinNode->runAction(coinAction);
    coinAction->gotoFrameAndPlay(0,false);
    if (node) {
        node->addChild(coinNode);
    }

}
