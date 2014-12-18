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
    
    tmx->playSE(unitSmallCaseNameByType.at(type) + "_deploy");
    
    hitpoints = this->getFullHitPoints();
    
    attackSpeed = attackSpeedByType.at(type);
    
    // ユニットの影
    auto shadowSprite = CCSprite::createWithSpriteFrameName("unit/shadow/0.0.png");
    this->addChild(shadowSprite,ShadowOrder,ShadowTag);
    shadowSprite->setPositionY(shadowPosYByType.at(type));
    shadowSprite->setScale(1.8);
    
    // 歩きのアクション
    auto motionNode = this->getActingNode();
    auto motionAction = this->getActionTimeline();
    motionNode->runAction(motionAction);
    motionAction->gotoFrameAndPlay(0, true);
    this->addChild(motionNode, MotionOrder, MotionTag);
    
//    CCLOG("[a]this->getChildrenCount(%lu)",this->getChildrenCount());

    // ライフゲージ 0〜100フレームまであって徐々に減らしていくことで操作できる
    auto lifeGageNode = tmx->csLoader->createNode("res/LifeGageUnit.csb");
    auto lifeGageAction = tmx->actionTimelineCache->createAction("res/LifeGageUnit.csb");
    lifeGageNode->runAction(lifeGageAction);
    lifeGageNode->setScale(0.5);
    lifeGageAction->gotoFrameAndPause(100);
    lifeGageNode->setPositionY(40);
    lifeGageNode->setVisible(false);
    this->addChild(lifeGageNode,LifeGageOrder,LifeGageTag);
    lifeGageAction->setTag(LifeGageActionTag);
    
    // デプロイされた場所で建物の射程距離
    this->pushTobuildingAttackRange(coord);
    
    this->scheduleOnce(schedule_selector(Unit::play), SOON);

    return true;
}

