#include "Building.h"

USING_NS_CC;

bool Building::init(Tmx* _tmx, Vec2 _coord)
{
    tmx = _tmx;
//    cache = SpriteFrameCache::getInstance();
    status = Alive;
    type = this->getType();
    coord = _coord;
    actionTimelineCache = timeline::ActionTimelineCache::getInstance();

    this->virtualInit();
    this->initNode();
    
    // 子で各々行う初期化
    this->initOwn();
    
    if (type != Wall) {
        tmx->incrementBuildingCount();
    }
    
    return true;
}

bool Building::isBuildingRange(Vec2 targetCoord)
{
    if (targetCoord == this->coord) {
        return true;
    }
    Vec2 coordInRange;
    auto space = this->getSpace();
    auto diffCoords = Building::coordsBuildingSpace.at(space);
    for (auto diffCoord: diffCoords) {
        coordInRange = this->coord + diffCoord;
        if (coordInRange == targetCoord) {
            return true;
        }
    }
    return false;
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
            break;
        }
        case ArcherTower:
        {
            buildingNode = CCSprite::createWithSpriteFrameName("stage/archer_tower/1036.0.png");
            this->initArchersOnTower();
            break;
        }
        case TrenchMortar:
        {
            buildingNode = CSLoader::createNode("res/TrenchMortar.csb");
            motionAction = timeline::ActionTimelineCache::createAction("res/TrenchMortar.csb");
            buildingNode->runAction(motionAction);
            break;
        }
        case TownHall:
        {
            buildingNode = CSLoader::createNode("res/TownHall.csb");
            break;
        }
        case Canon:
        {
            buildingNode = CSLoader::createNode("res/Canon.csb");
            motionAction = timeline::ActionTimelineCache::createAction("res/Canon.csb");
            buildingNode->runAction(motionAction);
            break;
        }
        case GoldBank:
        {
            buildingNode = CSLoader::createNode("res/GoldBank.csb");
            motionAction = timeline::ActionTimelineCache::createAction("res/GoldBank.csb");
            buildingNode->runAction(motionAction);
            buildingNode->setScale(0.7);
            break;
        }
        case ElixerTank:
        {
            buildingNode = CSLoader::createNode("res/ElixerTank.csb");
            motionAction = timeline::ActionTimelineCache::createAction("res/ElixerTank.csb");
            buildingNode->runAction(motionAction);
            buildingNode->setScale(0.7);
            buildingNode->setPositionY(10);
            motionAction->gotoFrameAndPause(6);
            break;
        }
        default:
            CCLOG("UNKNOWN buildingNode type");
            buildingNode = Node::create();
            break;
    }
    this->addChild(buildingNode,BuildingOrder,BuildingTag);
}

void Building::attacked(float damage)
{
    if (status == Died) {
        return;
    }

    hitpoints -= damage;
    hitpointPercentage = hitpoints / getFullHitPoints() * 100;
    
    this->damagedEffect();
    this->updateLifeGage();
    this->increaseResourceScore(damage);
    
    if (hitpoints <= 0) {
        hitpoints = 0;
        this->broken();
    }
//    CCLOG("Building[%i]::hitpoints %f",type,hitpoints);
}

inline void Building::updateLifeGage()
{
    auto parent = getParent();
    if (parent && !lifeGageNode) {
        // ライフゲージ初期化
        // 0〜100フレームまであって徐々に減らしていくことで操作できる
        lifeGageNode = CSLoader::createNode("res/LifeGageBuilding.csb");
        lifeGageAction = actionTimelineCache->createAction("res/LifeGageBuilding.csb");
        lifeGageNode->runAction(lifeGageAction);
        getParent()->addChild(lifeGageNode,LifeGageZOrder);
    }
    if (parent && 0 <= hitpointPercentage) {
//        CCLOG("BuildingLifeGage::percentage(%i)",percentage);
        // @fixme init の時点で pos セットされてない
        // @fixme 建物の高さに応じた lifeGage pos セット
        lifeGageNode->setVisible(true);
        lifeGageNode->setPosition(getPosition().x, getPosition().y + 60);
        lifeGageAction->gotoFrameAndPause((int)hitpointPercentage);
        this->scheduleOnce(schedule_selector(Building::hideLifeGage), 3.0);
    }
}

void Building::broken()
{
    status = Died;
    
    // @todo tmx のキャッシュを再構築
    tmx->eraseBuilding(this);
    
    this->stopAllActions();
    this->unscheduleAllCallbacks();
    this->removeAllChildrenWithCleanup(true);
    
    auto parent = getParent();
    if (parent && lifeGageNode) {
        parent->removeChild(lifeGageNode);
    }
    if (parent && targetMarkNode) {
        parent->removeChild(targetMarkNode);
    }

    this->brokenEffect();
    this->addWrack();
    
    if (type != Wall) {
        tmx->decrementBuildingCount();
    }
}

void Building::brokenEffect()
{
    auto node = CSLoader::createNode("res/Broken.csb");
    auto children = node->getChildren();
    for (auto shard: children) {
        //時間, 移動距離, ジャンプの最高点, ジャンプの回数
        auto jump = JumpTo::create(1, Vec2(rand() % 180 - 90, rand() % 180 - 90), rand() % 100, 2);
        auto disapear = FadeOut::create(2.5);
        shard->runAction(Spawn::create(jump,disapear, NULL));
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
    if (!targetMarkAction) {
        // ターゲットマークの初期化
        targetMarkNode = CSLoader::createNode("res/TargetMarkerNode.csb");
        targetMarkAction = actionTimelineCache->createAction("res/TargetMarkerNode.csb");
        targetMarkNode->setPosition(getPosition().x,getPosition().y + 5);
        targetMarkNode->runAction(targetMarkAction);
        getParent()->addChild(targetMarkNode,TargetMarkerZOrder);
    }
    targetMarkAction->gotoFrameAndPlay(0,false);
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
        Vec2(-1,-1), Vec2(-1,0), Vec2(-1,1), Vec2(-1,2),
        Vec2(0,-1), Vec2(0,2),
        Vec2(1,-1), Vec2(1,2),
        Vec2(2,-1), Vec2(2,0), Vec2(2,1), Vec2(2,2),
    }},
    {Large,  /** count: 16 */ {
        Vec2(-1,-1), Vec2(-1,0), Vec2(-1,1), Vec2(-1,2), Vec2(-1,3),
        Vec2(0,-1), Vec2(0,3),
        Vec2(1,-1), Vec2(1,3),
        Vec2(2,-1), Vec2(2,3),
        Vec2(3,-1), Vec2(3,0), Vec2(3,1), Vec2(3,2), Vec2(3,3),
    }},
};

const BuildingVec2sBySpace Building::coordsBuildingSpace = {
    {Small, /** count: 1 */ {
        Vec2(0,0), // @fixme
    }},
    {Regular, /** count: 4 */ {
        Vec2(0,1), Vec2(1,1), Vec2(1,0)
    }},
    {Large,  /** count: 9 */ {
        Vec2(0,1), Vec2(1,1), Vec2(1,0),
        Vec2(0,2), Vec2(1,2), Vec2(2,0), Vec2(2,1), Vec2(2,2),
    }},
};
