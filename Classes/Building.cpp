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
    
    // ライフゲージ 0〜100フレームまであって徐々に減らしていくことで操作できる
    lifeGageNode = CSLoader::createNode("res/LifeGageBuilding.csb");
    lifeGageAction = actionTimelineCache->createAction("res/LifeGageBuilding.csb");
    lifeGageNode->runAction(lifeGageAction);
    lifeGageNode->setVisible(false);
    this->addChild(lifeGageNode,1,LifeGageTag); // GrobalZOrderが割り当てられる
    
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
            auto* filename = String::create("stage/wall/");
            if (this->isTargetLayer("Wall", westCoord) && this->isTargetLayer("Wall", northCoord)) {
                filename->append("1027.0.png");
            } else if (this->isTargetLayer("Wall", westCoord)) {
                filename->append("1029.0.png");
            } else if (this->isTargetLayer("Wall", northCoord)) {
                filename->append("1028.0.png");
            } else {
                filename->append("1030.0.png");
            }
            buildingNode = CCSprite::createWithSpriteFrameName(filename->getCString());
            break;
        }
        case ArcherTower:
        {
            buildingNode = CCSprite::createWithSpriteFrameName("stage/archer_tower/1036.0.png");
            break;
        }
        case TrenchMortar:
        {
            buildingNode = CSLoader::createNode("res/TrenchMortar.csb");
            motionAction = timeline::ActionTimelineCache::createAction("res/TrenchMortar.csb");
            buildingNode->runAction(motionAction);
            motionAction->setTimeSpeed(0.1);
            motionAction->gotoFrameAndPlay(0, true);
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
            ////            motionAction->setTimeSpeed(0.01);
            //            motionAction->gotoFrameAndPlay(0, true);
            break;
        }
        case GoldBank:
        {
            buildingNode = CSLoader::createNode("res/GoldBank.csb");
            buildingNode->setScale(0.75);
            break;
        }
        case ElixerTank:
        {
            buildingNode = CSLoader::createNode("res/ElixerTank.csb");
            buildingNode->setScale(0.75);
            break;
        }
        default:
            CCLOG("UNKNOWN buildingNode type");
            buildingNode = Node::create();
            break;
    }
    this->addChild(buildingNode);
}

void Building::attacked(float damage)
{
    if (status == Died) {
        return;
    }
    if (hitpoints < damage) {
        hitpoints = 0;
        tmx->eraseBuilding(this);
        this->broken();
    } else {
        hitpoints -= damage;
        this->updateLifeGage();
    }
    CCLOG("Building[%i]::hitpoints %f",type,hitpoints);
}

inline void Building::updateLifeGage()
{
    int percentage = hitpoints / getFullHitPoints() * 100;
    if (0 <= percentage) {
        CCLOG("BuildingLifeGage::percentage(%i)",percentage);
        lifeGageNode->setVisible(true);
        // @fixme init の時点で pos セットされてない
        // @fixme 建物の高さに応じた lifeGage pos セット
        lifeGageNode->setPositionY(buildingNode->getPosition().y + 60);
        lifeGageAction->gotoFrameAndPause(percentage);
        this->scheduleOnce(schedule_selector(Building::hideLifeGage), 3.0);
    }
}

void Building::broken()
{
    status = Died;
    this->unscheduleAllCallbacks();
    this->removeChildByTag(LifeGageTag);
    
    // @todo tmx のキャッシュを再構築
    auto prevPos = buildingNode->getPosition();
    buildingNode->removeFromParentAndCleanup(true);
    buildingNode = CCSprite::createWithSpriteFrameName("stage/field/271.0.png");
    buildingNode->setPosition(prevPos);
    this->addChild(buildingNode);
}

BuildingSpace Building::getSpace()
{
    return typeSpace.at(type);
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
