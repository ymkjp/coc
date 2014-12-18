#include "AudioManager.h"
#include "Definitions.h"

USING_NS_CC;

AudioManager* AudioManager::audioManagerInstance = NULL;

AudioManager::AudioManager(){}

AudioManager* AudioManager::getInstance()
{
    if (NULL == audioManagerInstance) {
        audioManagerInstance = new AudioManager();
        
        // 設定の初期化
        SimpleAudioEngine::getInstance()->setEffectsVolume(0.8);
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.8);
    }
    return audioManagerInstance;
}

void AudioManager::preloadSE(const std::string &id)
{
    SimpleAudioEngine::getInstance()->preloadEffect(getFileName(id));
}

unsigned int AudioManager::playSE(const std::string &id)
{
    clock_t currentClock = clock();
    auto key = playingSE.find(id);
    if (key != playingSE.end()
        && currentClock - CLOCKS_PER_SEC * UNIT_DEPLOY_INTERVAL * 0.5 <= playingSE.at(id)) {
        // 再生履歴が見つかって、それが100ミリセカンド以内だとそれ以上は再生しない
//        CCLOG("SE SKIPPED(%s,%li)",id.c_str(),currentClock);
        return 0;
    }
    
    playingSE[id] = currentClock;
    
    auto index = SimpleAudioEngine::getInstance()->playEffect(getFileName(id), false, 1.0f, 0.0f, 1.0f);
//    CCLOG("SE PLAYED:(%s,%li,%i)",id.c_str(),currentClock,index);

    return index;
}

void AudioManager::preloadBgm(const std::string &id)
{
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(getFileName(id));
}

void AudioManager::playBgm(const std::string &id, bool bLoop)
{
    SimpleAudioEngine::getInstance()->playBackgroundMusic(getFileName(id), bLoop);
}

void AudioManager::stopBgm()
{
    SimpleAudioEngine::getInstance()->stopBackgroundMusic(false);
}

const char* AudioManager::getFileName(const std::string &id)
{
    std::string fileName;
    fileName += id;
    fileName += ".mp3";
    return fileName.data();
//    std::string filePath = FileUtils::getInstance()->fullPathForFilename(fileName);
//    CCLOG("[filePath]%s",filePath.data());
//    return filePath.data();
}

