#include "AppDelegate.h"
#include "SplashScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {
    
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        //        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }
    
    // turn on display FPS
    director->setDisplayStats(true);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    // create a scene. it's an autorelease object
    auto scene = SplashScene::createScene();
    
    // run
    director->runWithScene(scene);
    
    // Add file load pathes
    auto fileUtils = FileUtils::getInstance();
    
    // add res fold resource
    std::vector<std::string> resDirOrders;
    
    if (QUICK_DEBUG_MODE) {
        // デバッグ用に "Resources/*" 以下を読み込む
        resDirOrders.push_back("");
        resDirOrders.push_back("loading");
        resDirOrders.push_back("cocosstudio");
        resDirOrders.push_back("res");
        resDirOrders.push_back("bgm");
        resDirOrders.push_back("sfx");
    } else {
        resDirOrders.push_back(fileUtils->getWritablePath());
        resDirOrders.push_back(fileUtils->getWritablePath() + "loading");
        resDirOrders.push_back(fileUtils->getWritablePath() + "cocosstudio");
        resDirOrders.push_back(fileUtils->getWritablePath() + "res");
        resDirOrders.push_back(fileUtils->getWritablePath() + "bgm");
        resDirOrders.push_back(fileUtils->getWritablePath() + "sfx");
    }
    
    fileUtils->setSearchPaths(resDirOrders);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
