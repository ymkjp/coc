#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#define PI 3.1415926

#define DISPLAY_TIME_SPLASH_SCENE 0.02
#define TRANSITION_TIME 0.1

#define INIT_ZOOM_RATE 0.5
#define DEFAULT_ZOOM_RATE 0.3 // 0.65
#define MAX_ZOOM_RATE 3.5 // MIN: 0.521094

#define WORLD_MAP_WIDTH 44
#define WORLD_MAP_HEIGHT 44

#define DISTANCE_WEIGHT_VS_PF 2.0

#define BLOCK_LAYERS_NUM 7

#define PATH_FINDING_MAX_THRESHOLD 200

#define TILEDMAP_VISIBLE_ON false

class Tmx;
class Unit;
class Building;

inline static bool isInMapRange(Vec2 targetPoint)
{
    return (0 <= targetPoint.x
            && 0 <= targetPoint.y
            && targetPoint.x < WORLD_MAP_WIDTH
            && targetPoint.y < WORLD_MAP_HEIGHT);
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
    Archer = 0,
    Barbarian,
    Giant,
    Goblin,
    Wallbreaker,
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

// For Tmx
typedef std::map<BuildingType, std::vector<Vec2>> BuildingCoordsByType;
typedef std::array<std::array<Building*, WORLD_MAP_WIDTH>, WORLD_MAP_HEIGHT> BuildingGrid;

// For Building
typedef std::map<BuildingCategory, std::vector<BuildingType>> BuildingTypesByCategory;
typedef std::map<BuildingSpace, std::vector<Vec2>> BuildingVec2sBySpace;
typedef std::map<BuildingType, BuildingSpace> BuildingSpaceByType;

// For MapNavigator
typedef std::set<Vec2> Vec2Set;
typedef std::stack<Vec2> PathToGoal;

#endif // __DEFINITIONS_H__
