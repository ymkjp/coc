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
    
    Vec2 convertToCoord(Vec2 pos);
    Vec2 convertToRealPos(Vec2 pos);
    Vec2 convertToIso(Vec2 pos);
    
    void eraseBuilding(Building* building);
    bool noBuildings();
    
    // Caches
    BuildingCoordsByType buildingCoords;
    BuildingGrid buildingGrid = {{}};
    BuildingAttackRangeGrid buildingAttackRangeGrid = {{}};
    
    Vector<Unit*> units;
    Vector<Building*> buildings;
    
    void showBattleResult();
    
    UnitType getSelectedUnit();
    
    bool isRemainedUnitSelected();
    void decrementUnitCounter();
    
    static float calcCompassDegree(Vec2 startPos, Vec2 goalPos)
    {
        auto degree = calcDegree(startPos, goalPos);
        if (degree < 0) {
            degree += 360;
        }
        return degree;
    }
    
    static float calcDegree(Vec2 startPos, Vec2 goalPos)
    {
        auto diffPos = goalPos - startPos;
        auto radian = atan2(diffPos.y, diffPos.x);
        auto degree = radian * 180 / PI + 90;
        return degree;
    }
    
    Vector<Sprite*> domainCells = {};
    
    
protected:
    UI* ui;
    UnitCountByType unitRemainedCounterByType;

    const std::map<Stages,std::string> tmxFileNameByStage = {
        {Amigo,"map_01.tmx"},
        {Benito,"map_02.tmx"},
        {Carmen,"map_03.tmx"},
    };
    
    const UnitCountByType unitInitNumberByType = {
        {Barbarian,99},
        {Archer,99},
        {Giant,10},
        {Goblin,10},
        {Wallbreaker,10},
    };
    
};

#endif // __TMX_H__
