#include "Building.h"

USING_NS_CC;

const std::map<Building::__CATEGORY, std::vector<BuildingType>> Building::typesByCategory =
{
    {Resources, {ElixerTank, GoldBank}},
    {Defenses, {Canon, TrenchMortar, ArcherTower}},
    {AnyBuildings, {TownHall, ElixerTank, GoldBank, Canon, TrenchMortar, ArcherTower, /** Wall */}},
};

const std::map<Building::__SPACE, std::vector<Vec2>> Building::coordsSurround = {
    {Regular, /** count: 12 */ {
        Vec2(-1,-1), Vec2(-1,0), Vec2(-1,1), Vec2(-1,2),
        Vec2(0,-1), Vec2(0,2),
        Vec2(1,-1), Vec2(1,2),
        Vec2(2,-1), Vec2(2,0), Vec2(2,1), Vec2(2,2),
    }},
    {Large,  /** count: 16 */ {
        Vec2(-1,-1), Vec2(-1,0), Vec2(-1,1), Vec2(-1,2), Vec2(-1,3),
        Vec2(0,-1), Vec2(0,3),
        Vec2(1,-1), Vec2(1,3),
        Vec2(2,-1), Vec2(2,3),
        Vec2(3,-1), Vec2(3,0), Vec2(3,1), Vec2(3,2), Vec2(3,3),
    }},
};

bool Building::init(Tmx* _tmx, BuildingType _type, Vec2 _coord)
{
    tmx = _tmx;
//    cache = SpriteFrameCache::getInstance();
    status = Building::Alive;
    type = _type;
    coord = _coord;
    return true;
}

Building::__SPACE Building::getSpace()
{
    return typeSpace.at(type);
}
