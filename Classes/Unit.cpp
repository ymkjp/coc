#include "Unit.h"

USING_NS_CC;

bool Unit::init(TMXTiledMap* tiledMap, Unit::__TYPE unitType)
{
    _tiledMap = tiledMap;
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

void Unit::addAnimation()
{
    Vector<SpriteFrame*> animFrames(3);
    char str[100] = {0};
    for(int i = 53; i < 56; i++)
    {
        sprintf(str, "unit/archer/%02d.0.png", i);
        SpriteFrame* frame = spriteFrameCache->getSpriteFrameByName(str);
        animFrames.pushBack(frame);
    }
    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.15f);
    sprite->runAction( RepeatForever::create( Animate::create(animation) ) );
}

void Unit::run(Vec2 pos, Vec2 tileCoord)
{
    unitNode->setPosition(pos);
    
//    auto goalCoord = Vec2(22,22); // @fixme
//    auto mapNavigator = MapNavigator::create(_tiledMap);
//    auto path = mapNavigator->navigate(tileCoord, goalCoord);
//    sprite->setPosition(pos);
//    if (path->empty()) {CCLOG("HMM... EMPTY PATH IS DETECTED");return;};
//    
//    Vector<FiniteTimeAction*> arrayOfactions;
//    while (!path->empty()) {
//        auto node = path->top();
//        auto directionPoint = this->convertToTile(node);
//        auto move = MoveTo::create(0.5, directionPoint);
//        arrayOfactions.pushBack(move);
//        path->pop();
//    }
//    auto seq = Sequence::create(arrayOfactions);
//    sprite->runAction(seq);
    
    return;
}

inline Vec2 Unit::convertToTile(Vec2 pos)
{
    return _tiledMap->getLayer("Domain")->getPositionAt(pos) + _tiledMap->getTileSize() * 0.5;
}

