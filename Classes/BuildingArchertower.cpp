#include "BuildingArchertower.h"

USING_NS_CC;

void BuildingArchertower::initArchersOnTower()
{
    // アーチャーをタワーの上に載せる
    for (int i = 0; i < numberOfArchersOnTower; ++i) {
        auto archer = ArcherOnTower::create(tmx, coord, getDamagePerShot());
        archersOnTower.pushBack(archer);
        this->addChild(archer,ArcherOnTowerOrder,ArcherOnTowerTag);
    }
    
    // アーチャー1体ずつ位置を調整する (奥、左、右)
    archersOnTower.at(0)->setPosition(Vec2(0,46));
    archersOnTower.at(1)->setPosition(Vec2(-12,42));
    archersOnTower.at(2)->setPosition(Vec2(8,38));
}

void BuildingArchertower::attack()
{
    // 向き先に応じてアニメーションを切り替え
    FiniteTimeAction* turn = CallFunc::create([=]() {
        // @todo 180度未満の場合は10度づつ順回転、180度以上の場合は逆回転
        if (targetUnit->status == Unit::Alive) {
            float comassDegreeGoal = tmx->calcCompassDegree(coord, targetUnit->coord) + 45;
            if (360 < comassDegreeGoal) {
                comassDegreeGoal -= 360;
            }
            this->setDirection(comassDegreeGoal);
            this->updateArchersDirection();
        }
    });
    
    FiniteTimeAction* attack = CallFunc::create(CC_CALLBACK_0(BuildingArchertower::shoot, this));
    auto seq = Sequence::create(turn, attack, nullptr);
    this->runAction(seq);
}

inline void BuildingArchertower::setDirection(float degree)
{
    float compassIndex = ceil(degree / 45);
    if (compassIndex == 0) {
        direction = North;
    } else if (compassIndex == 1) {
        direction = NorthEast;
    } else if (compassIndex == 2) {
        direction = East;
    } else if (compassIndex == 3) {
        direction = SouthEast;
    } else if (compassIndex == 4) {
        direction = South;
    } else if (compassIndex == 5) {
        direction = SouthWest;
    } else if (compassIndex == 6) {
        direction = West;
    } else if (compassIndex == 7) {
        direction = NorthWest;
    }
}

void BuildingArchertower::updateArchersDirection()
{
    for (int i = 0; i < numberOfArchersOnTower; ++i) {
        archersOnTower.at(i)->setDirection(direction);
        archersOnTower.at(i)->updateMotionNode();
    }
}

void BuildingArchertower::shoot()
{
    if (targetUnit->status == Unit::Alive) {
        updateAttacingArcherIndex();
        auto archer = archersOnTower.at(attackingArcherIndex);
        archer->targetUnit = targetUnit;
        archer->shoot();
    }
}

inline void BuildingArchertower::updateAttacingArcherIndex()
{
    attackingArcherIndex += 1;
    if (numberOfArchersOnTower - 1 < attackingArcherIndex) {
        attackingArcherIndex = 0;
    }
}
