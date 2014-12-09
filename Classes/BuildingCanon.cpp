#include "BuildingCanon.h"

USING_NS_CC;

void BuildingCanon::attack()
{
    // 向き先に応じてアニメーションを切り替え
    FiniteTimeAction* turn = CallFunc::create([=]() {
        // @todo 180度未満の場合は10度づつ順回転、180度以上の場合は逆回転
        if (targetUnit->status == Unit::Alive) {
            float comassDegreeGoal = tmx->calcCompassDegree(coord, targetUnit->coord);
            float flameGoal = ceil(comassDegreeGoal * 0.1);
            motionAction->gotoFrameAndPause(flameGoal);
        }
    });
    
    FiniteTimeAction* attack = CallFunc::create(CC_CALLBACK_0(BuildingCanon::expandAndShrink, this));
    auto seq = Sequence::create(turn, attack, nullptr);
    this->runAction(seq);
}

void BuildingCanon::expandAndShrink()
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

void BuildingCanon::shoot()
{
    if (targetUnit->status == Unit::Alive) {
        bullet = CCSprite::createWithSpriteFrameName("stage/battle_effect/bullet.png");
        // @向いている方向で銃口の見え方は違う
        bullet->setPosition(this->getPosition());
        auto shot = MoveTo::create(0.6, targetUnit->getPosition());
        FiniteTimeAction* hit = CallFunc::create([=]() {
            // @todo ターゲットユニットがまだ射程距離にいるか
            targetUnit->attacked(getDamagePerShot());
            getParent()->removeChild(bullet);
        });
        auto disappear = FadeOut::create(0.1);
        auto sequence = Sequence::create(shot, hit, disappear, NULL);
        bullet->runAction(sequence);
        getParent()->addChild(bullet);
    }
}