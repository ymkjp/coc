#include "Unit.h"
#include "BuildingDefense.h"
#include "Grave.h"

USING_NS_CC;

bool Unit::init(Tmx* _tmx, Vec2 _coord)
{
    tmx = _tmx;
    type = this->getUnitType();
    coord = _coord;
    damagePerAttack = damagePerAttackByType.at(type);
    status = Alive;
    action = Walking;
    compass = NorthEast;
    
    audioManager = AudioManager::create();
   
    audioManager->playSE("unit_" + unitSmallCaseNameByType.at(type) + "_deploy");
    
    hitpoints = this->getFullHitPoints();
    
    attackSpeed = attackSpeedByType.at(type);
    
    actionTimelineCache = timeline::ActionTimelineCache::getInstance();
    
    // ユニットの影
    auto shadowSprite = CCSprite::createWithSpriteFrameName("unit/shadow/0.0.png");
    this->addChild(shadowSprite,ShadowOrder,ShadowTag);
    // @todo ゴブリンとジャイアントは位置調整
    shadowSprite->setPositionY(-14);
    shadowSprite->setScale(1.8);
    
    // 歩きのアクション
    motionNode = this->getActingNode();
    motionAction = this->getActionTimeline();
    motionNode->runAction(motionAction);
    motionAction->gotoFrameAndPlay(0, true);
    this->addChild(motionNode, MotionOrder, MotionTag);
    
//    CCLOG("[a]this->getChildrenCount(%lu)",this->getChildrenCount());

    // ライフゲージ 0〜100フレームまであって徐々に減らしていくことで操作できる
    lifeGageNode = CSLoader::createNode("res/LifeGageUnit.csb");
    lifeGageAction = actionTimelineCache->createAction("res/LifeGageUnit.csb");
    lifeGageNode->runAction(lifeGageAction);
    lifeGageAction->gotoFrameAndPause(100);
    lifeGageNode->setPositionY(40);
    lifeGageNode->setVisible(false);
    this->addChild(lifeGageNode,1,LifeGageTag);
    
    // デプロイされた場所で建物の射程距離
    this->pushTobuildingAttackRange(coord);
    
    this->play(1);
    return true;
}

void Unit::play(float frame)
{
    CCLOG("Unit::play frame[%f]",frame);
    if (tmx->noBuildings()) {
        // Finish battle
        this->unscheduleAllCallbacks();
        tmx->showBattleResult();
        return;
    }
    if (status == Died) {
        return;
    }
    auto startCoord = this->coord;
    auto goalCoord = this->findPointToGo();
    auto path = tmx->navigate(startCoord, goalCoord);
    //    CCLOG("Over steps?:%i",mapNavigator->isOverSteps);
    
    // 経路を閾値内で見つけられなかった場合
    if (!path.empty() && path.top() == Vec2(-1,-1)) {
        // 最寄りの壁を攻撃する
        // @todo 最寄りじゃなくて建物の近くの壁
        goalCoord = findNearestWallGoalCoord();
        //        CCLOG("[Wall]goalCoord(%f,%f)",goalCoord.x,goalCoord.y);
        path = tmx->navigate(startCoord, goalCoord);
    }
    if (!path.empty() && path.top() == Vec2(-1,-1)) {
        CCLOG("NOT FOUND");
    }
    
    // 初回の場合は施設上にターゲットマークを描写
    if (!alreadyMarked) {
        this->putTargetMark();
    }
    
    Vec2 nextCoord;
    Vec2 prevCoord = startCoord;
    Vec2 directionPoint;
    Vector<FiniteTimeAction*> arrayOfactions;
    MoveTo* moveAction;
    
    // 道のりがあれば移動 (なければ攻撃開始)
    while (!path.empty()) {
        nextCoord = path.top();
        directionPoint = tmx->convertToRealPos(nextCoord);
        moveAction = MoveTo::create(movementSpeedByType.at(type), directionPoint);
        
        // 向き先に応じてアニメーションを切り替え
        FiniteTimeAction* func = CallFunc::create([=]() {
            //            CCLOG("[CallFunc] prevCoord(%f,%f),nextCoord(%f,%f)",prevCoord.x,prevCoord.y,nextCoord.x,nextCoord.y);
            if (status == Alive) {
                
                // 移動中に目標の建物が壊れていたら探しなおす
                if (targetBuilding->status == Building::Died)
                {
                    this->stopActionByTag(PlayingSequence);
                    this->play(1);
                    return;
                }
                
                // Zオーダーの更新
                this->setLocalZOrder(nextCoord.x + nextCoord.y);
                
                // 向きをセットしてモーションを更新
                this->setCompass(prevCoord, nextCoord);
                this->updateMotionNode();
                
                // 防衛施設の射程に入ったことを通知
                this->pushTobuildingAttackRange(nextCoord);
                
                // 自身のcoordを更新
                this->coord = nextCoord;
            }
        });
        arrayOfactions.pushBack(func);
        
        arrayOfactions.pushBack(moveAction);
        path.pop();
        prevCoord = nextCoord;
    }
    FiniteTimeAction* attack = CallFunc::create(CC_CALLBACK_0(Unit::startAttacking, this));
    arrayOfactions.pushBack(attack);
    auto seq = Sequence::create(arrayOfactions);
    seq->setTag(PlayingSequence);
    
    this->runAction(seq);
}

