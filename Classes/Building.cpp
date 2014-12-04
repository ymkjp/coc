#include "Building.h"

USING_NS_CC;


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

bool Building::init(Tmx* _tmx, Vec2 _coord)
{
    tmx = _tmx;
//    cache = SpriteFrameCache::getInstance();
    status = Alive;
    type = this->getType();
    coord = _coord;

    this->virtualInit();
    this->initNode();
    
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
            buildingNode = CSLoader::createNode("CocosProject_01/res/TrenchMortar.csb");
            auto action = timeline::ActionTimelineCache::createAction("CocosProject_01/res/TrenchMortar.csb");
            buildingNode->runAction(action);
            action->setTimeSpeed(0.1);
            action->gotoFrameAndPlay(0, true);
            break;
        }
        case TownHall:
        {
            buildingNode = CSLoader::createNode("CocosProject_01/res/TownHall.csb");
            break;
        }
        case Canon:
        {
            buildingNode = CSLoader::createNode("CocosProject_01/res/Canon.csb");
            auto action = timeline::ActionTimelineCache::createAction("CocosProject_01/res/Canon.csb");
            action->setTimeSpeed(0.01);
            buildingNode->runAction(action);
            action->gotoFrameAndPlay(0, true);
            break;
        }
        case GoldBank:
        {
            buildingNode = CSLoader::createNode("CocosProject_01/res/GoldBank.csb");
            buildingNode->setScale(0.75);
            break;
        }
        case ElixerTank:
        {
            buildingNode = CSLoader::createNode("CocosProject_01/res/ElixerTank.csb");
            buildingNode->setScale(0.75);
            break;
        }
        default:
            CCLOG("UNKNOWN buildingNode");
            buildingNode = Node::create();
            break;
    }
    this->addChild(buildingNode);
}

void Building::attacked(float damage)
{
    if (hitpoints < damage) {
        status = Died;
        tmx->eraseBuilding(this);
        this->broken();
    } else {
        hitpoints -= damage;
    }
    CCLOG("Building[%i]::hitpoints %f",type,hitpoints);
}

void Building::broken()
{
    this->unscheduleAllCallbacks();

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

