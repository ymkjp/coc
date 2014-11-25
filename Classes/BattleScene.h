#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include <array>
#include <vector>

#include "cocos2d.h"
USING_NS_CC;

#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;

#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

#include "Definitions.h"
#include "Building.h"

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
    
    std::stack<Vec2>* worldPathCache = {};
    
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
    virtual Vec2 convertToTile(Vec2 pos);
    virtual bool isTargetLayer(std::string name, Vec2 coord);
    
    virtual Vec2 findGoalCoord(Vec2 pos, Building::__CATEGORY targetType);

    std::map<Building::__TYPE, std::vector<Vec2>> buildingCoords;

    std::array<std::array<Building*, WORLD_MAP_WIDTH>, WORLD_MAP_HEIGHT> buildingGrid = {};
    
    virtual void addToBuildingCache(Building::__TYPE type, Vec2 coord);
};

#endif // __BATTLE_SCENE_H__
