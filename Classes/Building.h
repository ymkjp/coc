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
    
};

#endif // __BUILDING_H__
