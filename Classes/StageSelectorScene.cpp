#include "StageSelectorScene.h"
#include "BattleScene.h"
#include "Definitions.h"

USING_NS_CC;

Scene* StageSelectorScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = StageSelectorScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StageSelectorScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // バイナリ版 (.csb) だとフォントが反映されないため、XML版 (.csd) を使って描画している
    Node* ui = CSLoader::getInstance()->createNodeFromXML("cocosstudio/StageSelectorLayer.csd");
    ui->setScale(visibleSize.width / ui->getContentSize().width);
    this->addChild(ui);
//    
//    auto backgroundSprite = Sprite::create("SplashScreen.png");
//    backgroundSprite->setContentSize(visibleSize);
//    backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
//    
//    this->addChild(backgroundSprite);
//    this->scheduleOnce(schedule_selector(StageSelectorScene::GoToBattleScene), DISPLAY_TIME_SPLASH_SCENE);
//    
    return true;
}

void StageSelectorScene::GoToBattleScene(float dt)
{
    auto scene = BattleScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}