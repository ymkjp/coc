#include "UnitArcher.h"

USING_NS_CC;

UnitType UnitArcher::getUnitType()
{
    return Archer;
}

Vec2 UnitArcher::findPointToGo()
{
    // ユニットタイプに応じた攻撃ターゲットのカテゴリを取得
    std::vector<Vec2> targetCoords = {};
    Building::__CATEGORY targetCategory = attackType.at(type);

    // カテゴリに対応したcoordを bmx->buildingCoords から取得
    targetCoords = this->getTargetCoords(targetCategory);
    if (targetCoords.empty()) {
//        CCLOG("targetCoords is empty, Attack type: Melee");
        targetCoords = this->getTargetCoords(Building::Melee);
    }
    
    auto mapNavigator = MapNavigator::create(tmx);
    
    float bestScore = -1;
    Vec2 bestCoord;
    float lastNodeScore;
    float distanceSq;
    Building* building;
    bool isToBreakWall = false;
    
    for (auto targetCoord: targetCoords) {
        building = this->tmx->buildingGrid[targetCoord.x][targetCoord.y];
//        if (targetCategory == Building::Walls) {
//            // Walls (Wallbreaker) の場合
//            isToBreakWall = true;
//            break;
//        }
        
        // 攻撃ターゲットcoordへの攻撃地点をユニットのレンジに応じて算出
        auto surroundCoords = Building::coordsSurround.at(building->getSpace());
        for (auto coord: surroundCoords) {
//            CCLOG("[isToBreakWall?%i] distanceSq(%f),bestScore(%f)",isToBreakWall,distanceSq,bestScore);
            auto startCoord = this->coord;
            auto goalCoord = building->coord + coord;
            
            // 単純距離を用いて壁を攻撃したほうがいい場合のスコアを算出して、最も低ければcoordを採用
            // @fixme 期待した動作にならないためスコア算出方法の見直し
            distanceSq = fabs(startCoord.getDistanceSq(goalCoord));
            if (bestScore == -1 || distanceSq * DISTANCE_WEIGHT_VS_PF < bestScore) {
//                CCLOG("[isToBreakWall]bestCoord(%f,%f),bestScore(%f)",bestCoord.x,bestCoord.y,bestScore);
                isToBreakWall = true;
                bestScore = distanceSq;
                bestCoord = goalCoord;
//                    targetBuilding = building; // @todo remove this line
            }
            
            // 攻撃地点に建物が建っていればこの攻撃地点はスキップ
            if (!mapNavigator->isTravelable(goalCoord.x,goalCoord.y)) {
                CCLOG("There are some buildings at goalCoord(%f,%f)",goalCoord.x,goalCoord.y);
                continue;
            } else {
                CCLOG("Ready for nav! goalCoord(%f,%f)",goalCoord.x,goalCoord.y);
            }
            
            // 攻撃地点への経路探索を行って移動回数を算出する
            lastNodeScore = mapNavigator->navigate(startCoord, goalCoord).size();
            CCLOG("[!isToBreakWall]goalCoord(%f,%f),lastNodeScore(%f)",goalCoord.x,goalCoord.y,lastNodeScore);
            
            // 移動回数が最も少なければcoordを採用する
            if (bestScore == -1 || lastNodeScore < bestScore) {
                bestScore = lastNodeScore;
                bestCoord = goalCoord;
                targetBuilding = building;
                isToBreakWall = false;
            }
        }
    }
    // 壁を攻撃したほうが良い場合は壁のcoordを算出
    if  (isToBreakWall) {
        CCLOG("[isToBreakWall!]");
        targetCoords = this->getTargetCoords(Building::Walls);
        float bestScore = -1;
        float distanceSq;
        Building* wall;
        
        for (auto targetCoord: targetCoords) {
            wall = tmx->buildingGrid[targetCoord.x][targetCoord.y];
            
            // 壁への攻撃地点
            auto surroundCoords = Building::coordsSurround.at(wall->getSpace());
            for (auto coord: surroundCoords) {
                auto startCoord = this->coord;
                auto goalCoord = wall->coord + coord;
                
                // 建物が建っていればスキップ
                if (!mapNavigator->isTravelable(goalCoord.x,goalCoord.y) || building->isBuildingRange(goalCoord)) {
                    CCLOG("[WALL] There are some buildings at goalCoord(%f,%f)",goalCoord.x,goalCoord.y);
                    continue;
                } else {
                    CCLOG("[WALL] Ready for nav! goalCoord(%f,%f)",goalCoord.x,goalCoord.y);
                }
                
                // 「ユニットの位置〜攻撃地点 + 攻撃地点〜建物の距離」としてスコア化
                // @fixme 算出方法の見直し
                auto startToGoalDistance = fabs(startCoord.getDistanceSq(goalCoord));
                auto buildingToGoalDistance = fabs(goalCoord.getDistanceSq(building->coord));
                distanceSq = startToGoalDistance + 100 * buildingToGoalDistance;
                CCLOG("[calc]startToGoalDistance:%f,buildingToGoalDistance:%f,distanceSq:%f",
                      startToGoalDistance,
                      buildingToGoalDistance,
                      distanceSq
                      );
                if (bestScore == -1 || distanceSq < bestScore) {
                    bestScore = distanceSq;
                    bestCoord = goalCoord;
                    targetBuilding = wall;
                    CCLOG("[WALL] bestScore:%f,bestCoord(%f,%f)",bestScore,bestCoord.x,bestCoord.y);
                }
            }
        }
    }
    CCLOG("[end]bestCoord(%f,%f),bestScore(%f)",bestCoord.x,bestCoord.y,bestScore);
    return bestCoord;
}

