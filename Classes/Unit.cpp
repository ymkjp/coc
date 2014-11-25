#include "Unit.h"

USING_NS_CC;

bool Unit::init(Unit::__TYPE unitType)
{
    _type = unitType;
    spriteFrameCache = SpriteFrameCache::getInstance();

    unitNode = CSLoader::createNode("CocosProject/res/UnitBarbarianAttackEast.csb");
    auto action = timeline::ActionTimelineCache::createAction("CocosProject/res/UnitBarbarianAttackEast.csb");
    unitNode->runAction(action);
    action->gotoFrameAndPlay(0, true);

//    cocos2d::Node *node = CSLoader::getInstance()->createNodeFromProtocolBuffers("HogeScene.csb");
//    this->addChild(node);
    
//    auto fileName = String::create("unit/");
//    switch (unitType) {
//        case Archer:
//            fileName->append("archer/walk/east/01.png");
//            break;
//        case Barbarian:
//            fileName->append("barbarian/walk/east/01.png");
//            break;
//        case Giant:
//            fileName->append("giant/walk/east/01.png");
//            break;
//        case Goblin:
//            fileName->append("giant/walk/east/01.png");
//            break;
//        case Wallbreaker:
//            fileName->append("giant/walk/east/01.png");
//            break;
//            
//        default:
//            break;
//    }
    
    
//    sprite = Sprite::createWithSpriteFrameName(fileName->getCString());
    
//    this->addAnimation();
    
    return true;
}
