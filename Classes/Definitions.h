#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#define DISPLAY_TIME_SPLASH_SCENE 0.02
#define TRANSITION_TIME 0.1

#define INIT_ZOOM_RATE 0.5
#define DEFAULT_ZOOM_RATE 0.3 // 0.65
#define MAX_ZOOM_RATE 3.5 // MIN: 0.521094

#define WORLD_MAP_WIDTH 44
#define WORLD_MAP_HEIGHT 44

#define DISTANCE_WEIGHT_VS_PF 2.0

#define BLOCK_LAYERS_NUM 7

#define TILEDMAP_VISIBLE_ON false


inline static bool isInMapRange(Vec2 targetPoint)
{
    return (0 <= targetPoint.x
            && 0 <= targetPoint.y
            && targetPoint.x < WORLD_MAP_WIDTH
            && targetPoint.y < WORLD_MAP_HEIGHT);
};

enum BuildingType {
    NoType = 0,
    TownHall,
    ElixerTank,
    GoldBank,
    Canon,
    TrenchMortar,
    ArcherTower,
    Wall,
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


#endif // __DEFINITIONS_H__
