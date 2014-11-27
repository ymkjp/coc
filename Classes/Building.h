#ifndef __BUILDING_H__
#define __BUILDING_H__

#include <map>
#include <vector>

#include "cocos2d.h"
USING_NS_CC;

#include "Definitions.h"
#include "Tmx.h"

class Tmx;

class Building : public Ref
{
public:
    enum __CATEGORY {
        NoCategory = 0,
        AnyBuildings,
        TownHalls,
        Resources,
        Defenses,
        Walls,
    };
    enum __STATUS {
        Died = 0,
        Alive,
    };
    enum __SPACE {
        Small = 1,
        Regular = 6,
        Large = 9,
    };
    Vec2 coord;
    Node* buildingNode;
    BuildingType type;
    __STATUS status;
    
    virtual bool init(Tmx* tmx, BuildingType type, Vec2 coord);
    static Building* create(Tmx* tmx, BuildingType type, Vec2 coord) {
        auto p = new Building();
        if (p->init(tmx, type, coord)) {
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return nullptr;
    }

    virtual __SPACE getSpace();
    
    const static std::map<__CATEGORY, std::vector<BuildingType>> typesByCategory;
    static std::vector<BuildingType> getTypesByCategory(__CATEGORY category)
    {
        return Building::typesByCategory.at(category);
    };
    
    const static std::map<__SPACE, std::vector<Vec2>> coordsSurround;
    
protected:
    Tmx* tmx;
    const std::map<BuildingType, __SPACE> typeSpace = {
        {TownHall,     Large},
        {ElixerTank,   Regular},
        {GoldBank,     Regular},
        {Canon,        Regular},
        {TrenchMortar, Regular},
        {ArcherTower,  Regular},
        {Wall,         Small},
    };
    
};

#endif // __BUILDING_H__