void Unit::play(float frame)
{
    CCLOG("[Unit::play] type(%i),frame(%f)",type,frame);
    if (status == Died) {
        return;
    }
    action = Walking;
    if (tmx->noBuildings()) {
        this->finishBattle();
        return;
    }
    auto startCoord = this->coord;
    auto goalCoord = this->findPointToGo();
    auto path = tmx->navigate(startCoord, goalCoord);
//    CCLOG("startCoord(%f,%f),goalCoord(%f,%f),path.size(%lu)",
//          startCoord.x,startCoord.y,
//          goalCoord.x,goalCoord.y,
//          path.size());
    
    // 経路を閾値内で見つけられなかった場合
    if (!path.empty() && path.top() == ERROR_COORD) {
        // 建物の近くの壁を攻撃する
        CCLOG("Wall condition!");
        goalCoord = findNearestWallGoalCoord();
        if (goalCoord == ERROR_COORD) {
            // エラーケース
            CCLOG("[ERROR_COORD] Wall not found!");
            this->unschedule(schedule_selector(Unit::play));
            this->scheduleOnce(schedule_selector(Unit::play), WAITING_SEC);
            return;
        }
        path = tmx->navigate(startCoord, goalCoord);
        
        CCLOG("[Wall]goalCoord(%f,%f),path.size(%lu)",goalCoord.x,goalCoord.y,path.size());
    }
    if (!path.empty() && path.top() == ERROR_COORD) {
        CCLOG("NOT FOUND");
        this->setScale(2);
        this->unschedule(schedule_selector(Unit::play));
        this->scheduleOnce(schedule_selector(Unit::play), WAITING_SEC);
        return;
    }
    
    // 初回の場合は施設上にターゲットマークを描写
    if (!alreadyMarked) {
        this->putTargetMark();
    }
    
    // アーチャーは移動せずに攻撃を始めることがあるため、ここで向きを変えさせる
    this->updateDirection();
    
    Vec2 nextCoord;
    Vec2 prevCoord = startCoord;
    Vec2 directionPoint;
    Vector<FiniteTimeAction*> arrayOfactions;
    MoveTo* moveAction;
    
    // 道のりがあれば移動 (なければ攻撃開始)
    while (!path.empty()) {
        nextCoord = path.top();
        
        directionPoint = tmx->convertToRealPos(nextCoord);
        if (path.size() == 1) {
            directionPoint += Vec2(rand() % 20 - 10, rand() % 20 - 10);
        }
        moveAction = MoveTo::create(movementSpeedByType.at(type), directionPoint);
        
        // 向き先に応じてアニメーションを切り替え
        FiniteTimeAction* transferring = CallFunc::create([=]() {
            CCLOG("[CallFunc] prevCoord(%f,%f),nextCoord(%f,%f)",prevCoord.x,prevCoord.y,nextCoord.x,nextCoord.y);
            if (status == Alive) {
                
                // 移動中に目標の建物が壊れていたら探しなおす
                if (targetBuilding->status == Building::Died)
                {
                    this->stopActionByTag(PlayingSequence);
                    this->scheduleOnce(schedule_selector(Unit::play), SOON);
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
        arrayOfactions.pushBack(transferring);
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
    
    auto ghostNode = tmx->csLoader->createNode("res/Ghost.csb");
    auto ghostAction = tmx->actionTimelineCache->createAction("res/Ghost.csb");
    ghostNode->setScale(1.4);
    ghostNode->runAction(ghostAction);
    ghostAction->gotoFrameAndPlay(0, false);
    
    this->getChildByTag(MotionTag)->setVisible(false);
    this->getChildByTag(LifeGageTag)->setVisible(false);
    this->getChildByTag(ShadowTag)->setVisible(false);

    this->addChild(ghostNode, GhostOrder);
    this->addGrave();
    
    tmx->units.eraseObject(this);
}

void Unit::finishBattle()
{
    finished();
    tmx->showBattleResult();
}

void Unit::finished()
{
    if (status == Died) {
        return;
    }
    status = Died;
    this->unscheduleAllCallbacks();
    this->stopAllActions();
    
    auto elixerNode = tmx->csLoader->createNode("res/ElixerBubble.csb");
    auto elixerAction = tmx->actionTimelineCache->createAction("res/ElixerBubble.csb");
    elixerNode->runAction(elixerAction);
    elixerAction->gotoFrameAndPlay(0,false);
    
    auto prevNode = this->getChildByTag(MotionTag);
    this->removeChildByTag(LifeGageTag);
    this->removeChildByTag(ShadowTag);
    this->removeChild(prevNode);
    this->addChild(elixerNode, GhostOrder);
}

void Unit::addGrave()
{
    // GraveGrid ですでにお墓が建っていないかチェック
    auto parent = getParent();
    if (parent && !tmx->graveGrid[coord.x][coord.y])
    {
        auto grave = Grave::create(tmx, coord);
        grave->setPosition(tmx->convertToRealPos(coord));
        parent->addChild(grave,coord.x + coord.y - 1); // Unit より後ろにするために -1
        tmx->graveGrid[coord.x][coord.y] = grave;
    }
}

void Unit::startAttacking()
{
    CCLOG("startAttacking!");
    this->scheduleOnce(schedule_selector(Unit::attack), SOON);
}


void Unit::attack(float frame)
{
    CCLOG("attack");
    if (status == Alive && targetBuilding && targetBuilding->status == Building::Alive) {
        this->action = Attacking;
        this->playStartAttackingVoice();
        
        // 攻撃動作アニメーション
        auto prevMotionNode = this->getChildByTag(MotionTag);
        auto nextMotionNode = this->getActingNode();
        auto action = this->getActionTimeline();
        if (nextMotionNode) {
            nextMotionNode->runAction(action);
        }
        if (action) {
            action->gotoFrameAndPlay(0, false);
        }
        if (prevMotionNode) {
            this->removeChild(prevMotionNode);
        }
        if (nextMotionNode) {
            this->addChild(nextMotionNode, MotionOrder, MotionTag);
        }
        
        this->shoot();
    }
    if (targetBuilding->status == Building::Died) {
        // 建物が破壊されると play に戻って新たな探索を始める
//        CCLOG("Unit::attack target died!");
        action = Walking;
        this->updateMotionNode();
        this->scheduleOnce(schedule_selector(Unit::play), SOON);
        return;
    } else if (targetBuilding->type == Wall) {
        // Wall の場合は攻撃のたびに play に戻って新たな探索を始める
        this->scheduleOnce(schedule_selector(Unit::play), attackSpeed);
        return;
    } else {
        // 攻撃速度で攻撃を続ける
        CCLOG("keep punching!");
        this->scheduleOnce(schedule_selector(Unit::play), attackSpeed);
        return;
    }
}

void Unit::update( float frame )
{
    // nothing by frame
}

void Unit::shoot()
{
    targetBuilding->attacked(damagePerAttack);
}

inline void Unit::updateMotionNode()
{
    if (status == Alive) {
        auto prevMotionNode = this->getChildByTag(MotionTag);
        auto nextMotionNode = this->getActingNode();
        auto action = this->getActionTimeline();
        if (nextMotionNode) {
            nextMotionNode->runAction(action);
        }
        if (action) {
            action->gotoFrameAndPlay(0, true);
        }
        if (prevMotionNode) {
            this->removeChild(prevMotionNode);
        }
        if (nextMotionNode) {
            this->addChild(nextMotionNode, MotionOrder, MotionTag);
        }
    }
}

inline void Unit::updateLifeGage()
{
    auto node = this->getChildByTag(LifeGageTag);
    if (!node) {return;}
    auto action = dynamic_cast<timeline::ActionTimeline*>(node->getActionByTag(LifeGageActionTag));
    
    int percentage = hitpoints / getFullHitPoints() * 100;
    if (0 <= percentage && action) {
//        CCLOG("percentage%i",percentage);
        node->setVisible(true);
        action->gotoFrameAndPause(percentage);
        this->scheduleOnce(schedule_selector(Unit::hideLifeGage), 3.0);
    }
}

inline bool Unit::isNextCoord(float num)
{
    return (num == -1.0f || num == 0.0f || num == 1.0f);
}

void Unit::updateDirection()
{
    if (!targetBuilding || targetBuilding->Died) {
        return;
    }
    
    float comassDegreeGoal = tmx->calcCompassDegree(coord, targetBuilding->coord) + 45;
    if (360 < comassDegreeGoal) {
        comassDegreeGoal -= 360;
    }
    compass = tmx->convertToCompass(comassDegreeGoal);
    
    this->updateMotionNode();
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
    Building* building = tmx->buildingGrid[targetCoord.x][targetCoord.y];
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
    // 1. 直近の建物を見つける
    // ユニットタイプに応じた攻撃ターゲットのカテゴリを取得
    std::vector<Vec2> targetCoords = {};
    BuildingCategory targetCategory = attackType.at(type);
    
    // カテゴリに対応したcoordを bmx->buildingCoords から取得
    targetCoords = this->getTargetCoords(targetCategory);
    if (targetCoords.empty()) {
        // 好みの標的がなければ Melee mode
        targetCoords = this->getTargetCoords(Melee);
    }
    
    // 1. 建物を走査して単純距離が最も近い建物を探す
    float bestScore = -1;
    float distanceSq;
    Vec2 nearestCoord;
    Vec2 startCoord = this->coord;
    Vec2 buildingCoord;
    Building* building;
    
    for (auto targetCoord: targetCoords) {
        building = tmx->buildingGrid[targetCoord.x][targetCoord.y];
        buildingCoord = building->coord;
        distanceSq = fabs(startCoord.getDistanceSq(buildingCoord));
        if (bestScore == -1 || distanceSq < bestScore) {
            bestScore = distanceSq;
            nearestCoord = buildingCoord;
//            targetBuilding = building;
        }
    }
    
    
    // 2. ユニットの現地点と直近の建物の途中にある壁をリストアップする
    // ax + by + c = 0
    float a = nearestCoord.y - startCoord.y;
    float b = nearestCoord.x - startCoord.x;
    float c = startCoord.y * (nearestCoord.x - startCoord.x) - startCoord.x * (nearestCoord.y - startCoord.y);

//    CCLOG("startCoord(%f,%f),nearestCoord(%f,%f),abc(%f,%f,%f),",
//          startCoord.x,startCoord.y,nearestCoord.x,nearestCoord.y,a,b,c);
    
//    if (a == 0 || b == 0) {
//        CCLOG("ZERO DIVISION");
//    }

    Vector<Building*> targetWalls = {};
//    if (b != 0) {x
        bool greaterX = (startCoord.x < nearestCoord.x);
        for (int x = startCoord.x; (greaterX) ? x <= nearestCoord.x : x >= nearestCoord.x; (greaterX) ? ++x : --x) {
            int y = abs((-a * x - c) / b);
//            CCLOG("[X]targetWall(%i,%i)",x,y);
            if (isInMapRange(Vec2(x,y))
                && tmx->buildingGrid[x][y] != nullptr
                && tmx->buildingGrid[x][y]->type == Wall) {
                
                targetWalls.pushBack(tmx->buildingGrid[x][y]);
                break;
            }
        }
//    }
//    if (a != 0) {
        bool greaterY = (startCoord.y < nearestCoord.y);
        for (int y = startCoord.y; (greaterY) ? y <= nearestCoord.y : y >= nearestCoord.y; (greaterY) ? ++y : --y) {
            int x = abs((b * y - c)/ a);
//            CCLOG("[Y]targetWall(%i,%i)",x,y);
            if (isInMapRange(Vec2(x,y))
                && tmx->buildingGrid[x][y] != nullptr
                && tmx->buildingGrid[x][y]->type == Wall) {
                
                targetWalls.pushBack(tmx->buildingGrid[x][y]);
                break;
            }
        }
//    }

//    CCLOG("targetWalls.size(%lu)",targetWalls.size());


    // 3. リストアップされた壁の中からユニットに最も近い壁をターゲットとする
    // 予期しないケースで size 0 の場合は、エラー値 ERROR_COORD を返す
    float bestWallScore = -1;
    float distanceUnitWall;
    Building* targetWall;
    if (targetWalls.size() > 0) {
        for (auto candidateWall: targetWalls) {
            distanceUnitWall = fabs(startCoord.getDistanceSq(candidateWall->coord));
            if (bestWallScore == -1 || distanceUnitWall < bestWallScore) {
                bestWallScore = distanceUnitWall;
                targetWall = candidateWall;
                //            CCLOG("bestWallScore(%f),candidateWall->coord(%f,%f)",
                //                  bestWallScore,candidateWall->coord.x,candidateWall->coord.y);
            }
            
        }
    } else {
        return ERROR_COORD;
    }
    
    targetBuilding = targetWall;
    Vec2 targetWallCoord = targetWall->coord;
    
//    CCLOG("targetWallCoord(%f,%f)",
//          targetWallCoord.x,targetWallCoord.y);
    
    // 4. ターゲットの壁の周囲マスから最も近い場所をgoalCoordとする
    Vec2 goalCoord;
    Vec2 goalCandidate;
    float bestGoalScore = -1;
    
    auto surroundCoords = this->getSurroundedCoords(targetWallCoord);
    for (auto coord: surroundCoords) {
        goalCandidate = targetWallCoord + coord;
        
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
    return tmx->csLoader->createNode(this->createFilename().getCString());
}

timeline::ActionTimeline* Unit::getActionTimeline()
{
    return tmx->actionTimelineCache->createAction(this->createFilename().getCString());
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
    
    // 東・西・東・南西・北東・北西
    // @note 真北と真南は西に寄せているので直前の compass に依存させたければここ以前で compass を上書きする必要あり
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
    } else if (this->compass == North) {
        fileName.append("NorthWest");
    } else if (this->compass == South) {
        fileName.append("SouthWest");
    } else {
        CCLOG("[default] Unit::getActionTimeline unexpected direction:%i",this->compass);
        fileName.append("NorthWest");
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

void Unit::playDeathVoice()
{
    tmx->playSE(unitSmallCaseNameByType.at(type) + "_death");
};

void Unit::playStartAttackingVoice()
{
    tmx->playSE(unitSmallCaseNameByType.at(type) + "_attack_hit");
}

void Unit::testAdd(__String fileName, Vec2 pos)
{
    // 動作確認用
    auto node = tmx->csLoader->createNode(fileName.getCString());
    auto anim = tmx->actionTimelineCache->createAction(fileName.getCString());
    node->runAction(anim);
    anim->gotoFrameAndPlay(0, true);
    node->setScale(5);
    node->setPosition(pos);
    this->addChild(node);
}

