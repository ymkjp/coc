#include "BattleScene.h"
#include "Definitions.h"
#include "editor-support/cocostudio/CocoStudio.h"

USING_NS_CC;

Scene* BattleScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = BattleScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool BattleScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    scrollView = ScrollView::create(visibleSize);
    backgroundLayer = Layer::create();
    
    CCLOG("DISPLAY INFO: %f, %f, %f, %f", visibleSize.width, origin.x, visibleSize.height, origin.y);
    
    auto lister = EventListenerTouchOneByOne::create();
    lister->setSwallowTouches(true);
    
    lister->onTouchBegan = CC_CALLBACK_2(BattleScene::onTouchBegan, this);
    lister->onTouchMoved = CC_CALLBACK_2(BattleScene::onTouchMoved, this);
    lister->onTouchEnded = CC_CALLBACK_2(BattleScene::onTouchEnded, this);
    lister->onTouchCancelled = CC_CALLBACK_2(BattleScene::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(lister, this);
    
    addBattleStage();
    return true;
}

void BattleScene::addBattleStage()
{
    scrollView->setBounceable(false);
    
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
    scrollView->setDelegate(this);
    
    addChild(scrollView);
}

bool BattleScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    CCLOG("1. touched");
    return true;
}

void BattleScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    CCLOG("2. moved");
}

void BattleScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    CCLOG("3. ended");
}

void BattleScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    CCLOG("4. cancelled");
}


void BattleScene::scrollViewDidScroll(ScrollView *view)
{
    CCLOG("スクロール！");
}

void BattleScene::scrollViewDidZoom(ScrollView *view)
{
    CCLOG("ズーム！");
}