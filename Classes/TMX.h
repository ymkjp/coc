#ifndef __TMX_H__
#define __TMX_H__

#include <array>
#include <map>

#include "cocos2d.h"
USING_NS_CC;

#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;

#include "Definitions.h"
#include "AudioManager.h"
#include "Building.h"
#include "Unit.h"
#include "UI.h"
#include "MapNavigator.h"

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

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
    
    UI* ui;
    MapNavigator* mapNavigator;
    
    CSLoader* csLoader;
    timeline::ActionTimelineCache* actionTimelineCache;
    
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
    
    static Compass convertToCompass(float degree)
    {
        float compassIndex = ceil(degree / 45);
        if (compassIndex == 0) {
            return North;
        } else if (compassIndex == 1) {
            return NorthEast;
        } else if (compassIndex == 2) {
            return East;
        } else if (compassIndex == 3) {
            return SouthEast;
        } else if (compassIndex == 4) {
            return South;
        } else if (compassIndex == 5) {
            return SouthWest;
        } else if (compassIndex == 6) {
            return West;
        } else if (compassIndex == 7) {
            return NorthWest;
        } else {
            return North;
        }
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
    
    // UIメッセージ表示
    void showWarning(std::string warningMessage);
    
    // MapNavigator関連
    bool isTravelable(float posX, float posY);
    PathToGoal navigate(Vec2 startCoord, Vec2 goalCoord);
    void initWorldGrid();
    WorldGrid worldGrid;
    bool worldGridInitialized = false;
    
    // AudioManager
    void playSE(const std::string &id)
    {
        audioManager->playSE(id);
    }
    
protected:
    AudioManager* audioManager;
    
    int fullBattleSecond = 180;
    int currentBattleSecound = 0;
    int fullBuildingCount = 0;
    int currentBuildingCount = 0;
    
    UnitCountByType unitRemainedCounterByType;
    
    void decreaseTimerCount(float frame);
    void detectUnitAnnihilation(float frame);
    
    void hideMessage(float frame);
    
    void killAllUnits(float frame);
    
    const std::map<Stages,std::string> tmxFileNameByStage = {
        {Amigo,"map_01.tmx"},
        {Benito,"map_02.tmx"},
        {Carmen,"map_03.tmx"},
    };
    
    const UnitCountByType unitInitNumberByType = {
        {Barbarian,50},
        {Archer,50},
        {Giant,50},
        {Goblin,50},
        {Wallbreaker,50},
//        {Barbarian,20},
//        {Archer,20},
//        {Giant,20},
//        {Goblin,20},
//        {Wallbreaker,20},
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
