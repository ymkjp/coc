#include "StageScene.h"

USING_NS_CC;

Scene* StageScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = StageScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StageScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // TODO
    // Add menu
    // http://www.youtube.com/watch?v=fFMLWs2zYoU&index=4&list=PLRtjMdoYXLf7GSD9crXIjMQiRuIZ7mUVp
    
    return true;
}

