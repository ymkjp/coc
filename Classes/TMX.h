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
#include "UI.h"

class Tmx : public Node
{
public:
    virtual bool init(Stages stage);
    static Tmx* create(Stages stage) {
        auto p = new Tmx();
        if (p->init(stage)) {
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return nullptr;
    }

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
    
    void showBattleController();
    void showBattleResult();
    
    UnitType getSelectedUnit();
    

protected:
    UI* ui;
    const std::map<Stages,std::string> tmxFileNameByStages = {
        {Amigo,"map_01.tmx"},
        {Benito,"map_02.tmx"},
        {Carmen,"map_03.tmx"},
    };
};

#endif // __TMX_H__
