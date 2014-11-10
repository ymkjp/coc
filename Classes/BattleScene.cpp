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
    
    addBattleStage();
    return true;
}

void BattleScene::addBattleStage()
{
    scrollView->setBounceable(false);
    auto spriteRight = Sprite::create("BackgroundStage1.png");
    auto spriteLeft = Sprite::create("BackgroundStage1.png");
    spriteLeft->setFlippedX(true);
    CCLOG("[CONTENT SIZE] %f, %f",spriteRight->getContentSize().width, spriteRight->getContentSize().height);
    
    spriteRight->setScale(0.525);
    spriteLeft->setScale(0.525);
    
    spriteRight->setPosition(Vec2(origin.x + visibleSize.width * 0.75, origin.y + visibleSize.height / 2));
    spriteLeft->setPosition(Vec2(origin.x + visibleSize.width * 0.25, origin.y + visibleSize.height / 2));
    
    backgroundLayer->addChild(spriteRight);
    backgroundLayer->addChild(spriteLeft);
    
    backgroundLayer->setScale(DEFAULT_ZOOM_RATE);
    
    scrollView->setContainer(backgroundLayer);
    scrollView->setDelegate(this);
    scrollView->setMaxScale(MAX_ZOOM_RATE);
    scrollView->setMinScale(MIN_ZOOM_RATE);
    
    addChild(scrollView);
    
    auto lister = EventListenerTouchOneByOne::create();
    lister->setSwallowTouches(true);
    lister->onTouchBegan = CC_CALLBACK_2(BattleScene::onTouchBegan, this);
    lister->onTouchMoved = CC_CALLBACK_2(BattleScene::onTouchMoved, this);
    lister->onTouchEnded = CC_CALLBACK_2(BattleScene::onTouchEnded, this);
    lister->onTouchCancelled = CC_CALLBACK_2(BattleScene::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(lister, this);
    
    auto multi_listener = EventListenerTouchAllAtOnce::create();
    multi_listener->onTouchesBegan = CC_CALLBACK_2(BattleScene::onTouchesBegan, this);
    multi_listener->onTouchesMoved = CC_CALLBACK_2(BattleScene::onTouchesMoved, this);
    multi_listener->onTouchesCancelled = CC_CALLBACK_2(BattleScene::onTouchesCancelled, this);
    multi_listener->onTouchesEnded = CC_CALLBACK_2(BattleScene::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(multi_listener, this);
}

bool BattleScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    CCLOG("1. touched");
    return true;
}

void BattleScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    CCLOG("2. moved");
    backgroundLayer->setPosition(backgroundLayer->getPosition() + touch->getDelta());
}

void BattleScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    CCLOG("DISPLAY INFO: %f, %f, %f, %f", visibleSize.width, origin.x, visibleSize.height, origin.y);
    CCLOG("[noConvert]    location.x: %f, location.y: %f", touch->getLocation().x, touch->getLocation().y);
    CCLOG("[NodeSpace]    location.x: %f, location.y: %f", this->convertToNodeSpace(touch->getLocation()).x, this->convertToNodeSpace(touch->getLocation()).y);
    CCLOG("[WorldSpace]   location.x: %f, location.y: %f", this->convertToWorldSpace(touch->getLocation()).x, this->convertToWorldSpace(touch->getLocation()).y);
    CCLOG("[NodeSpaceAR]  location.x: %f, location.y: %f", this->convertToNodeSpaceAR(touch->getLocation()).x, this->convertToNodeSpaceAR(touch->getLocation()).y);
    CCLOG("[WorldSpaceAR] location.x: %f, location.y: %f", this->convertToWorldSpaceAR(touch->getLocation()).x, this->convertToWorldSpaceAR(touch->getLocation()).y);
    
    CCLOG("3. ended");
}

void BattleScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    CCLOG("4. cancelled");
}

void BattleScene::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent)
{
CCLOG("11. Touches Began");
}

void BattleScene::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches,cocos2d::Event *pEvent)
{
    CCLOG("11. Touches Moved");
}

void BattleScene::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent)
{
    CCLOG("11. Touches Ended");
}

void BattleScene::onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent)
{
    CCLOG("11. Touches Cancelled");
}


void BattleScene::scrollViewDidScroll(ScrollView *view)
{
    CCLOG("スクロール！");
}

void BattleScene::scrollViewDidZoom(ScrollView *view)
{
    CCLOG("ズーム！");
}