void Unit::pushTobuildingAttackRange(Vec2 coord)
{
    auto defenseBuildings = &tmx->buildingAttackRangeGrid.at(coord.x).at(coord.y);
    if (defenseBuildings->size() > 0) {
        for (auto defense: *defenseBuildings) {
            defense->targetUnits.pushBack(this);
        }
    }
}

void Unit::attacked(float damage)
{
    if (status == Died) {
        return;
    }
    if (hitpoints < damage) {
        hitpoints = 0;
        this->die();
    } else {
        this->hitpoints -= damage;
        this->updateLifeGage();
//        CCLOG(">> Unit::attacked[%i]::hitpoints%f,damage:%f",type,this->hitpoints,damage);
    }
}

void Unit::die()
{
    status = Died;
    this->unscheduleAllCallbacks();
    this->stopAllActions();
    
    this->playDeathVoice();
    
    auto prevNode = this->getChildByTag(MotionTag);
    auto ghostNode = CSLoader::createNode("res/Ghost.csb");
    auto ghostAction = actionTimelineCache->createAction("res/Ghost.csb");
    ghostNode->setScale(1.4);
    ghostNode->setPosition(prevNode->getPosition());
    ghostNode->runAction(ghostAction);
    ghostAction->gotoFrameAndPlay(0, false);
    this->removeChildByTag(LifeGageTag);
    this->removeChildByTag(ShadowTag);
    this->removeChild(prevNode);
    this->addChild(ghostNode, GhostOrder);
    this->addGrave();
}

void Unit::addGrave()
{
    // GraveGrid ですでにお墓が建っていないかチェック
    // @todo 建設可能coordか
    auto parent = getParent();
    if (parent && !tmx->graveGrid[coord.x][coord.y])
    {
        auto grave = Grave::create(tmx, coord);
        grave->setPosition(tmx->convertToRealPos(coord));
        parent->addChild(grave,coord.x + coord.y);
        tmx->graveGrid[coord.x][coord.y] = grave;
    }
}

void Unit::startAttacking()
{
    Unit::attack(1);
    this->schedule(schedule_selector(Unit::attack), attackSpeed);
}


void Unit::attack(float frame)
{
    if (status == Alive && this->targetBuilding && this->targetBuilding->status == Building::Alive) {
        this->action = Attacking;
        this->playStartAttackingVoice();
        
        // 攻撃動作アニメーション
        // @todo delay
        auto prevMotionNode = this->getChildByTag(MotionTag);
        auto nextMotionNode = this->getActingNode();
        auto action = this->getActionTimeline();
        nextMotionNode->runAction(action);
        action->gotoFrameAndPlay(0, false);
        this->removeChild(prevMotionNode);
        this->addChild(nextMotionNode, MotionOrder, MotionTag);
        
        this->shoot();
    }
    if (this->targetBuilding->status == Building::Died) {
//        CCLOG("Unit::attack target died!");
        action = Walking;
        this->updateMotionNode();
        this->scheduleOnce(schedule_selector(Unit::play), 0);
        this->unschedule(schedule_selector(Unit::attack));
    }
}

void Unit::shoot()
{
    this->targetBuilding->attacked(damagePerAttack);
}

inline void Unit::updateMotionNode()
{
    if (status == Alive) {
        auto prevMotionNode = this->getChildByTag(MotionTag);
        auto nextMotionNode = this->getActingNode();
        auto action = this->getActionTimeline();
        nextMotionNode->runAction(action);
        action->gotoFrameAndPlay(0, true);
        this->removeChild(prevMotionNode);
        this->addChild(nextMotionNode, MotionOrder, MotionTag);
        
        motionAction = action;
    }
}

inline void Unit::updateLifeGage()
{
    int percentage = hitpoints / getFullHitPoints() * 100;
    if (0 <= percentage) {
//        CCLOG("percentage%i",percentage);
        lifeGageNode->setVisible(true);
        lifeGageAction->gotoFrameAndPause(percentage);
        this->scheduleOnce(schedule_selector(Unit::hideLifeGage), 3.0);
    }
}

