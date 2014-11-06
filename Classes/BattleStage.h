#ifndef __BATTLE_STAGE_H__
#define __BATTLE_STAGE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;

class BattleStage : public cocos2d::Layer
{
public:
    BattleStage();
    void addBattleStage(cocos2d::Layer *layer);
    
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
};

#endif // __BATTLE_STAGE_H__
