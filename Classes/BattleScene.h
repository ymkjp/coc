#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;

#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

class BattleScene : public cocos2d::Layer, public ScrollViewDelegate
{
public:
    struct ScrollStatus {
        int scrollingDelay;
        int zoomingDelay;
    } scrollStatus;
 
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(BattleScene);
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent);
    virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches,cocos2d::Event *pEvent);
    virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent);
    virtual void onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent);
    
    virtual void scrollViewDidScroll(ScrollView *view);
    virtual void scrollViewDidZoom(ScrollView *view);

    virtual void addBattleStage();
    virtual void addBuildings();
    virtual void addEventDispacher();
    virtual void addUILayer();
    
private:
    Size visibleSize;
    Vec2 origin;
    ScrollView *scrollView;
    Layer *backgroundLayer;
    Layer *tiledMapLayer;
    
    TMXTiledMap *tiledMap;
    TMXLayer *domainTMXLayer;
    TMXLayer *wallTMXLayer;
    
    SpriteBatchNode* spriteBatch;
    
    virtual Vec2 convertToCoord(Vec2 pos);
};

#endif // __BATTLE_SCENE_H__
