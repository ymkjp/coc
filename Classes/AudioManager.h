#ifndef __AUDIO_MANAGER_H__
#define __AUDIO_MANAGER_H__

#include "cocos2d.h"
USING_NS_CC;

class AudioManager : public Ref
{
public:
    virtual bool init();
    CREATE_FUNC(AudioManager);
    void preloadSE(const std::string &id);
    unsigned int playSE(const std::string &id);
    void preloadBgm(const std::string &id);
    void playBgm(const std::string &id, bool bLoop);
    void stopBgm();
    
protected:
    const char* getFileName(const std::string &id);
};

#endif // __AUDIO_MANAGER_H__
