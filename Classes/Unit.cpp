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
    compass = East;
    
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

void Unit::testAdd(__String fileName, Vec2 pos)
{
    auto node = CSLoader::createNode(fileName.getCString());
    auto anim = actionTimelineCache->createAction(fileName.getCString());
    node->runAction(anim);
    anim->gotoFrameAndPlay(0, true);
    node->setScale(5);
    node->setPosition(pos);
    this->addChild(node);
}

void Unit::update( float frame )
{
}

void Unit::startAttacking()
{
    this->action = Attacking;
    this->updateNode();
    this->schedule(schedule_selector(Unit::attack), 1.0f);
}


void Unit::attack(float frame)
{
//    CCLOG("targetBuilding->status %i",this->targetBuilding->status);
    this->targetBuilding->attacked(damagePerSec);
    if (this->targetBuilding->status == Building::Died) {
        CCLOG("[%i]Unit::attack target died!",__LINE__);
        action = Walking;
        this->updateNode();
        this->scheduleOnce(schedule_selector(Unit::play), 0);
        this->unschedule(schedule_selector(Unit::attack));
    }
}

inline void Unit::updateNode()
{
    auto childNode = unitNode->getChildByName("childNode");
    auto newChildNode = this->getActingNode();
    auto action = this->getActionTimeline();
    newChildNode->runAction(action);
    action->gotoFrameAndPlay(0, true);
    unitNode->removeChild(childNode);
    unitNode->addChild(newChildNode ,1,"childNode");
    actionTimeline = action;
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
    auto startCoord = this->coord;
    auto goalPoint = this->findPointToGo();
    auto mapNavigator = MapNavigator::create(this->tmx);
    auto path = mapNavigator->navigate(startCoord, goalPoint);
//    CCLOG("Over steps?:%i",mapNavigator->isOverSteps);
    if (mapNavigator->isOverSteps) {
        // 経路を閾値内で見つけられなかった場合、最寄りの壁を攻撃する
        goalPoint = findNearestWallGoalPoint();
//        CCLOG("[Wall]goalPoint(%f,%f)",goalPoint.x,goalPoint.y);
        path = mapNavigator->navigate(startCoord, goalPoint);
    }
    
    Vec2 nextCoord;
    Vec2 prevCoord = startCoord;
    Vec2 directionPoint;
    Vector<FiniteTimeAction*> arrayOfactions;
    MoveTo* moveAction;
    
    // 道のりがあれば移動 (なければ攻撃開始)
    while (!path.empty()) {
        nextCoord = path.top();
        directionPoint = this->tmx->convertToTile(nextCoord);
        moveAction = MoveTo::create(0.5, directionPoint);
        
        // 向き先に応じてアニメーションを切り替え
        FiniteTimeAction* func = CallFunc::create([=]() {
//            CCLOG("[CallFunc] prevCoord(%f,%f),nextCoord(%f,%f)",prevCoord.x,prevCoord.y,nextCoord.x,nextCoord.y);
            this->setCompass(prevCoord, nextCoord);
            this->updateNode();
            this->coord = nextCoord;
        });
        arrayOfactions.pushBack(func);
        
        arrayOfactions.pushBack(moveAction);
        path.pop();
        prevCoord = nextCoord;
    }
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
    
//    CCLOG("[Compass]currentCompass:%i,this->compass:%i,",currentCompass,this->compass);
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
        // 好みの標的がなければ Melee mode
        targetCoords = this->getTargetCoords(Building::Melee);
    }
    
    auto mapNavigator = MapNavigator::create(tmx);
    
    float bestScore = -1;
    float distanceSq;
    Vec2 bestCoord;
    Building* building;
    
    for (auto targetCoord: targetCoords) {
        building = this->tmx->buildingGrid[targetCoord.x][targetCoord.y];
        
        // 攻撃ターゲットcoordへの攻撃地点をユニットのレンジに応じて算出
        auto surroundCoords = this->getSurroundedCoords(targetCoord);
        for (auto coord: surroundCoords) {
            //            CCLOG("[isToBreakWall?%i] distanceSq(%f),bestScore(%f)",isToBreakWall,distanceSq,bestScore);
            auto startCoord = this->coord;
            auto goalCoord = building->coord + coord;
            
            // 攻撃地点に建物が建っていればこの攻撃地点はスキップ
            if (!mapNavigator->isTravelable(goalCoord.x,goalCoord.y)) {
//                CCLOG("There are some buildings at goalCoord(%f,%f)",goalCoord.x,goalCoord.y);
                continue;
            }
            
            // 単純距離が最も低いcoordを採用
            distanceSq = fabs(startCoord.getDistanceSq(goalCoord));
            if (bestScore == -1 || distanceSq < bestScore) {
                bestScore = distanceSq;
                bestCoord = goalCoord;
                targetBuilding = building;
//                CCLOG("[]bestCoord(%f,%f),bestScore(%f)",bestCoord.x,bestCoord.y,bestScore);
            }
        }
    }
//    CCLOG("[findPointToGo]bestCoord(%f,%f),bestScore(%f)",bestCoord.x,bestCoord.y,bestScore);
    return bestCoord;
}

Vec2 Unit::findNearestWallGoalPoint()
{
    float bestScore = -1;
    float distanceSq;
    Vec2 nearestWallCoord;
    Vec2 startCoord = this->coord;
    Vec2 wallCoord;
    Building* building;
    
    auto targetCoords = this->getTargetCoords(Building::Walls);
    
    // 壁を走査して単純距離が最も近い壁を探す
    for (auto targetCoord: targetCoords) {
        building = this->tmx->buildingGrid[targetCoord.x][targetCoord.y];
        wallCoord = building->coord;
        distanceSq = fabs(startCoord.getDistanceSq(wallCoord));
        if (bestScore == -1 || distanceSq < bestScore) {
            bestScore = distanceSq;
            nearestWallCoord = wallCoord;
            targetBuilding = building;
        }
    }
    
    // 最も近い壁の周囲マスから最も近い場所をgoalPointとする
    Vec2 goalCoord;
    Vec2 goalCandidate;
    float bestGoalScore = -1;
    
    auto surroundCoords = this->getSurroundedCoords(nearestWallCoord);
    for (auto coord: surroundCoords) {
        goalCandidate = nearestWallCoord + coord;
        
        // 移動地点候補に建物が建っていればスキップ
        if (nullptr != tmx->buildingGrid.at(goalCandidate.x).at(goalCandidate.y)) {
//            CCLOG("There are some buildings at goalCoord(%f,%f)",goalCoord.x,goalCoord.y);
            continue;
        }
        distanceSq = fabs(startCoord.getDistanceSq(goalCandidate));
        if (bestGoalScore == -1 || distanceSq < bestGoalScore) {
            bestGoalScore = distanceSq;
            goalCoord = goalCandidate;
//            CCLOG("bestGoalScore(%f),goalCoord(%f,%f)",bestGoalScore,goalCoord.x,goalCoord.y);
        }
    }
    return goalCoord;
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
