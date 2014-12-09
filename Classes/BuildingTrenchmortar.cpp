#include "BuildingTrenchmortar.h"

USING_NS_CC;

void BuildingTrenchmortar::attack()
{
    // 向き先に応じてアニメーションを切り替え
    FiniteTimeAction* turn = CallFunc::create([=]() {
        if (targetUnit->status == Unit::Alive) {
            float comassDegreeGoal = tmx->calcCompassDegree(coord, targetUnit->coord);
            float flameGoal = ceil(comassDegreeGoal / 60);
            motionAction->gotoFrameAndPause(flameGoal);
        }
    });
    
    FiniteTimeAction* attack = CallFunc::create(CC_CALLBACK_0(BuildingTrenchmortar::expandAndShrink, this));
    auto seq = Sequence::create(turn, attack, nullptr);
    this->runAction(seq);
}

void BuildingTrenchmortar::expandAndShrink()
{
    // @todo 火花・煙
    auto expandAction = ScaleTo::create(0.02, 1.1);
    auto delay = DelayTime::create(0.1);
    FiniteTimeAction* shootBullet = CallFunc::create([=]() {
        this->shoot();
    });
    auto shrinkAction = ScaleTo::create(0.02, 1);
    auto sequence = Sequence::create(expandAction, delay, shootBullet, shrinkAction, NULL);
    buildingNode->runAction(sequence);
}

void BuildingTrenchmortar::shoot()
{
    if (targetUnit->status == Unit::Alive) {
        bullet = CCSprite::createWithSpriteFrameName("stage/battle_effect/bullet.png");
        bullet->setPosition(this->getPosition());
        auto shot = JumpTo::create(1, targetUnit->getPosition(), 200, 1);
        FiniteTimeAction* hit = CallFunc::create([=]() {
            // @todo 射程距離にいるユニットにダメージ
            targetUnit->attacked(getDamagePerShot());
            getParent()->removeChild(bullet);
        });
        auto disappear = FadeOut::create(0.1);
        auto sequence = Sequence::create(shot, hit, disappear, NULL);
        bullet->runAction(sequence);
        getParent()->addChild(bullet);
    }
}