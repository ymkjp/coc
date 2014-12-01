#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include <vector>
#include <array>
#include <map>


#include "cocos2d.h"
USING_NS_CC;

#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;

#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

#include "Definitions.h"
#include "Tmx.h"
#include "Building.h"
#include "Unit.h"


class BattleScene : public cocos2d::Layer, public ScrollViewDelegate
{
public:
    struct ScrollStatus {
        int scrollingDelay;
        int zoomingDelay;
    } scrollStatus;
 
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
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
    virtual void addEventDispacher();
    virtual void addUILayer();
    virtual void initBuildings();

    Vector<Unit*> units;
    Vector<Building*> buildings;
    void addBuilding(BuildingType type, Vec2 coord, Vec2 Pos);

private:
    Size visibleSize;
    Vec2 origin;
    ScrollView *scrollView;
    Layer *backgroundLayer;
    Layer *tiledMapLayer;

    Tmx* tmx;
    
    SpriteBatchNode* spriteBatch;
    bool isTargetLayer(std::string name, Vec2 coord);
};

#endif // __BATTLE_SCENE_H__
