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
    Vec2 coord;
    Node* buildingNode;
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

protected:
    float hitpoints = 400;
    Tmx* tmx;
    timeline::ActionTimelineCache* actionTimelineCache;
    
    Node* lifeGageNode;
    timeline::ActionTimeline* lifeGageAction;
    
    Node* motionNode;
    timeline::ActionTimeline* motionAction;
    
    enum NodeTag {
        MotionTag,
        LifeGageTag,
    };
    
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
    virtual bool virtualInit() {return true;};
    virtual void updateLifeGage();

    
    const std::map<BuildingType, float> hitpointsByType =
    {
        {TownHall, 2100},
        {ElixerTank, 800},
        {GoldBank, 800},
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
        this->lifeGageNode->setVisible(false);
    }
    
};

#endif // __BUILDING_H__
