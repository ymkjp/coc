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
    
    // オーディオマネジャの初期化とジングルプリロード、再生はダウンロード開始時
    audioManager = AudioManager::getInstance();
    audioManager->preloadSE("loading/jingle");
    audioManager->playSE("loading/jingle");
    
    if (QUICK_DEBUG_MODE) {
        // デバッグ用に遷移する (Resource 以下にファイルがあることが前提)
        this->scheduleOnce(schedule_selector(SplashScene::GoToStageSelectorScene), DISPLAY_TIME_SPLASH_SCENE);
    } else {
        // アセットマネジャの初期化
        initDownloadDir();
        
        // 前回ダウンロードしたものを削除
        reset();
    
        // アセットをダウンロード
        updateMessage("Now Downloading ...");
        getAssetManager()->update();
    }
    
    return true;
}

void SplashScene::updateMessage(std::string message)
{
    auto ui = getChildByTag(UITag);
    if (!ui) {
        return;
    }

    // 「ダウンロード中」などのメッセージ表示用ラベル
    auto label = ui->getChildByName<cocos2d::ui::Text*>("Text_Message");
    auto labelLeft = ui->getChildByName<cocos2d::ui::Text*>("Text_Message_Shadow_Left");
    auto labelRight = ui->getChildByName<cocos2d::ui::Text*>("Text_Message_Shadow_Right");
    
    if (label && labelLeft && labelRight) {
        label->setString(message.c_str());
        labelLeft->setString(message.c_str());
        labelRight->setString(message.c_str());
    }
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
        updateMessage("No new version of files");
        this->scheduleOnce(schedule_selector(SplashScene::GoToStageSelectorScene), DISPLAY_TIME_SPLASH_SCENE);
    }
    else if (errorCode == AssetsManager::ErrorCode::NETWORK)
    {
        updateMessage("Network error, retrying...");
        getAssetManager()->update();
    }
    else if (errorCode == AssetsManager::ErrorCode::CREATE_FILE)
    {
        updateMessage("Creating file error...");
    }
}

void SplashScene::onProgress(int percent)
{
    if (percent < 0) {
        return;
    }
    // プログレスバーはダウンロード完了で 90%, uncompress 完了で 100% で表示する
    updateProgressBar(percent * 0.9);
    
    if (percent == 100) {
        updateMessage("Loading files ...");
    }
}

void SplashScene::onSuccess()
{
    // ダウンロード成功後、ステージ選択画面に遷移
    updateMessage("Download succeessfully completed");
    updateProgressBar(100);
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
        assetManager->setConnectionTimeout(120);
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
    
    // 音楽ファイルのプリロード
    preloadAudioResources();
    
    auto scene = StageSelectorScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void SplashScene::preloadAudioResources()
{
    for (auto file: sfxList) {
        audioManager->preloadSE(file);
    }
    for (auto file: bgmList) {
        audioManager->preloadBgm(file);
    }
}
