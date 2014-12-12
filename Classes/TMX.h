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
    enum __BATTLE_STATUS  {
        WaitingForStart,
        Playing,
        Paused,
        Finished,
    } battleStatus;
    
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
    
    void startBattle();
    void eraseBuilding(Building* building);
    bool noBuildings();
    
    // Caches
    BuildingCoordsByType buildingCoords;
    BuildingGrid buildingGrid = {{}};
    BuildingAttackRangeGrid buildingAttackRangeGrid = {{}};
    GraveGrid graveGrid = {{}};
    
    // 経路のキャッシュ
    std::map<std::array<Vec2,2>, std::stack<Vec2>> pathCache;
    void cachePath(std::array<Vec2,2> cacheKey, std::stack<Vec2> path);
    
    Vector<Unit*> units;
    Vector<Building*> buildings;
    
    void showBattleResult();
    
    UnitType getSelectedUnit();
    
    bool isRemainedUnitSelected();
    void decrementUnitCounter();
    
    int count = 0;
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
    Vector<Sprite*> buildingShadowCells = {};
    Vector<Sprite*> grassCells = {};
    
    void increaseBattleScore(ScoreType type, float amount);
    BattleScoreByType* getBattleScoreResult();
    
    void initBattleControllerCounters();
    void countUpRemainingScore(ScoreType type, float amount);
    void incrementBuildingCount();
    void decrementBuildingCount();
    
protected:
    int fullBattleSecond = 180;
    int currentBattleSecound = 0;
    int fullBuildingCount = 0;
    int currentBuildingCount = 0;
    
    UI* ui;
    UnitCountByType unitRemainedCounterByType;

    void decreaseTimerCount(float frame);
    void detectUnitAnnihilation(float frame);
    
    const std::map<Stages,std::string> tmxFileNameByStage = {
        {Amigo,"map_01.tmx"},
        {Benito,"map_02.tmx"},
        {Carmen,"map_03.tmx"},
    };
    
    const UnitCountByType unitInitNumberByType = {
        {Barbarian,10},
        {Archer,15},
        {Giant,15},
        {Goblin,10},
        {Wallbreaker,10},
    };
    
    BattleScoreByType remainingScoreByType = {
        {ElixerScore,0},
        {CoinScore,0},
        {DestructionRatioScore,0},
        {StarCountScore,0},
    };
    
    BattleScoreByType earnedBattleScoreByType = {
        {ElixerScore,0},
        {CoinScore,0},
        {DestructionRatioScore,0},
        {StarCountScore,0},
    };
};

#endif // __TMX_H__
