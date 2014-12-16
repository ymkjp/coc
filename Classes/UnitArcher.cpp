#include "UnitArcher.h"

USING_NS_CC;

std::vector<Vec2> UnitArcher::getSurroundedCoords(Vec2 targetCoord)
{
    // 射程距離 3
    const std::vector<Vec2> surroundedCoords = {
        Vec2(-4,-4),Vec2(-4,-3),Vec2(-4,-2),Vec2(-4,-1),Vec2(-4,0),Vec2(-4,1),Vec2(-4,2),Vec2(-4,3),Vec2(-4,4),Vec2(-4,5), // 10
        Vec2(-3,-4),Vec2(-3,-3),Vec2(-3,-2),Vec2(-3,-1),Vec2(-3,0),Vec2(-3,1),Vec2(-3,2),Vec2(-3,3),Vec2(-3,4),Vec2(-3,5),
        Vec2(-2,-4),Vec2(-2,-3),Vec2(-2,-2),Vec2(-2,-1),Vec2(-2,0),Vec2(-2,1),Vec2(-2,2),Vec2(-2,3),Vec2(-2,4),Vec2(-2,5),
        Vec2(-1,-4),Vec2(-1,-3),Vec2(-1,-2),Vec2(-1,-1),Vec2(-1,0),Vec2(-1,1),Vec2(-1,2),Vec2(-1,3),Vec2(-1,4),Vec2(-1,5),
        Vec2(0,-4),Vec2(0,-3),Vec2(0,-2),Vec2(0,-1),Vec2(0,0),Vec2(0,2),Vec2(0,3),Vec2(0,4),Vec2(0,5),
        Vec2(1,-4),Vec2(1,-3),Vec2(1,-2),Vec2(1,-1),Vec2(1,0),Vec2(1,2),Vec2(1,3),Vec2(1,4),Vec2(1,5),
        Vec2(2,-4),Vec2(2,-3),Vec2(2,-2),Vec2(2,-1),Vec2(2,0),Vec2(2,2),Vec2(2,3),Vec2(2,4),Vec2(2,5),
        Vec2(3,-4),Vec2(3,-3),Vec2(3,-2),Vec2(3,-1),Vec2(3,0),Vec2(3,2),Vec2(3,3),Vec2(3,4),Vec2(3,5),
        Vec2(4,-4),Vec2(4,-3),Vec2(4,-2),Vec2(4,-1),Vec2(4,0),Vec2(4,2),Vec2(4,3),Vec2(4,4),Vec2(4,5),
        Vec2(5,-4),Vec2(5,-3),Vec2(5,-2),Vec2(5,-1),Vec2(5,0),Vec2(5,1),Vec2(5,2),Vec2(5,3),Vec2(5,4),Vec2(5,5), // 10
    };
    return surroundedCoords;
}

void UnitArcher::shoot()
{
    if (status == Alive && targetBuilding->status == Building::Alive) {
        
        auto arrow = CCSprite::createWithSpriteFrameName("unit/archer/arrow/dark.png");
        arrow->setPosition(this->getPosition());

        // 矢を施設方向に向けて回転
        float degree = tmx->calcDegree(coord, targetBuilding->coord);
        arrow->setRotation(225 + degree);
        
        auto parent = getParent();
        auto shot = JumpTo::create(0.6, targetBuilding->getPosition(),20,1);
        FiniteTimeAction* hit = CallFunc::create([=]() {
            
            tmx->playSE("arrow_hit");
            targetBuilding->attacked(damagePerAttack);
            if (parent) {
                parent->removeChild(arrow);
            }
        });
        auto disappear = FadeOut::create(0.1);
        auto sequence = Sequence::create(shot, hit, disappear, NULL);
        arrow->runAction(sequence);

        if (parent) {
            parent->addChild(arrow,ArrowZOrder,ArrowTag);
        }
    }
}

void UnitArcher::playStartAttackingVoice()
{
    tmx->playSE("arrow_shoot");
};
