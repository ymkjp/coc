#include "Building.h"

USING_NS_CC;

bool Building::init(Tmx* _tmx, Vec2 _coord)
{
    tmx = _tmx;
//    cache = SpriteFrameCache::getInstance();
    status = Alive;
    type = this->getType();
    coord = _coord;

    hitpoints = getFullHitPoints();
    
    this->startScan();
    this->initNode();
    
    // 子で各々行う初期化
    this->initOwn();
    
    if (type != Wall) {
        tmx->incrementBuildingCount();
    }
    
    return true;
}

inline bool Building::isTargetLayer(std::string name, Vec2 coord)
{
    return (isInMapRange(coord) && 0 != tmx->tiledMap->getLayer(name.c_str())->getTileGIDAt(coord));
}

void Building::initNode()
{
    switch (type) {
        case Wall:
        {
            Node* buildingNode;
            auto westCoord = coord + Vec2(-1,0);
            auto northCoord = coord + Vec2(0,-1);
            if (this->isTargetLayer("Wall", westCoord) && this->isTargetLayer("Wall", northCoord)) {
                // "\/"
                buildingNode = CCSprite::createWithSpriteFrameName("stage/wall/1027.0.png");
            } else if (this->isTargetLayer("Wall", westCoord)) {
                // "\"
                buildingNode = CCSprite::createWithSpriteFrameName("stage/wall/1029.0.png");
            } else if (this->isTargetLayer("Wall", northCoord)) {
                // "/"
                buildingNode = CCSprite::createWithSpriteFrameName("stage/wall/1028.0.png");
                buildingNode->setPositionX(buildingNode->getPosition().x + buildingNode->getContentSize().width * 0.2);
            } else {
                // "."
                buildingNode = CCSprite::createWithSpriteFrameName("stage/wall/1030.0.png");
                buildingNode->setPositionX(buildingNode->getPosition().x + buildingNode->getContentSize().width * 0.2);
            }
            this->addChild(buildingNode,BuildingOrder,BuildingNodeTag);
            break;
        }
        case ArcherTower:
        {
            auto buildingNode = CCSprite::createWithSpriteFrameName("stage/archer_tower/1036.0.png");
            this->initArchersOnTower();
            this->addChild(buildingNode,BuildingOrder,BuildingNodeTag);
            break;
        }
        case TrenchMortar:
        {
            auto buildingNode = tmx->csLoader->createNode("res/TrenchMortar.csb");
            auto motionAction = tmx->actionTimelineCache->createAction("res/TrenchMortar.csb");
            this->addChild(buildingNode,BuildingOrder,BuildingNodeTag);
            motionAction->setTag(BuildingActionTag);
            buildingNode->runAction(motionAction);
            motionAction->gotoFrameAndPause(1);
            break;
        }
        case TownHall:
        {
            auto buildingNode = tmx->csLoader->createNode("res/TownHall.csb");
            this->addChild(buildingNode,BuildingOrder,BuildingNodeTag);
            break;
        }
        case Canon:
        {
            auto buildingNode = tmx->csLoader->createNode("res/Canon.csb");
            auto motionAction = tmx->actionTimelineCache->createAction("res/Canon.csb");
            this->addChild(buildingNode,BuildingOrder,BuildingNodeTag);
            motionAction->setTag(BuildingActionTag);
            buildingNode->runAction(motionAction);
            motionAction->retain();
            break;
        }
        case GoldBank:
        {
            auto buildingNode = tmx->csLoader->createNode("res/GoldBank.csb");
            auto motionAction = tmx->actionTimelineCache->createAction("res/GoldBank.csb");
            this->addChild(buildingNode,BuildingOrder,BuildingNodeTag);
            buildingNode->runAction(motionAction);
            motionAction->setTag(BuildingActionTag);
            motionAction->gotoFrameAndPause(6);
            buildingNode->setScale(0.7);
            break;
        }
        case ElixerTank:
        {
            auto buildingNode = tmx->csLoader->createNode("res/ElixerTank.csb");
            auto motionAction = tmx->actionTimelineCache->createAction("res/ElixerTank.csb");
            this->addChild(buildingNode,BuildingOrder,BuildingNodeTag);
            motionAction->setTag(BuildingActionTag);
            buildingNode->runAction(motionAction);
            buildingNode->setScale(0.7);
            buildingNode->setPositionY(10);
            motionAction->gotoFrameAndPause(6);
            break;
        }
        default:
            CCLOG("UNKNOWN buildingNode type");
            break;
    }
}

