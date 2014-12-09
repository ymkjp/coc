#include <iostream>

#include "BuildingDefense.h"

USING_NS_CC;

bool BuildingDefense::virtualInit()
{
    this->minRange = this->getMinRange();
    this->maxRange = this->getMaxRange();
    this->schedule(schedule_selector(BuildingDefense::scan), getAttackSpeed());
    return true;
}

void BuildingDefense::scan(float frame)
{
    // ターゲットコンテナが空であれば何もしない
    if (targetUnits.empty()) {
        return;
    }
    
    float bestDistance = -1;
    float distanceSq;
    for (auto unit: targetUnits) {
        if (!inAttackRange(unit) || unit->status == Unit::Died)
        {
            targetUnits.eraseObject(unit);;
        }
        distanceSq = fabsf(unit->coord.getDistanceSq(this->coord));
        if (bestDistance == -1 || distanceSq < bestDistance) {
            bestDistance = distanceSq;
            targetUnit = unit;
        }
    }
    if (targetUnit) {
        this->attack();
    }
}

inline bool BuildingDefense::inAttackRange(Unit* unit)
{
    return true;
    
    auto distance = unit->coord.getDistanceSq(this->coord);
    CCLOG("BuildingDefense::inAttackRange(%f),getMaxRange(%f),getMinRange(%f),",distance,getMaxRange(),getMinRange());
    // @fixme
    return (getMinRange() <= distance && distance <= getMaxRange());
}

void BuildingDefense::updateAttackRangeGrid(Tmx* tmx)
{
    BuildingDefense* defense;
    int leftBeginX;
    int leftEndX;
    int rightBeginX;
    int rightEndX;
    int topBeginY;
    int topEndY;
    int bottomBeginY;
    int bottomEndY;
    
    // まずキャッシュをクリアする
    tmx->buildingAttackRangeGrid = {};
//    for (auto row: tmx->buildingAttackRangeGrid) {
//        for (auto deffenses: row) {
//            deffenses.clear();
//        }
//    }
    
    for (auto building: tmx->buildings) {
        defense = dynamic_cast<BuildingDefense*>(building);
        if (defense == nullptr) {
            continue;
        }
        // int なのでキャストが起きる
        leftBeginX   = normalize(defense->coord.x - defense->maxRange);
        leftEndX     = normalize(defense->coord.x - defense->minRange);
        rightBeginX  = normalize(defense->coord.x + defense->minRange);
        rightEndX    = normalize(defense->coord.x + defense->maxRange);
        topBeginY    = normalize(defense->coord.y - defense->maxRange);
        topEndY      = normalize(defense->coord.y - defense->minRange);
        bottomBeginY = normalize(defense->coord.y + defense->minRange);
        bottomEndY   = normalize(defense->coord.y + defense->maxRange);

        
        for (int x = leftBeginX; x < rightEndX; ++x) {
            for (int y = topBeginY; y < bottomEndY; ++y) {
                if (leftEndX < x && x < rightBeginX && topEndY < y && y < bottomBeginY) {
                    continue;
                } else {
                    tmx->buildingAttackRangeGrid[x][y].push_back(defense);
                }
            }
        }
    }
    
//    // 図が完成
//    int count = 0;
//    for (auto row: tmx->buildingAttackRangeGrid) {
//        for (auto deffenses: row) {
//            ++count; CCLOG("[%i]deffenses.size(%lu)",count,deffenses.size());
//        }
//    }
}

inline float BuildingDefense::normalize(float number)
{
    if (WORLD_MAP_HEIGHT < number) {
        number = WORLD_MAP_HEIGHT;
    } else if (0 > number) {
        number = 0;
    }
    return number;
}
