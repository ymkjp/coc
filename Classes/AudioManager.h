#ifndef __AUDIO_MANAGER_H__
#define __AUDIO_MANAGER_H__

#include <time.h>

#include "cocos2d.h"
USING_NS_CC;

#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

class AudioManager
{
public:
    static AudioManager* getInstance();
    
    void preloadSE(const std::string &id);
    unsigned int playSE(const std::string &id);
    void preloadBgm(const std::string &id);
    void playBgm(const std::string &id, bool bLoop);
    void stopBgm();
                                   
protected:
    const char* getFileName(const std::string &id);
    std::map<std::string, time_t> playingSE;
    
private:
    AudioManager();
    static AudioManager* audioManagerInstance;
};

#endif // __AUDIO_MANAGER_H__
