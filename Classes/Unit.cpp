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

bool Unit::init(Unit::__TYPE unitType)
{
    type = unitType;
//    SpriteFrameCache::getInstance();

    unitNode = Node::create();
    auto childNode = CSLoader::createNode("CocosProject/res/UnitBarbarianWalkEast.csb");
    actionTimeline = timeline::ActionTimelineCache::createAction("CocosProject/res/UnitBarbarianWalkEast.csb");
    childNode->runAction(actionTimeline);
    actionTimeline->gotoFrameAndPlay(0, true);
    unitNode->addChild(childNode,1 ,"childNode");
    
//    cocos2d::Node *node = CSLoader::getInstance()->createNodeFromProtocolBuffers("HogeScene.csb");
//    this->addChild(node);
    
    return true;
}

void Unit::update( float frame )
{
    //    CCLOG("だるい");
}


inline bool Unit::isNextCoord(float num)
{
    return (num == -1.0f || num == 0.0f || num == 1.0f);
}

Node* Unit::createAnimatedNode(Vec2 posDiff){
    return this->unitNode;
}

void Unit::attack()
{
    action = Attacking;
    CCLOG("Unit attattak%i",__LINE__);
    auto childNode = unitNode->getChildByName("childNode");
    childNode->stopAction(actionTimeline);
    unitNode->removeChild(childNode);

    auto newChildNode = CSLoader::createNode("CocosProject/res/UnitBarbarianAttackEast.csb");
    actionTimeline = timeline::ActionTimelineCache::createAction("CocosProject/res/UnitBarbarianAttackEast.csb");
    newChildNode->runAction(actionTimeline);
    actionTimeline->gotoFrameAndPlay(0, true);
    unitNode->addChild(newChildNode);
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


