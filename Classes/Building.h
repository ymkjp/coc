#ifndef __BUILDING_H__
#define __BUILDING_H__

#include <map>
#include <vector>

#include "cocos2d.h"
USING_NS_CC;

class Building : public Ref
{
public:
    enum __TYPE {
        NoType = 0,
        TownHall,
        ElixerTank,
        GoldBank,
        Canon,
        TrenchMortar,
        ArcherTower,
        Wall,
    };
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
    Node* node;
    __TYPE type;
    __STATUS status;
    
    virtual bool init(__TYPE type, Vec2 coord);
    static Building* create(__TYPE type, Vec2 coord) {
        auto p = new Building();
        if (p->init(type, coord)) {
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return nullptr;
    }
    virtual void addBuilding();
    
//    virtual __CATEGORY getCategory(__TYPE type)
//    {
//        return typeCategory.at(type);
//    };
//    
//    virtual __SPACE getSpace(__TYPE _type)
//    {
//        return typeSpace.at(_type);
//    }
//    
    virtual __SPACE getSpace();
    
    const static std::map<__CATEGORY, std::vector<__TYPE>> typesByCategory;
    static std::vector<__TYPE> getTypesByCategory(__CATEGORY category)
    {
        return Building::typesByCategory.at(category);
    };
    
    const static std::map<__SPACE, std::vector<Vec2>> coordsSurround;
    
protected:
//    const std::map<__TYPE, __CATEGORY> typeCategory = {
//        {TownHall,     TownHalls},
//        {ElixerTank,   Resources},
//        {GoldBank,     Resources},
//        {Canon,        Defenses},
//        {TrenchMortar, Defenses},
//        {ArcherTower,  Defenses},
//        {Wall,         Walls},
//    };
    const std::map<__TYPE, __SPACE> typeSpace = {
        {TownHall,     Large},
        {ElixerTank,   Regular},
        {GoldBank,     Regular},
        {Canon,        Regular},
        {TrenchMortar, Regular},
        {ArcherTower,  Regular},
        {Wall,         Small},
    };
    virtual Vec2 convertToIso(Vec2 pos);
};

#endif // __BUILDING_H__
