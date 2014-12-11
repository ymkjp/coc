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
    AssetsManager* getAssetManager();
    void initDownloadDir();
    std::string _pathToSave = "";
    Label *_showDownloadInfo = {};
    
    AudioManager* audioManager;
    void preloadAllSoundEffects();
    void GoToStageSelectorScene(float dt);
    
    void updateProgressBar(int percent);
    
    const std::vector<std::string> audioList = {
        "arrow_hit",
        "arrow_shoot",
        "arrow_shoot_by_archer_tower",
        "button_click",
        "cannon_attack",
        "coin_steal",
        "elixir_steal",
        "explosion",
        "mortar_hit",
        "mortar_shoot",
        "unit_archer_death",
        "unit_archer_deploy",
        "unit_barbarian_attack_hit",
        "unit_barbarian_death",
        "unit_barbarian_deploy",
        "unit_giant_attack_hit",
        "unit_giant_attack_start",
        "unit_giant_death",
        "unit_giant_deploy",
        "unit_goblin_attack_hit",
        "unit_goblin_death",
        "unit_goblin_deploy",
        "unit_wallbreaker_attack_hit",
        "unit_wallbreaker_deploy",
    };
};

#endif // __SPLASH_SCENE_H__
