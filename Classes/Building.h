#ifndef __BUILDING_H__
#define __BUILDING_H__

#include <map>
#include <vector>

#include "cocos2d.h"
USING_NS_CC;

#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

#include "Definitions.h"
#include "Tmx.h"

class Building : public Node
{
public:
    enum __STATUS {
        Died = 0,
        Alive,
    };
    
    // Zオーダーが大きければ上に表示される
    enum LocalZOrder {
        DefaultOrder = 1,
        BuildingOrder,
        BuildingExplosionShardsOrder,
        CanonSmokeOrder,
        LuminousCirclebOrder, 
        ArcherOnTowerOrder,
    };
    
    enum Tag {
        BuildingNodeTag = 1,
        BuildingActionTag,
        ArcherOnTowerTag,
        SmokeNodeTag,
        SmokeActionTag,
        LuminousNodeTag,
        LuminousActionTag,
        BulletNodeTag,
        BulletShadowNodeTag,
        DamagedEffectNodeTag,
        DamagedEffectActionTag,
    };
    
    Vec2 coord;
    BuildingType type;
    __STATUS status;
    
    virtual bool init(Tmx* tmx, Vec2 coord);

    virtual BuildingSpace getSpace();
    
    const static BuildingTypesByCategory typesByCategory;
    static std::vector<BuildingType> getTypesByCategory(BuildingCategory category)
    {
        return Building::typesByCategory.at(category);
    };

    const static std::map<BuildingSpace, std::vector<Vec2>> coordsSurround;
    const static std::map<BuildingSpace, std::vector<Vec2>> coordsBuildingSpace;

    bool isBuildingRange(Vec2 coord);
    bool isTargetLayer(std::string name, Vec2 coord);
    void attacked(float damage);
    void broken();
    void addWrack();
    void putTargetMark();
    void brokenEffect();
    
protected:
    float hitpoints;
    float hitpointPercentage = 100;
    
    Tmx* tmx;
    
    // 親ゲージのタグの管理が難しいためポインタでハンドルする
    Node* lifeGageNode;
    timeline::ActionTimeline* lifeGageAction;
    
    Node* targetMarkNode;
    timeline::ActionTimeline* targetMarkAction;
    
    const BuildingSpaceByType typeSpace = {
        {TownHall,     Large},
        {ElixerTank,   Regular},
        {GoldBank,     Regular},
        {Canon,        Regular},
        {TrenchMortar, Regular},
        {ArcherTower,  Regular},
        {Wall,         Small},
    };
    
    void initNode();
    
    // 子クラスで定義されるべきメソッド
    virtual BuildingType getType() = 0;

    // 子クラスで再定義されてもいいメソッド
    virtual bool startScan() {return true;};
    virtual void updateLifeGage();
    virtual void damagedEffect() {};
    virtual void initArchersOnTower() {};
    virtual void increaseResourceScore(float damage) {};
    virtual void initOwn() {};
    
    const std::map<BuildingType, float> hitpointsByType =
    {
        {TownHall, 2400},
        {ElixerTank, 1200},
        {GoldBank, 1200},
        {Canon, 400},
        {TrenchMortar, 500},
        {ArcherTower, 500},
        {Wall,700},
    };
    
    float getFullHitPoints()
    {
        return hitpointsByType.at(type);
    }
    
    void hideLifeGage(float frame)
    {
        if (lifeGageNode) {
            lifeGageNode->setVisible(false);
            return;
        }
    }
    
};

#endif // __BUILDING_H__
