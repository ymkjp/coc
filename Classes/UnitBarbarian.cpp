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

bool Unit::init(Tmx* _tmx, UnitType unitType, Vec2 _coord)
{
    tmx = _tmx;
    type = unitType;
    coord = _coord;
    damagePerSec = 60;
    action = Walking;
    
    actionTimelineCache = timeline::ActionTimelineCache::getInstance();
    
    unitNode = Node::create();
    auto childNode = CSLoader::createNode("CocosProject/res/UnitBarbarianWalkEast.csb");
    actionTimeline = actionTimelineCache->createAction("CocosProject/res/UnitBarbarianWalkEast.csb");
    childNode->runAction(actionTimeline);
    actionTimeline->gotoFrameAndPlay(0, true);
    unitNode->addChild(childNode,1 ,"childNode");
    this->addChild(unitNode);
//    cocos2d::Node *node = CSLoader::getInstance()->createNodeFromProtocolBuffers("HogeScene.csb");
//    this->addChild(node);
    
    this->play(1);
    return true;
}

void Unit::update( float frame )
{
}


inline bool Unit::isNextCoord(float num)
{
    return (num == -1.0f || num == 0.0f || num == 1.0f);
}

Node* Unit::createAnimatedNode(Vec2 posDiff)
{
    return this->unitNode;
}

