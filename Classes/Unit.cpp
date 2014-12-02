#include "Unit.h"

USING_NS_CC;

const std::map<Vec2, Unit::__COMPASS> Unit::compassByCoords =
{
    {Vec2(-1,-1), North},
    {Vec2(-1,0),  NorthEast},
    {Vec2(1,-1),  East},
    {Vec2(1,0),   SouthEast},
    {Vec2(1,1),   South},
    {Vec2(0,1),   SouthWest},
    {Vec2(-1,1),  West},
    {Vec2(-1,0),  NorthWest},
};

bool Unit::init(Tmx* _tmx, Vec2 _coord)
{
    tmx = _tmx;
    type = this->getUnitType();
    coord = _coord;
    damagePerSec = 60;
    action = Walking;
    
    actionTimelineCache = timeline::ActionTimelineCache::getInstance();
    
    unitNode = Node::create();
    auto childNode = this->getActingNode();
    actionTimeline = this->getActionTimeline();
    
    childNode->runAction(actionTimeline);
    actionTimeline->gotoFrameAndPlay(0, true);
    unitNode->addChild(childNode,1 ,"childNode");
    this->addChild(unitNode);
    
    this->play(1);
    return true;
}

void Unit::update( float frame )
{
}

void Unit::startAttacking()
{
    auto childNode = unitNode->getChildByName("childNode");
    auto newChildNode = this->getActingNode();

    auto action = this->getActionTimeline();
    newChildNode->runAction(action);
    action->gotoFrameAndPlay(0, true);

    unitNode->removeChild(childNode);
    unitNode->addChild(newChildNode ,1,"childNode");
    actionTimeline = action;

    if (this->action == Walking) {
        this->action = Attacking;
        this->schedule(schedule_selector(Unit::attack), 1.0f);
    }
}


void Unit::attack(float frame)
{
//    CCLOG("targetBuilding->status %i",this->targetBuilding->status);
    this->targetBuilding->attacked(damagePerSec);
    if (this->targetBuilding->status == Building::Died) {
//        CCLOG("[%i]Unit::attack target died!",__LINE__);
        action = Walking;
        
        auto childNode = unitNode->getChildByName("childNode");
        auto newChildNode = this->getActingNode();
        auto action = this->getActionTimeline();
        newChildNode->runAction(action);
        action->gotoFrameAndPlay(0, true);
        unitNode->removeChild(childNode);
        unitNode->addChild(newChildNode ,1,"childNode");
        actionTimeline = action;
        
        this->scheduleOnce(schedule_selector(Unit::play), 0);
        this->unschedule(schedule_selector(Unit::attack));
    }
}

inline bool Unit::isNextCoord(float num)
{
    return (num == -1.0f || num == 0.0f || num == 1.0f);
}

void Unit::play(float frame)
{
    CCLOG("Unit::play frame[%f]",frame);
    if (this->tmx->noBuildings()) {
        CCLOG("@todo GAME DONE!");
        return;
    }
    auto goalPoint = this->findPointToGo();
    auto mapNavigator = MapNavigator::create(this->tmx);
    auto path = mapNavigator->navigate(this->coord, goalPoint);
    this->coord = goalPoint;
    if (path.empty()) {
        CCLOG("HMM... EMPTY PATH IS DETECTED");
        return;
    };
    
    Vec2 nextCoord;
    Vec2 prevCoord;
    Vec2 directionPoint;
    Vector<FiniteTimeAction*> arrayOfactions;
    MoveTo* moveAction;
    while (!path.empty()) {
        nextCoord = path.top();
        directionPoint = this->tmx->convertToTile(nextCoord);
        moveAction = MoveTo::create(0.5, directionPoint);
        
        // 向き先に応じてアニメーションを切り替え
//                    Node* animatedNode = unit->createAnimatedNode(posDiff);
//                    FiniteTimeAction* func = CallFunc::create(CC_CALLBACK_0(Unit::animateNode, unit));

        FiniteTimeAction* func = CallFunc::create([=]() {
//            CCLOG("prevCoord(%f,%f),nextCoord(%f,%f)",prevCoord.x,prevCoord.y,nextCoord.x,nextCoord.y);
            this->setCompass(prevCoord, nextCoord);
            this->unitNode = this->getActingNode();
            this->actionTimeline = this->getActionTimeline();
        });
        arrayOfactions.pushBack(func);
        
        arrayOfactions.pushBack(moveAction);
        path.pop();
        prevCoord = nextCoord;
    }
    //        FiniteTimeAction* attack = CallFunc::create(CC_CALLBACK_0(BattleScene::attack, this));
    FiniteTimeAction* attack = CallFunc::create(CC_CALLBACK_0(Unit::startAttacking, this));
    arrayOfactions.pushBack(attack);
    auto seq = Sequence::create(arrayOfactions);

    this->unitNode->runAction(seq);    
}

