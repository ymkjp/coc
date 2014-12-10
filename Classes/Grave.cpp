#include "Grave.h"

USING_NS_CC;

bool Grave::init(Tmx* _tmx, Vec2 _coord)
{
    tmx = _tmx;
    coord = _coord;
    
    
    auto graveSprite = CCSprite::createWithSpriteFrameName("stage/battle_effect/grave.png");
    graveSprite->setRotation(-90);
    this->addChild(graveSprite);
    
    return true;
}