void Unit::play(float frame)
{
    CCLOG("Unit::play frame[%f]",frame);
    if (tmx->noBuildings()) {
        CCLOG("@todo GAME DONE!");
        return;
    }
    auto goalPoint = this->findPointToGo();
    auto mapNavigator = MapNavigator::create(tmx);
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
        directionPoint = tmx->convertToTile(nextCoord);
        moveAction = MoveTo::create(0.5, directionPoint);
        auto posDiff =  Vec2((int)prevCoord.x - (int)nextCoord.x, (int)prevCoord.y - (int)nextCoord.y);
        // @todo 向き先に応じてアニメーションを切り替え
        //            Node* animatedNode = unit->createAnimatedNode(posDiff);
        //            FiniteTimeAction* func = CallFunc::create(CC_CALLBACK_0(Unit::animateNode, unit));
        //            FiniteTimeAction* func = CallFuncN::create([this,&animatedNode](Ref* target) {
        //                auto node = tiledMapLayer->getChildByName("unit");
        //                CCLOG("[node:%s]",node->getName().c_str());
        ////                unit->unitNode = animatedNode;
        //                tiledMapLayer->removeChildByName("unit");
        //                tiledMapLayer->addChild(animatedNode,1,"unit");
        //                // @todo
        //            });
        //            arrayOfactions.pushBack(func);
        
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


inline std::vector<Vec2> Unit::getTargetCoords(Building::__CATEGORY category)
{
    auto types = Building::getTypesByCategory(category);
    
    std::vector<Vec2> targetCoords = {};
    for (auto type: types) {
//        CCLOG("[Unit::getTargetCoords]type%i",type);
        targetCoords.insert(
                            targetCoords.end(),
                            tmx->buildingCoords[type].begin(),
                            tmx->buildingCoords[type].end());
    }
    return targetCoords;
}

/**
 @todo Highlight the nearest coord
 ターゲット {4マスの周囲12マス|9マスの周囲16マス} に経路探索をかけて最もコストの低かったマスを goalCoord とする
 */
inline Vec2 Unit::findPointToGo()
{
    std::vector<Vec2> targetCoords = {};
    Building::__CATEGORY targetCategory = attackType.at(type);
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
        building = tmx->buildingGrid[targetCoord.x][targetCoord.y];
        if (targetCategory == Building::Walls) {
            // Walls (Wallbreaker) の場合
            isToBreakWall = true;
            break;
        }
        auto surroundCoords = Building::coordsSurround.at(building->getSpace());
        for (auto coord: surroundCoords) {
//            CCLOG("[isToBreakWall?%i] distanceSq(%f),bestScore(%f)",isToBreakWall,distanceSq,bestScore);
            auto startCoord = this->coord;
            auto goalCoord = building->coord + coord;
            distanceSq = fabs(goalCoord.getDistanceSq(startCoord));
            if (bestScore == -1 || distanceSq * DISTANCE_WEIGHT_VS_PF < bestScore) {
//                CCLOG("[isToBreakWall]bestCoord(%f,%f),bestScore(%f)",bestCoord.x,bestCoord.y,bestScore);
                isToBreakWall = true;
                bestScore = distanceSq;
                bestCoord = goalCoord;
//                    targetBuilding = building; // @todo remove this line
            }
            if (!mapNavigator->isTravelable(goalCoord.x,goalCoord.y)) {
                CCLOG("There are some buildings at goalCoord(%f,%f)",goalCoord.x,goalCoord.y);
                continue;
            } else {
                CCLOG("Ready for nav! goalCoord(%f,%f)",goalCoord.x,goalCoord.y);
            }
            lastNodeScore = mapNavigator->navigate(startCoord, goalCoord).size();
            CCLOG("[!isToBreakWall]goalCoord(%f,%f),lastNodeScore(%f)",goalCoord.x,goalCoord.y,lastNodeScore);
            if (bestScore == -1 || lastNodeScore < bestScore) {
                bestScore = lastNodeScore;
                bestCoord = goalCoord;
                targetBuilding = building;
                isToBreakWall = false;
            }
        }
    }
    if  (isToBreakWall) {
        CCLOG("[isToBreakWall!]");
        targetCoords = this->getTargetCoords(Building::Walls);
        float bestScore = -1;
        float distanceSq;
        Building* wall;
        
        for (auto targetCoord: targetCoords) {
            wall = tmx->buildingGrid[targetCoord.x][targetCoord.y];
            auto surroundCoords = Building::coordsSurround.at(wall->getSpace());
            for (auto coord: surroundCoords) {
                auto startCoord = this->coord;
                auto goalCoord = wall->coord + coord;
                if (!mapNavigator->isTravelable(goalCoord.x,goalCoord.y) || building->isBuildingRange(goalCoord)) {
                    CCLOG("[WALL] There are some buildings at goalCoord(%f,%f)",goalCoord.x,goalCoord.y);
                    continue;
                } else {
                    CCLOG("[WALL] Ready for nav! goalCoord(%f,%f)",goalCoord.x,goalCoord.y);
                }
                auto startToGoalDistance = fabs(startCoord.getDistanceSq(goalCoord));
                auto buildingToGoalDistance = fabs(building->coord.getDistanceSq(goalCoord));
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

void Unit::startAttacking()
{
    auto childNode = unitNode->getChildByName("childNode");
    auto newChildNode = CSLoader::createNode("CocosProject/res/UnitBarbarianAttackEast.csb");

    auto action = actionTimelineCache->createAction("CocosProject/res/UnitBarbarianAttackEast.csb");
    newChildNode->runAction(action);
    action->gotoFrameAndPlay(0, true);

//    childNode->stopAction(actionTimeline);
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
    CCLOG("targetBuilding->status %i",this->targetBuilding->status);
    this->targetBuilding->attacked(damagePerSec);
    if (this->targetBuilding->status == Building::Died) {
        CCLOG("[%i]Unit::attack target died!",__LINE__);
        action = Walking;
        
        auto childNode = unitNode->getChildByName("childNode");
        auto newChildNode = CSLoader::createNode("CocosProject/res/UnitBarbarianWalkEast.csb");
        auto action = actionTimelineCache->createAction("CocosProject/res/UnitBarbarianWalkEast.csb");
        newChildNode->runAction(action);
        action->gotoFrameAndPlay(0, true);
        unitNode->removeChild(childNode);
        unitNode->addChild(newChildNode ,1,"childNode");
        actionTimeline = action;
        
        this->scheduleOnce(schedule_selector(Unit::play), 0);
        this->unschedule(schedule_selector(Unit::attack));
    }
}

void Unit::animateNode()
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


