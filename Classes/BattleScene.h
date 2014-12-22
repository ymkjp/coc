#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include <vector>
#include <array>
#include <map>
#include <time.h>

#include "cocos2d.h"
USING_NS_CC;

#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;

#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

#include "Definitions.h"
#include "Tmx.h"
#include "Unit.h" // @todo setPosition あたり直したので読み込み不要
#include "NodeFactory.h"

class BattleScene : public Layer, public ScrollViewDelegate
{
public:
    struct ScrollStatus {
        int scrollingDelay;
        int zoomingDelay;
    } scrollStatus;
 
    static cocos2d::Scene* createScene(Stages stage);
    
    virtual bool init(Stages stage);
    static BattleScene* create(Stages stage) {
        auto p = new BattleScene();
        if (p->init(stage)) {
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return nullptr;
    }
    
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
    virtual void initBuildings();

    void addBuilding(BuildingType type, Vec2 coord, Vec2 Pos);
    void addBuildingShadow(BuildingType type, Vec2 coord);
    void addGrass(Vec2 coord);

private:
    // Z値が大きければ上に表示される
    enum localZOrder {
        TmxOrder = 1000, // ゲーム結果画面の表示のときにUIを表示する
    };
    
    Size visibleSize;
    Vec2 origin;
    ScrollView *scrollView;
    Layer *backgroundLayer;
    Layer *tiledMapLayer;
    Layer *deployAreaLayer;

    clock_t touchStartingTime;
    
    Tmx* tmx;
    NodeFactory* nodeFactory;
    
    SpriteBatchNode* spriteBatch;
    bool isTargetLayer(std::string name, Vec2 coord);
    
    void showCloud();
    
    // 長押し Touch 判定
    cocos2d::Touch* targetTouch;
    void deployUnitIfKeptTouching(float frame);
    void deployUnit();
};

#endif // __BATTLE_SCENE_H__