void Building::attacked(float damage)
{
    if (status == Died) {
        return;
    }

    hitpoints -= damage;
    hitpointPercentage = hitpoints / getFullHitPoints() * 100;
    
//    CCLOG("prevHitpoints(%f),damage(%f),hitpoints(%f)/getFullHitPoints(%f)*100=hitpointPercentage(%f)",prevHitpoints,damage,hitpoints,getFullHitPoints(),hitpointPercentage);
    
    this->damagedEffect();
    this->updateLifeGage();
    this->increaseResourceScore(damage);
    
    if (hitpoints <= 0) {
        hitpoints = 0;
        this->broken();
    }
}

inline void Building::updateLifeGage()
{
    auto parent = getParent();
    if (parent && !lifeGageNode) {
        // ライフゲージ初期化
        // 0〜100フレームまであって徐々に減らしていくことで操作できる
        lifeGageNode = tmx->csLoader->createNode("res/LifeGageBuilding.csb");
        lifeGageNode->setScale(0.5);
        lifeGageAction = tmx->actionTimelineCache->createAction("res/LifeGageBuilding.csb");
        lifeGageNode->runAction(lifeGageAction);
        parent->addChild(lifeGageNode,LifeGageZOrder);
    }
    if (parent && lifeGageNode && 0 <= hitpointPercentage && hitpointPercentage <= 100) {
//        CCLOG("BuildingLifeGage::percentage(%i)",percentage);
        // @fixme 建物の高さに応じた lifeGage pos セット
        lifeGageNode->setVisible(true);
        lifeGageNode->setPosition(getPosition().x, getPosition().y + 60);
        
        if (lifeGageAction) {
            lifeGageAction->gotoFrameAndPause((int)hitpointPercentage);
        }
        this->scheduleOnce(schedule_selector(Building::hideLifeGage), 3.0);
    }
}

void Building::broken()
{
    status = Died;
    
    tmx->eraseBuilding(this);
    
    this->stopAllActions();
    this->unscheduleAllCallbacks();
//    this->removeAllChildrenWithCleanup(true);
    
    for (auto child: this->getChildren()) {
        child->stopAllActions();
        child->setVisible(false);
    }
    
    auto parent = getParent();
    if (!parent) {
        return;
    }
    if (lifeGageNode) {
        lifeGageNode->setVisible(false);
//        parent->removeChild(lifeGageNode);
    }
    if (targetMarkNode) {
        targetMarkNode->setVisible(false);
//        parent->removeChild(targetMarkNode);
    }
    
    this->brokenEffect();
    this->addWrack();
    
    if (type != Wall) {
        tmx->decrementBuildingCount();
    }
}

void Building::brokenEffect()
{
    auto node = tmx->csLoader->createNode("res/Broken.csb");
    auto children = node->getChildren();
    for (auto shard: children) {
        //時間, 移動距離, ジャンプの最高点, ジャンプの回数
        auto jump = JumpTo::create(1, Vec2(rand() % 180 - 90, rand() % 180 - 90), rand() % 100, 2);
        auto disapear = FadeOut::create(2.5);
        FiniteTimeAction* invisible = CallFunc::create([=]() {
            if (shard) {
                shard->setVisible(false);
            }
        });
        shard->runAction(Sequence::create(Spawn::create(jump,disapear, NULL),invisible,NULL));
    }
    this->addChild(node,BuildingExplosionShardsOrder);
}

