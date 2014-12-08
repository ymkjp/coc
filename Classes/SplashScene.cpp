#include "SplashScene.h"
#include "StageSelectorScene.h"
#include "Definitions.h"

USING_NS_CC;

Scene* SplashScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SplashScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto backgroundSprite = Sprite::create("SplashScreen.png");    backgroundSprite->setScale(visibleSize.width / backgroundSprite->getContentSize().width);
    backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    
    this->addChild(backgroundSprite);
    this->scheduleOnce(schedule_selector(SplashScene::GoToStageSelectorScene), DISPLAY_TIME_SPLASH_SCENE);
    
    return true;
}

void SplashScene::GoToStageSelectorScene(float dt)
{
    auto scene = StageSelectorScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}