#include <iostream>

#include "BuildingDefense.h"

USING_NS_CC;

void BuildingDefense::updateAttackRangeGrid()
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
        if (building != nullptr && typeid(building) == typeid(BuildingDefense)) {
            defense = dynamic_cast<BuildingDefense*>(building);
            
            // int なのでキャストが起きる
            leftBeginX   = defense->coord.x - defense->maxRange;
            leftEndX     = defense->coord.x - defense->minRange;
            rightBeginX  = defense->coord.x - defense->minRange;
            rightEndX    = defense->coord.x - defense->maxRange;
            topBeginY    = defense->coord.y - defense->maxRange;
            topEndY      = defense->coord.y - defense->minRange;
            bottomBeginY = defense->coord.y - defense->minRange;
            bottomEndY   = defense->coord.y - defense->maxRange;
            
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
    }
    
    // 図が完成
    for (auto row: tmx->buildingAttackRangeGrid) {
        for (auto deffenses: row) {
            CCLOG("deffenses.size(%lu)",deffenses.size());
        }
    }
}