Node* UnitArcher::getActingNode()
{
    Node* node;
    switch (this->action) {
        case Unit::Attacking:
            node = CSLoader::createNode("CocosProject/res/UnitArcherAttackEast.csb");
            break;
            
        default:
            node = CSLoader::createNode("CocosProject/res/UnitArcherWalkEast.csb");
            break;
    }
    return node;
}

timeline::ActionTimeline* UnitArcher::getActionTimeline()
{
    timeline::ActionTimeline* timeline;
    switch (this->action) {
        case Unit::Attacking:
            timeline = actionTimelineCache->createAction("CocosProject/res/UnitArcherAttackEast.csb");
            break;
            
        default:
            timeline = actionTimelineCache->createAction("CocosProject/res/UnitArcherWalkEast.csb");
            break;
    }
    return timeline;
}


void UnitArcher::animateNode()
{
    posDiff = Vec2(1,1);
    if (!this->isNextCoord(posDiff.x) || !this->isNextCoord(posDiff.y)) {
        CCLOG("POS DIFF OUT OF RANGE (%f,%f)",posDiff.x,posDiff.y);
        return;// this->unitNode;
    }
//    CCLOG("[type%i]posDiff(%f,%f)",type,posDiff.x,posDiff.y);
    
    std::string fileName = "CocosProject/res/Unit";
    switch (type) {
        case Archer:
            fileName.append("Archer");
            break;
        case Barbarian:
            fileName.append("Barbarian");
            break;
        case Giant:
            fileName.append("Giant");
            break;
        case Goblin:
            fileName.append("Goblin");
            break;
        case Wallbreaker:
            fileName.append("Wallbreaker");
            break;
        default:
            CCLOG("type%i",type);
            return;// this->unitNode;
            break;
    }
    fileName.append("Walk");
    
    auto directionCompass = compassByCoords.at(posDiff);
    CCLOG("directionCompass:%i",directionCompass);
    bool isFlip = false;
    switch (directionCompass) {
        case North:
            fileName.append("South");
            break;
        case East:
            fileName.append("East");
            break;
        case South:
            fileName.append("North");
            break;
        case West:
            fileName.append("East");
            isFlip = true;
            break;
        default:
            // @todo
            return;// this->unitNode;
            break;
    }
    fileName.append(".csb");
    CCLOG("[FileName]%s",fileName.c_str());
    
    auto node = CSLoader::createNode(fileName);
    auto actionTimeline = timeline::ActionTimelineCache::createAction(fileName);
    node->runAction(actionTimeline);
    actionTimeline->gotoFrameAndPlay(0, true);
    unitNode = node;
    return;
}


