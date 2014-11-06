#include "BattleStage.h"
#include "Definitions.h"
#include "editor-support/cocostudio/CocoStudio.h"

USING_NS_CC;

BattleStage::BattleStage()
{
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
}

void BattleStage::addBattleStage(cocos2d::Layer *layer)
{
//    Size visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();
//    CCLOG("xxx: %f, %f, %f, %f", visibleSize.width, origin.x, visibleSize.height, origin.y);
    
    auto scrollView = ScrollView::create(visibleSize);
    scrollView->setBounceable(false);
    
    auto backgroundLayer = Layer::create();
    
    auto spriteRight = Sprite::create("BackgroundStage1.png");
    auto spriteLeft = Sprite::create("BackgroundStage1.png");
    spriteLeft->setFlippedX(true);
    
    spriteRight->setScale(0.525);
    spriteLeft->setScale(0.525);
    
    spriteRight->setPosition(Point(origin.x + visibleSize.width * 0.75, origin.y + visibleSize.height / 2));
    spriteLeft->setPosition(Point(origin.x + visibleSize.width * 0.25, origin.y + visibleSize.height / 2));
    
    backgroundLayer->addChild(spriteRight);
    backgroundLayer->addChild(spriteLeft);
    
    scrollView->setContainer(backgroundLayer);
    scrollView->setContentSize(backgroundLayer->getContentSize());
    
    layer->addChild(scrollView);
}
