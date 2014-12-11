#include "BuildingCanon.h"

USING_NS_CC;

void BuildingCanon::initOwn()
{
    smokeNode = CSLoader::createNode("res/CanonSmoke.csb");
    smokeAction = timeline::ActionTimelineCache::createAction("res/CanonSmoke.csb");
    smokeNode->runAction(smokeAction);
    smokeAction->gotoFrameAndPause(60);
    this->addChild(smokeNode,CanonSmokeOrder,CanonSmokeNodeTag);
    
    luminousNode = CSLoader::createNode("res/LuminousCircle.csb");
    luminousAction = timeline::ActionTimelineCache::createAction("res/LuminousCircle.csb");
    luminousNode->runAction(luminousAction);
    luminousAction->gotoFrameAndPause(60);
    this->addChild(luminousNode,LuminousCirclebOrder, LuminousCircleNodeTag);
}

void BuildingCanon::attack()
{
    // 向き先に応じてアニメーションを切り替え
    FiniteTimeAction* turn = CallFunc::create([=]() {
        // @todo 180度未満の場合は10度づつ順回転、180度以上の場合は逆回転
        if (targetUnit->status == Unit::Alive) {
            float comassDegreeGoal = tmx->calcCompassDegree(coord, targetUnit->coord) + 45;
            if (360 < comassDegreeGoal) {
                comassDegreeGoal -= 360;
            }
            float flameGoal = ceil(comassDegreeGoal * 0.1);
            if (motionAction) {
                motionAction->gotoFrameAndPause(flameGoal);
            }
        }
    });
    
    FiniteTimeAction* attack = CallFunc::create(CC_CALLBACK_0(BuildingCanon::expandAndShrink, this));
    auto seq = Sequence::create(turn, attack, nullptr);
    this->runAction(seq);
}

void BuildingCanon::expandAndShrink()
{
    // @todo 火花・煙
    auto expandAction = ScaleTo::create(0.02, 1.02);
    auto delay = DelayTime::create(0.1);
    FiniteTimeAction* shootBullet = CallFunc::create([=]() {
        this->shoot();
    });
    auto shrinkAction = ScaleTo::create(0.02, 1);
    auto sequence = Sequence::create(expandAction, delay, shootBullet, shrinkAction, NULL);
    // @todo buildingNode 解放ハンドリング
    if (buildingNode) {
        buildingNode->runAction(sequence);
    }
}

void BuildingCanon::shoot()
{
    if (targetUnit->status == Unit::Alive) {
        audioManager->playSE("cannon_attack");
        bullet = CCSprite::createWithSpriteFrameName("stage/battle_effect/bullet.png");
        bullet->setPosition(this->getPosition());
        
        FiniteTimeAction* fire = CallFunc::create([=]() {
            if (luminousAction && smokeAction) {
                luminousAction->gotoFrameAndPlay(0, false);
                smokeAction->gotoFrameAndPlay(0, false);
            }
        });
        auto shot = MoveTo::create(0.6, targetUnit->getPosition());
        
        FiniteTimeAction* hit = CallFunc::create([=]() {
            // @todo ターゲットユニットがまだ射程距離にいるか
            targetUnit->attacked(getDamagePerShot());
            getParent()->removeChild(bullet);
        });
        auto disappear = FadeOut::create(0.1);
        auto sequence = Sequence::create(fire, shot, hit, disappear, NULL);
        bullet->runAction(sequence);
        getParent()->addChild(bullet);
    }
}