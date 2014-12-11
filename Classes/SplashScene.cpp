#include <fstream>

#include "SplashScene.h"
#include "StageSelectorScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

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
    
    // スプラッシュスクリーンの表示
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto ui = CSLoader::getInstance()->createNodeFromXML("LoadingLayer.csd");
    ui->setScale(visibleSize.width / ui->getContentSize().width);
    this->addChild(ui,UIOrder,UITag);
    
    
    // オーディオマネジャの初期化とジングル
    audioManager = AudioManager::create();
    audioManager->playLoadingSE("loading_screen_jingle");
    
    // アセットマネジャの初期化
    initDownloadDir();
    _showDownloadInfo = Label::createWithSystemFont("", "Arial", 20);
    this->addChild(_showDownloadInfo);
    _showDownloadInfo->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 20));
    
    // アセットをダウンロード
    getAssetManager()->update();
    
    return true;
}

void SplashScene::updateProgressBar(int percent)
{
    // LoadingBar_1
    auto progressBarNode = getChildByTag(UITag)->getChildByName("LoadingBar_1");
    
    auto timer = dynamic_cast<LoadingBar*>(progressBarNode);
    timer->setPercent(percent);
}


void SplashScene::onError(AssetsManager::ErrorCode errorCode)
{
    if (errorCode == AssetsManager::ErrorCode::NO_NEW_VERSION)
    {
        this->scheduleOnce(schedule_selector(SplashScene::GoToStageSelectorScene), DISPLAY_TIME_SPLASH_SCENE);
    }
    else if (errorCode == AssetsManager::ErrorCode::NETWORK)
    {
        _showDownloadInfo->setString("network error");
    }
    else if (errorCode == AssetsManager::ErrorCode::CREATE_FILE)
    {
        _showDownloadInfo->setString("create file error");
    }
}

void SplashScene::onProgress(int percent)
{
    if (percent < 0) {
        return;
    }
    CCLOG("download::onProgress(%i)",percent);
    updateProgressBar(percent);
}

void SplashScene::onSuccess()
{
    CCLOG("download success");
    
    // ダウンロード成功後、ステージ選択画面に遷移
    this->scheduleOnce(schedule_selector(SplashScene::GoToStageSelectorScene), DISPLAY_TIME_SPLASH_SCENE);
}

AssetsManager* SplashScene::getAssetManager()
{
    static AssetsManager *assetManager = NULL;
    if (!assetManager)
    {
        assetManager = new AssetsManager("http://dev-kenta-ky-yamamoto2.dev.gree-dev.net/ky-tools/coc-my-assets/download_package.zip",
                                         "http://dev-kenta-ky-yamamoto2.dev.gree-dev.net/ky-tools/coc-my-assets/version.txt",
                                         _pathToSave.c_str());
        assetManager->setDelegate(this);
        assetManager->setConnectionTimeout(6);
    }
    return assetManager;
}

void SplashScene::initDownloadDir()
{
    CCLOG("initDownloadDir");
    _pathToSave = CCFileUtils::getInstance()->getWritablePath();
    
    CCLOG("[initDownloadDir]Path: %s", _pathToSave.c_str());
    
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
//    DIR *pDir = NULL;
//    pDir = opendir(_pathToSave.c_str());
//    if (!pDir)
//    {
//        mkdir(_pathToSave.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
//    }
//#else
//    if ((GetFileAttributesA(_pathToSave.c_str())) == INVALID_FILE_ATTRIBUTES)
//    {
//        CreateDirectoryA(_pathToSave.c_str(), 0);
//    }
//#endif
    
    CCLOG("initDownloadDir end");
}

void SplashScene::reset()
{
    // 動作確認用リセット
    _showDownloadInfo->setString("");
    // Remove downloaded files
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    std::string command = "rm -r ";
    // Path may include space.
    command += "\"" + _pathToSave + "\"";
    system(command.c_str());
#else
    std::string command = "rd /s /q ";
    // Path may include space.
    command += "\"" + _pathToSave + "\"";
    system(command.c_str());
#endif
    getAssetManager()->deleteVersion();
    initDownloadDir();
}


void SplashScene::GoToStageSelectorScene(float dt)
{
    // 画像関連のキャッシュ
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("assets.plist");
    
    preloadAllSoundEffects();
    
    auto scene = StageSelectorScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void SplashScene::preloadAllSoundEffects()
{
    for (auto file: audioList) {
        audioManager->preloadSE(file);
    }
}
