#ifndef __TMX_H__
#define __TMX_H__

#include <array>
#include <map>

#include "cocos2d.h"
USING_NS_CC;

#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;

#include "Definitions.h"
#include "Building.h"
#include "Unit.h"

class Tmx : public Ref
{
public:
    virtual bool init();
    CREATE_FUNC(Tmx);

    TMXTiledMap* tiledMap;
    TMXLayer *domainTMXLayer;
    TMXLayer *wallTMXLayer;
    
    Vec2 convertToCoord(Vec2 pos);
    Vec2 convertToTile(Vec2 pos);
    Vec2 convertToIso(Vec2 pos);
    
    void eraseBuilding(Building* building);
    bool noBuildings();
    
    // Caches
    BuildingCoordsByType buildingCoords;
    BuildingGrid buildingGrid = {{}};
    BuildingAttackRangeGrid buildingAttackRangeGrid = {{}};
    
    Vector<Unit*> units;
    Vector<Building*> buildings;
};

#endif // __TMX_H__
