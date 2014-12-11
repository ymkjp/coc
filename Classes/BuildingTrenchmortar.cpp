#include "BuildingTrenchmortar.h"

USING_NS_CC;

void BuildingTrenchmortar::initOwn()
{
    smokeNode = CSLoader::createNode("res/MortarSmoke.csb");
    smokeAction = timeline::ActionTimelineCache::createAction("res/MortarSmoke.csb");
    smokeNode->runAction(smokeAction);
    smokeAction->gotoFrameAndPause(60);
    this->addChild(smokeNode,CanonSmokeOrder,CanonSmokeNodeTag);
    
    luminousNode = CSLoader::createNode("res/LuminousCircle.csb");
    luminousAction = timeline::ActionTimelineCache::createAction("res/LuminousCircle.csb");
    luminousNode->runAction(luminousAction);
    luminousAction->gotoFrameAndPause(60);
    this->addChild(luminousNode,LuminousCirclebOrder, LuminousCircleNodeTag);
}

void BuildingTrenchmortar::attack()
{
    // 向き先に応じてアニメーションを切り替え
    FiniteTimeAction* turn = CallFunc::create([=]() {
        if (motionAction && targetUnit->status == Unit::Alive) {
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
    auto expandAction = ScaleTo::create(0.02, 1.06);
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
        audioManager->playSE("mortar_shoot");
        bullet = CCSprite::createWithSpriteFrameName("stage/battle_effect/bullet.png");
        bullet->setPosition(this->getPosition());
        
        auto spot = targetUnit->getPosition();
        auto parentNode = getParent();
        
        FiniteTimeAction* fire = CallFunc::create([=]() {
            if (luminousAction && smokeAction) {
                luminousAction->gotoFrameAndPlay(0, false);
                smokeAction->gotoFrameAndPlay(0, false);
            }
        });
        auto shot = JumpTo::create(1, targetUnit->getPosition(), 200, 1);
        FiniteTimeAction* hit = CallFunc::create([=]() {
            // @todo 射程距離にいるユニットにダメージ
            audioManager->playSE("mortar_hit");
            if (parentNode) {
                impactNode = CSLoader::createNode("res/MortarImpact.csb");
                impactAction = timeline::ActionTimelineCache::createAction("res/MortarImpact.csb");
                impactNode->runAction(impactAction);
                parentNode->addChild(impactNode);
                impactNode->setPosition(spot);
                impactAction->gotoFrameAndPlay(0, false);
            }
            targetUnit->attacked(getDamagePerShot());
            getParent()->removeChild(bullet);
        });
        auto disappear = FadeOut::create(0.1);
        auto sequence = Sequence::create(fire, shot, hit, disappear, NULL);
        bullet->runAction(sequence);
        getParent()->addChild(bullet);
    }
}