void Building::addWrack()
{
    if (type == Wall) {
        // 残骸の代わりに音だけ表示
        tmx->playSE("explosion");
        return;
    }
    
    Node* wrackNode;
    switch (type) {
        case TownHall:
        {
            wrackNode = CCSprite::createWithSpriteFrameName("stage/town_hall/broken.png");
            break;
        }
        case GoldBank:
        {
            wrackNode = CCSprite::createWithSpriteFrameName("stage/gold_bank/broken.png");
            wrackNode->setRotation(-90);
            break;
        }
        case ElixerTank:
        {
            auto glass = CCSprite::createWithSpriteFrameName("stage/elixer_tank/broken_glass.png");
            wrackNode = CCSprite::createWithSpriteFrameName("stage/elixer_tank/broken_base.png");
            wrackNode->addChild(glass);
            wrackNode->setPosition(wrackNode->getPosition().x +18, wrackNode->getPosition().y +18);
            break;
        }
        default:
            wrackNode = CCSprite::createWithSpriteFrameName("stage/battle_effect/defense_broken.png");
            break;
    }
//    auto prevPos = buildingNode->getPosition();
//    wrackNode->setPosition(prevPos);
    
    this->addChild(wrackNode);
    
    
    // 残骸はユニットより下に表示されるべき
    this->setLocalZOrder(coord.x + coord.y - 12); // タウンホールの残骸のうえをユニットが歩くように
}

BuildingSpace Building::getSpace()
{
    return typeSpace.at(type);
}

void Building::putTargetMark()
{
    auto parent = getParent();
    if (!parent) {
        return;
    }
    if (!targetMarkNode) {
        // ターゲットマークの初期化
        targetMarkNode = tmx->csLoader->createNode("res/TargetMarkerNode.csb");
        targetMarkNode->setScale(0.5);
        targetMarkAction = tmx->actionTimelineCache->createAction("res/TargetMarkerNode.csb");
        targetMarkNode->setPosition(getPosition().x,getPosition().y + 5);
        targetMarkNode->runAction(targetMarkAction);
        parent->addChild(targetMarkNode,TargetMarkerZOrder);
    }
    if (targetMarkAction) {
        targetMarkAction->gotoFrameAndPlay(0,false);
    }
}


const BuildingTypesByCategory Building::typesByCategory =
{
    {Resources, {ElixerTank, GoldBank}},
    {Defenses, {Canon, TrenchMortar, ArcherTower}},
    {Walls, {Wall}},
    {Melee, {TownHall, ElixerTank, GoldBank, Canon, TrenchMortar, ArcherTower, /** Wall */}},
    /** For Debug */
    {TownHalls, {TownHall,}},
};

const BuildingVec2sBySpace Building::coordsSurround = {
    {Small, /** count: 8 */ {
        Vec2(-1,-1), Vec2(-1,0), Vec2(-1,1),
        Vec2(0,-1), Vec2(0,1),
        Vec2(1,-1), Vec2(1,0), Vec2(1,1),
    }},
    {Regular, /** count: 12 */ {
        Vec2(-1,-1), Vec2(-1,0), Vec2(-1,1),
        Vec2(0,0), Vec2(0,1),
        Vec2(1,-1), Vec2(1,0), Vec2(1,1),
    }},
    {Large,  /** count: 16 */ {
        Vec2(-2,-2), Vec2(-2,-1), Vec2(-2,0), Vec2(-2,1), Vec2(-2,2),
        Vec2(-1,0), Vec2(-1,2),
        Vec2(0,0), Vec2(0,2),
        Vec2(1,0), Vec2(1,2),
        Vec2(2,-2), Vec2(2,-1), Vec2(2,0), Vec2(2,1), Vec2(2,2),
    }},
};