inline bool Unit::isNextCoord(float num)
{
    return (num == -1.0f || num == 0.0f || num == 1.0f);
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
        // 真南の場合は前回の向きに影響される
        if (currentCompass == NorthEast
            || currentCompass == East
            || currentCompass == SouthEast) {
            this->compass = SouthWest;
        } else if (currentCompass == NorthWest
                   || currentCompass == West
                   || currentCompass == SouthWest) {
            this->compass = SouthEast;
        }
    } else if (dx == -1 && dy == 0) {
        this->compass = SouthEast;
    } else if (dx == -1 && dy == 1) {
        this->compass = East;
    } else if (dx == 0 && dy == -1) {
        this->compass = SouthWest;
    } else if (dx == 0 && dy == 1) {
        this->compass = NorthEast;
    } else if (dx == 1 && dy == -1) {
        this->compass = West;
    } else if (dx == 1 && dy == 0) {
        this->compass = NorthWest;
    } else if (dx == 1 && dy == 1) {
        // 真北の場合は前回の向きに影響される
        if (currentCompass == NorthEast
            || currentCompass == East
            || currentCompass == SouthEast) {
            this->compass = NorthWest;
        } else if (currentCompass == NorthWest
                   || currentCompass == West
                   || currentCompass == SouthWest) {
            this->compass = NorthEast;
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
    BuildingCategory targetCategory = attackType.at(type);
    
    // カテゴリに対応したcoordを bmx->buildingCoords から取得
    targetCoords = this->getTargetCoords(targetCategory);
    if (targetCoords.empty()) {
        // 好みの標的がなければ Melee mode
        targetCoords = this->getTargetCoords(Melee);
    }
    
//    auto mapNavigator = MapNavigator::create(tmx);
    
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
            // @todo 判定を Wall に変更すべき
            if (!isInMapRange(goalCoord) || !tmx->isTravelable(goalCoord.x,goalCoord.y)) {
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

Vec2 Unit::findNearestWallGoalCoord()
{
    // @fixme 「直近の壁」になっているが「直近の建物との間にある壁」になるべき
    float bestScore = -1;
    float distanceSq;
    Vec2 nearestWallCoord;
    Vec2 startCoord = this->coord;
    Vec2 wallCoord;
    Building* building;
    
    auto targetCoords = this->getTargetCoords(Walls);
    
    // 壁を走査して単純距離が最も近い壁を探す
    for (auto targetCoord: targetCoords) {
        building = tmx->buildingGrid[targetCoord.x][targetCoord.y];
        wallCoord = building->coord;
        distanceSq = fabs(startCoord.getDistanceSq(wallCoord));
        if (bestScore == -1 || distanceSq < bestScore) {
            bestScore = distanceSq;
            nearestWallCoord = wallCoord;
            targetBuilding = building;
        }
    }
    
    // 最も近い壁の周囲マスから最も近い場所をgoalCoordとする
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

std::vector<Vec2> Unit::getTargetCoords(BuildingCategory category)
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

/**
 @example "res/UnitArcherWalkEast.csb"
 */
__String Unit::createFilename()
{
    __String fileName = "res/Unit";
    
    fileName.append(unitNameByType.at(this->type));
    
    switch (this->action) {
        case Unit::Attacking:
            fileName.append("Attack");
            break;
        case Unit::Walking:
            fileName.append("Walk");
            break;
        default:
            CCLOG("[default?!] Unit::getActionTimeline unexpected action");
            fileName.append("Walk");
            break;
    }
    
    // 東・西・東・南西・北東・北西 (真北と真南は直前の compass に依存するため渡ってこない想定)
    if (this->compass == East) {
        fileName.append("East");
    } else if (this->compass == West) {
        fileName.append("West");
    } else if (this->compass == NorthEast) {
        fileName.append("NorthEast");
    } else if (this->compass == NorthWest) {
        fileName.append("NorthWest");
    } else if (this->compass == SouthEast) {
        fileName.append("SouthEast");
    } else if (this->compass == SouthWest) {
        fileName.append("SouthWest");
    } else {
        CCLOG("[default] Unit::getActionTimeline unexpected direction:%i",this->compass);
        fileName.append("SouthEast");
    }
    
    fileName.append(".csb");
//    CCLOG("fileName:%s,this->action:%i,,this->compass:%i",fileName.getCString(),this->action,this->compass);
    return fileName.getCString();
}


const std::map<Vec2, Compass> Unit::compassByCoords =
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

void Unit::putTargetMark()
{
    // マーク済みのフラグを立てる
    alreadyMarked = true;
    if (targetBuilding) {
        targetBuilding->putTargetMark();
    }
}

void Unit::testAdd(__String fileName, Vec2 pos)
{
    // 動作確認用
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
    // nothing by frame
}
