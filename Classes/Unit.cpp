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

    actionTimelineCache = timeline::ActionTimelineCache::getInstance();
    
    unitNode = Node::create();
    auto childNode = CSLoader::createNode("CocosProject/res/UnitBarbarianWalkEast.csb");
    actionTimeline = actionTimelineCache->createAction("CocosProject/res/UnitBarbarianWalkEast.csb");
    childNode->runAction(actionTimeline);
    actionTimeline->gotoFrameAndPlay(0, true);
    unitNode->addChild(childNode,1 ,"childNode");
    
//    cocos2d::Node *node = CSLoader::getInstance()->createNodeFromProtocolBuffers("HogeScene.csb");
//    this->addChild(node);
    
    this->play();
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

void Unit::play()
{
    auto goalCoord = this->findGoalCoord(this->coord, Building::Resources);
    auto mapNavigator = MapNavigator::create(tmx->tiledMap);
    auto path = mapNavigator->navigate(this->coord, goalCoord);
    
    if (path->empty()) {CCLOG("HMM... EMPTY PATH IS DETECTED");return;};
    
    Vec2 nextCoord;
    Vec2 prevCoord;
    Vec2 directionPoint;
    Vector<FiniteTimeAction*> arrayOfactions;
    MoveTo* moveAction;
    while (!path->empty()) {
        nextCoord = path->top();
        directionPoint = tmx->convertToTile(nextCoord);
        moveAction = MoveTo::create(0.5, directionPoint);
        auto posDiff =  Vec2((int)prevCoord.x - (int)nextCoord.x, (int)prevCoord.y - (int)nextCoord.y);
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
        path->pop();
        prevCoord = nextCoord;
    }
    //        FiniteTimeAction* attack = CallFunc::create(CC_CALLBACK_0(BattleScene::attack, this));
    FiniteTimeAction* attack = CallFunc::create(CC_CALLBACK_0(Unit::attack, this));
    arrayOfactions.pushBack(attack);
    auto seq = Sequence::create(arrayOfactions);

    this->unitNode->runAction(seq);    
}


/**
 @todo Highlight the nearest coord
 */
inline Vec2 Unit::findGoalCoord(Vec2 startCoord, Building::__CATEGORY targetCategory)
{
    // 1. ターゲットのマスを経路探索で近いものから算出
    // 2.　ターゲット4マスの周囲12マスに経路探索をかけて最もコストの低かったマスを goalCoord とする
    // 2'. ターゲット9マスの周囲16マスに経路探索をかけて最もコストの低かったマスを goalCoord とする
    // 3. 経路探索でゴールにたどり着かなかった場合、単純移動距離の短い壁を goalCoord とする // todo
    
    auto types = Building::getTypesByCategory(targetCategory);
    
    std::vector<Vec2> targetCoords;
    for (auto type: types) {
        targetCoords.insert(
                            targetCoords.end(),
                            tmx->buildingCoords[type].begin(),
                            tmx->buildingCoords[type].end());
    }
    
    auto navi = MapNavigator::create(tmx->tiledMap);
    
    Vec2 nearestCoord = Vec2(-1,-1);
    float nearestDistance;
    float distance;
    for (auto coord: targetCoords) {
        distance = startCoord.getDistanceSq(coord);
        if (nearestCoord == Vec2(-1,-1) ||  distance < nearestDistance) {
            nearestDistance = distance;
            nearestCoord = coord;
        }
    }
    
    auto space = tmx->buildingGrid.at(nearestCoord.x).at(nearestCoord.y)->getSpace();
    auto coordsSurround = Building::coordsSurround.at(space);
    int bestScore = -1;
    Vec2 bestCoord;
    float lastNodeScore;
    for (auto coord: coordsSurround) {
        lastNodeScore = navi->findLastNode(startCoord, nearestCoord + coord)->GetScore();
        if (bestScore == -1 || lastNodeScore < bestScore) {
            bestScore = lastNodeScore;
            bestCoord = nearestCoord + coord;
        }
    }
    return bestCoord;
}


void Unit::attack()
{
    action = Attacking;
    CCLOG("Unit attattak%i",__LINE__);
    auto childNode = unitNode->getChildByName("childNode");
    auto newChildNode = CSLoader::createNode("CocosProject/res/UnitBarbarianAttackEast.csb");

    auto action = actionTimelineCache->createAction("CocosProject/res/UnitBarbarianAttackEast.csb");
    newChildNode->runAction(action);
    action->gotoFrameAndPlay(0, true);

    childNode->stopAction(actionTimeline);
    unitNode->removeChild(childNode);
    unitNode->addChild(newChildNode /** ,1,"childNode"*/ );
    
//    actionTimeline = action;
    CCLOG("Unit attattak finished%i",__LINE__);
    
}

void Unit::animateNode()
{
    posDiff = Vec2(1,1);
    if (!this->isNextCoord(posDiff.x) || !this->isNextCoord(posDiff.y)) {
        CCLOG("POS DIFF OUT OF RANGE (%f,%f)",posDiff.x,posDiff.y);
        return;// this->unitNode;
    }
    CCLOG("[type%i]posDiff(%f,%f)",type,posDiff.x,posDiff.y);
    
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
//    node-
    actionTimeline->gotoFrameAndPlay(0, true);
    unitNode = node;
    return;// unitNode;
}


