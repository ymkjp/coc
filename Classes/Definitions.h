#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

USING_NS_CC;

#define PI 3.141592653589793

#define QUICK_DEBUG_MODE true
#define DISPLAY_TIME_SPLASH_SCENE 0.02
#define TRANSITION_TIME 0.1

//#define DEFAULT_ZOOM_RATE 0.521094 // For debug 全画面表示
#define DEFAULT_ZOOM_RATE 0.54
#define MAX_ZOOM_RATE 3.5 // MIN: 0.521094

#define WORLD_MAP_WIDTH 44
#define WORLD_MAP_HEIGHT 44

#define PATH_FINDING_MAX_THRESHOLD 200

#define TILEDMAP_VISIBLE_ON false

class Tmx;
class MapNavigator;
class UI;
class Unit;
class Building;
class BuildingDefense;
class Grave;

inline static bool isInMapRange(Vec2 targetPoint)
{
    return (0 <= targetPoint.x
            && 0 <= targetPoint.y
            && targetPoint.x < WORLD_MAP_WIDTH
            && targetPoint.y < WORLD_MAP_HEIGHT);
};

// 地面の上の施設 44*2 より大きくするとバトルマップで上に表示される
enum BattleSceneZOrder {
    // 地面の上の影
    BulletShadowZOrder = 400,
    
    // 地面の上の効果
    MortalImpactZOrder = 450,
    
    // 飛行しているもの
    BulletZOrder = 500,
    ArrowZOrder = 500,
    
    // UI要素
    TargetMarkerZOrder = 1000,
    LifeGageZOrder = 2000,
};

enum TiledMapOrder {
    DomainOrder = 1,
    GrassOrder,
    BuildingShadowOrder,
    BuildingOrder,
    UnitOrder,
};

enum Stages {
    Amigo,
    Benito,
    Carmen,
};

enum ScoreType {
    ElixerScore,
    CoinScore,
    DestructionRatioScore,
    StarCountScore,
};

enum BuildingType {
    NoBuidlingType = 0,
    TownHall,
    ElixerTank,
    GoldBank,
    Canon,
    TrenchMortar,
    ArcherTower,
    Wall,
};

enum BuildingCategory {
    NoBuildingCategory = 0,
    Melee,
    TownHalls,
    Resources,
    Defenses,
    Walls,
};

enum BuildingSpace {
    Small = 1,
    Regular = 6,
    Large = 9,
};

enum UnitType {
    NoUnitType = 0,
    Archer,
    Barbarian,
    Giant,
    Goblin,
    Wallbreaker,
};


struct AStar
{
    enum __STATUS
    {
        NONE,
        OPEN,
        CLOSE,
        UNABLE,
    };
    __STATUS status;
    float cost;
    float distance;
    AStar *parent;
    Vec2 pos;
    float GetScore()
    {
        return cost + distance;
    }
};

enum Compass {
    North,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    NorthWest,
};

enum UITypes {
    BattleControllerUI,
    BattleResultUI,
};

const std::map<UnitType, std::string> unitNameByType =
{
    {Archer, "Archer"},
    {Barbarian, "Barbarian"},
    {Giant, "Giant"},
    {Goblin, "Goblin"},
    {Wallbreaker, "Wallbreaker"},
};

const std::map<UnitType, std::string> unitSmallCaseNameByType =
{
    {Archer, "archer"},
    {Barbarian, "barbarian"},
    {Giant, "giant"},
    {Goblin, "goblin"},
    {Wallbreaker, "wallbreaker"},
};


/**
 ElixerScore,
 CoinScore,
 DestructionRatioScore,
 StarCountScore,
 */
const std::map<ScoreType, std::string> scoreNameByType =
{
    {ElixerScore, "Elixer"},
    {CoinScore, "Coin"},
};

// For Tmx
typedef std::map<BuildingType, std::vector<Vec2>> BuildingCoordsByType;
typedef std::array<std::array<Building*, WORLD_MAP_WIDTH>, WORLD_MAP_HEIGHT> BuildingGrid;
typedef std::array<std::array<std::vector<BuildingDefense*>, WORLD_MAP_WIDTH>, WORLD_MAP_HEIGHT> BuildingAttackRangeGrid;
typedef std::map<UnitType,int> UnitCountByType;
typedef std::array<std::array<Grave*, WORLD_MAP_WIDTH>, WORLD_MAP_HEIGHT> GraveGrid;
typedef std::map<ScoreType,float> BattleScoreByType;
typedef std::array<std::array<AStar,WORLD_MAP_WIDTH>,WORLD_MAP_HEIGHT> WorldGrid;

// For Building
typedef std::map<BuildingCategory, std::vector<BuildingType>> BuildingTypesByCategory;
typedef std::map<BuildingSpace, std::vector<Vec2>> BuildingVec2sBySpace;
typedef std::map<BuildingType, BuildingSpace> BuildingSpaceByType;

// For MapNavigator
typedef std::set<Vec2> Vec2Set;
typedef std::stack<Vec2> PathToGoal;


#endif // __DEFINITIONS_H__
