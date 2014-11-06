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

    battleStage.addBattleStage(this);
    return true;
}
