#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#include "cocostudio/CocoStudio.h"
using namespace cocostudio;
#include "cocosGUI.h"
using namespace cocos2d::ui;

#include "Definitions.h"
#include "AudioManager.h"

class SplashScene : public Layer, public AssetsManagerDelegateProtocol
{
public:
    enum LocalZOrder {
        UIOrder = 1,
    };
    enum NodeTag {
        UITag,
    };
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(SplashScene);
    
    // アセットマネジャ
    void reset();
    virtual void onError(AssetsManager::ErrorCode errorCode);
    virtual void onProgress(int percent);
    virtual void onSuccess();

    
private:
    void updateMessage(std::string message);
    
    AssetsManager* getAssetManager();
    void initDownloadDir();
    std::string _pathToSave = "";
    
    AudioManager* audioManager;
    void preloadAudioResources();
    void GoToStageSelectorScene(float dt);
    
    void updateProgressBar(int percent);
    
    const std::vector<std::string> sfxList = {
        "arrow_hit",
        "arrow_shoot",
        "archer_tower_shoot",
        "button_click",
        "cannon_attack",
        "coin_steal",
        "elixir_steal",
        "explosion",
        "mortar_hit",
        "mortar_shoot",
        "archer_death",
        "archer_deploy",
        "barbarian_attack_hit",
        "barbarian_death",
        "barbarian_deploy",
        "giant_attack_hit",
        "giant_attack_start",
        "giant_death",
        "giant_deploy",
        "goblin_attack_hit",
        "goblin_death",
        "goblin_deploy",
        "wallbreaker_attack_hit",
        "wallbreaker_deploy",
    };
    
    const std::vector<std::string> bgmList = {
        "battle_lost_02",
        "combat_music",
        "planning",
        "winwinwin",
    };

};

#endif // __SPLASH_SCENE_H__
