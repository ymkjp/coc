#include "BuildingTownhall.h"

USING_NS_CC;


void BuildingTownhall::damagedEffect()
{
    auto node = this->getChildByTag(BuildingNodeTag);
    
    // res/ElixerBubble.csb
    auto elixerNode = CSLoader::createNode("res/ElixerBubble.csb");
    auto elixerAction = tmx->actionTimelineCache->createAction("res/ElixerBubble.csb");
    elixerNode->setPositionY(10);
    elixerNode->setScale(2);
    elixerNode->runAction(elixerAction);
    elixerAction->gotoFrameAndPlay(0,false);
    if (node) {
        node->addChild(elixerNode);
    }
    
    // res/CoinBubble.csb
    auto coinNode = CSLoader::createNode("res/CoinBubble.csb");
    auto coinAction = tmx->actionTimelineCache->createAction("res/CoinBubble.csb");
    coinNode->setPositionY(10);
    coinNode->runAction(coinAction);
    coinAction->gotoFrameAndPlay(0,false);
    if (node) {
        node->addChild(coinNode);
    }

}
