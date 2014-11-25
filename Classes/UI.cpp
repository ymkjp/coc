#include "Unit.h"
#include "MapNavigator.h"

USING_NS_CC;

bool Unit::init(TMXTiledMap* tiledMap, Unit::__TYPE unitType)
{
    _tiledMap = tiledMap;
    _type = unitType;
    spriteFrameCache = SpriteFrameCache::getInstance();

    std::string fileName = {};
    switch (unitType) {
        case Archer:
            fileName = "unit/archer/53.0.png";
            break;
        case Barbarian:
            fileName = "unit/barbarian/770.0.png";
            break;
        case Giant:
            fileName = "unit/giant/1949.0.png";
            break;
        case Goblin:
            fileName = "unit/giant/1949.0.png";
            break;
        case Wallbreaker:
            fileName = "unit/giant/1949.0.png";
            break;
            
        default:
            break;
    }
    
    
    sprite = Sprite::createWithSpriteFrameName(fileName.c_str());
    
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
    sprite->setPosition(this->convertToTile(tileCoord));
    
    
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

