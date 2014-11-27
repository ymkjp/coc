#include "Tmx.h"
USING_NS_CC;

bool Tmx::init()
{
    tiledMap = TMXTiledMap::create("map_01.tmx");
    domainTMXLayer = tiledMap->getLayer("Domain");
    wallTMXLayer = tiledMap->getLayer("Wall");
    return true;
}

Vec2 Tmx::convertToCoord(Vec2 pos)
{
    Size tileSize = tiledMap->getTileSize();
    Point deployedPoint = domainTMXLayer->convertToNodeSpace(pos);
    Size domainSize = domainTMXLayer->getContentSize();
    
    float slopeFormula = tileSize.height / tileSize.width;
    float tileX = floor((slopeFormula * deployedPoint.x + domainSize.height * 0.5 - deployedPoint.y) / tileSize.height);
    float tileY = floor((-slopeFormula * deployedPoint.x + domainSize.height * 1.5 - deployedPoint.y) / tileSize.height);
    return Vec2(tileX, tileY);
}

Vec2 Tmx::convertToTile(Vec2 pos)
{
    return tiledMap->getLayer("Domain")->getPositionAt(pos) + tiledMap->getTileSize() * 0.5;
}

Vec2 Tmx::convertToIso(Vec2 rawPos)
{
    Size tileSize = tiledMap->getTileSize();
    Size mapSize = tiledMap->getMapSize();
    auto pos = Vec2(rawPos.x, mapSize.height - rawPos.y);
    
    float ratio = tileSize.width / tileSize.height;
    float x = tileSize.width * 0.5 * (pos.x/(tileSize.width / ratio) - pos.y/tileSize.height);
    float y = tileSize.height * 0.5 * (mapSize.height - pos.x/(tileSize.width / ratio) - pos.y/tileSize.height);
    return Vec2(x, y) + Vec2(mapSize.width * 0.65, mapSize.height);
}

void Tmx::eraseBuilding(Building* building)
{
    auto coords = &buildingCoords.at(building->type);
    CCLOG("[Tmx::eraseBuilding] type:%i, building->coord(%f,%f)",building->type,building->coord.x,building->coord.y);
    
    for(int i =0; i <coords->size(); ++i){
        CCLOG("[Tmx::eraseBuilding] cached coord(%f,%f)",coords->at(i).x,coords->at(i).y);
    }
    
    coords->erase(remove(coords->begin(), coords->end(), building->coord), coords->end());
    
    CCLOG("[Tmx::eraseBuilding] coords.size%lu",coords->size());
    for(int i =0; i <coords->size(); ++i){
        CCLOG("[Tmx::eraseBuilding] remain coord(%f,%f)",coords->at(i).x,coords->at(i).y);
    }
    
    buildingGrid[building->coord.x][building->coord.y] = nullptr;
}