void Unit::setCompass(Vec2 prevCoord, Vec2 nextCoord)
{
    auto posDiff = prevCoord - nextCoord;
    if (!this->isNextCoord(posDiff.x) || !this->isNextCoord(posDiff.y)) {
        return;
    }
    auto dx = posDiff.x;
    auto dy = posDiff.y;
    auto currentCompass = this->compass;
    if (dx == -1 && dy == -1) {
        // 真北の場合は前回の向きに影響される
        if (currentCompass == NorthEast
            || currentCompass == East
            || currentCompass == SouthEast) {
            this->compass = NorthEast;
        } else if (currentCompass == NorthWest
                   || currentCompass == West
                   || currentCompass == SouthWest) {
            this->compass = NorthWest;
        }
    } else if (dx == -1 && dy == 0) {
        this->compass = NorthWest;
    } else if (dx == -1 && dy == 1) {
        this->compass = West;
    } else if (dx == 0 && dy == -1) {
        this->compass = NorthEast;
    } else if (dx == 0 && dy == 1) {
        this->compass = SouthWest;
    } else if (dx == 1 && dy == -1) {
        this->compass = East;
    } else if (dx == 1 && dy == 0) {
        this->compass = SouthEast;
    } else if (dx == 1 && dy == 1) {
        // 真南の場合は前回の向きに影響される
        if (currentCompass == NorthEast
            || currentCompass == East
            || currentCompass == SouthEast) {
            this->compass = SouthEast;
        } else if (currentCompass == NorthWest
                   || currentCompass == West
                   || currentCompass == SouthWest) {
            this->compass = SouthWest;
        }
    }
}

std::vector<Vec2> Unit::getSurroundedCoords(Vec2 targetCoord)
{
    Building* building = this->tmx->buildingGrid[targetCoord.x][targetCoord.y];
    return Building::coordsSurround.at(building->getSpace());
}

Vec2 Unit::findPointToGo()
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
//    float distanceSq;
    Building* building;
    bool isToBreakWall = false;
    
    for (auto targetCoord: targetCoords) {
        building = this->tmx->buildingGrid[targetCoord.x][targetCoord.y];
        
        // 攻撃ターゲットcoordへの攻撃地点をユニットのレンジに応じて算出
        auto surroundCoords = this->getSurroundedCoords(targetCoord);
        for (auto coord: surroundCoords) {
            //            CCLOG("[isToBreakWall?%i] distanceSq(%f),bestScore(%f)",isToBreakWall,distanceSq,bestScore);
            auto startCoord = this->coord;
            auto goalCoord = building->coord + coord;
            
//            // 単純距離を用いて壁を攻撃したほうがいい場合のスコアを算出して、最も低ければcoordを採用
//            // @fixme 期待した動作にならないためスコア算出方法の見直し
//            distanceSq = fabs(startCoord.getDistanceSq(goalCoord));
//            if (bestScore == -1 || distanceSq * DISTANCE_WEIGHT_VS_PF < bestScore) {
//                //                CCLOG("[isToBreakWall]bestCoord(%f,%f),bestScore(%f)",bestCoord.x,bestCoord.y,bestScore);
//                isToBreakWall = true;
//                bestScore = distanceSq;
//                bestCoord = goalCoord;
//            }
            
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

std::vector<Vec2> Unit::getTargetCoords(Building::__CATEGORY category)
{
    auto types = Building::getTypesByCategory(category);
    
    std::vector<Vec2> targetCoords = {};
    for (auto type: types) {
//        CCLOG("[Unit::getTargetCoords]type%i",type);
        targetCoords.insert(
                            targetCoords.end(),
                            this->tmx->buildingCoords[type].begin(),
                            this->tmx->buildingCoords[type].end());
    }
    return targetCoords;
}

Node* Unit::getActingNode()
{
    return CSLoader::createNode(this->createFilename().getCString());
}

timeline::ActionTimeline* Unit::getActionTimeline()
{
    return actionTimelineCache->createAction(this->createFilename().getCString());
}
