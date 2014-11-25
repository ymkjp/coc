#include "Building.h"

const std::map<Building::__CATEGORY, std::vector<Building::__TYPE>> Building::typesByCategory =
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

bool Building::init(__TYPE _type, Vec2 _coord)
{
//    cache = SpriteFrameCache::getInstance();
    status = Building::Alive;
    type = _type;
    coord = _coord;
    this->addBuilding();
    return true;
}

Building::__SPACE Building::getSpace()
{
    return typeSpace.at(type);
}

void Building::addBuilding()
{
    
    return;
}

inline Vec2 Building::convertToIso(Vec2 rawPos)
{
    return Vec2(-1,-1);
    
//    Size tileSize = _tiledMap->getTileSize();
//    Size mapSize = _tiledMap->getMapSize();
//    auto pos = Vec2(rawPos.x, mapSize.height - rawPos.y);
//    
//    float ratio = tileSize.width / tileSize.height;
//    float x = tileSize.width * 0.5 * (pos.x/(tileSize.width / ratio) - pos.y/tileSize.height);
//    float y = tileSize.height * 0.5 * (mapSize.height - pos.x/(tileSize.width / ratio) - pos.y/tileSize.height);
//    return Vec2(x, y) + Vec2(mapSize.width * 0.65, mapSize.height);
}
