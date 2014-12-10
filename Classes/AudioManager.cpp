#include "AudioManager.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

USING_NS_CC;

bool AudioManager::init()
{
    SimpleAudioEngine::getInstance()->setEffectsVolume(1);
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);
    return true;
}

void AudioManager::preloadSE(const std::string &id)
{
    SimpleAudioEngine::getInstance()->preloadEffect(getFileName(id));
}

unsigned int AudioManager::playSE(const std::string &id)
{
    return SimpleAudioEngine::getInstance()->playEffect(getFileName(id), false, 1.0f, 0.0f, 1.0f);
}

const char* AudioManager::getFileName(const std::string &id)
{
    std::string tmp = "sfx/";
    tmp += id;
    tmp += ".mp3";
    return tmp.data();
}
