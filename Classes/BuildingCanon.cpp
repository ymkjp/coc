#include "BuildingCanon.h"

USING_NS_CC;

void BuildingCanon::initOwn()
{
    auto smokeNode = CSLoader::createNode("res/CanonSmoke.csb");
    auto smokeAction = tmx->actionTimelineCache->createAction("res/CanonSmoke.csb");
    this->addChild(smokeNode,CanonSmokeOrder,SmokeNodeTag);
    smokeAction->setTag(SmokeActionTag);
    smokeNode->runAction(smokeAction);
    smokeAction->gotoFrameAndPause(60);
    
    auto luminousNode = CSLoader::createNode("res/LuminousCircle.csb");
    auto luminousAction = tmx->actionTimelineCache->createAction("res/LuminousCircle.csb");
    this->addChild(luminousNode,LuminousCirclebOrder, LuminousNodeTag);
    luminousAction->setTag(LuminousActionTag);
    luminousNode->runAction(luminousAction);
    luminousAction->gotoFrameAndPause(60);
}

void BuildingCanon::attack()
{
    auto node = this->getChildByTag(BuildingNodeTag);
    auto action = dynamic_cast<timeline::ActionTimeline*>(node->getActionByTag(BuildingNodeTag));
    // 向き先に応じてアニメーションを切り替え
    FiniteTimeAction* turn = CallFunc::create([=]() {
        // @todo 180度未満の場合は10度づつ順回転、180度以上の場合は逆回転
        if (targetUnit->status == Unit::Alive) {
            float comassDegreeGoal = tmx->calcCompassDegree(coord, targetUnit->coord) + 45;
            if (360 < comassDegreeGoal) {
                comassDegreeGoal -= 360;
            }
            float goalFrame = ceil(comassDegreeGoal * 0.1);
//            CCLOG("goalFrame(%f),comassDegreeGoal(%f)",goalFrame,comassDegreeGoal);
            if (node && action) {
                action->gotoFrameAndPause(goalFrame);
            }
            
            // 発火位置調整
            if (0 < comassDegreeGoal && comassDegreeGoal <= 45) {
                adjustedBulletPos = Vec2(40,80);
            } else if (45 < comassDegreeGoal && comassDegreeGoal <= 90) {
                adjustedBulletPos = Vec2(80,40);
            } else if (90 < comassDegreeGoal && comassDegreeGoal <= 135) {
                adjustedBulletPos = Vec2(80,-40);
            } else if (135 < comassDegreeGoal && comassDegreeGoal <= 180) {
                adjustedBulletPos = Vec2(40,-80);
            } else if (180 < comassDegreeGoal && comassDegreeGoal <= 225) {
                adjustedBulletPos = Vec2(-40,-80);
            } else if (225 < comassDegreeGoal && comassDegreeGoal <= 270) {
                adjustedBulletPos = Vec2(-80,-40);
            } else if (270 < comassDegreeGoal && comassDegreeGoal <= 315) {
                adjustedBulletPos = Vec2(-80,40);
            } else if (315 < comassDegreeGoal && comassDegreeGoal <= 360) {
                adjustedBulletPos = Vec2(-40,80);
            }
//            CCLOG("adjustedBulletPos(%f,%f)",adjustedBulletPos.x,adjustedBulletPos.y);
        }
    });
    
    FiniteTimeAction* attack = CallFunc::create(CC_CALLBACK_0(BuildingCanon::expandAndShrink, this));
    auto seq = Sequence::create(turn, attack, nullptr);
    this->runAction(seq);
}

void BuildingCanon::expandAndShrink()
{
    auto expandAction = ScaleTo::create(0.02, 1.02);
    auto delay = DelayTime::create(0.1);
    auto shrinkAction = ScaleTo::create(0.02, 1);
    FiniteTimeAction* shootBullet = CallFunc::create([=]() {
        this->shoot();
    });
    auto sequence = Sequence::create(expandAction, shrinkAction, delay, shootBullet, NULL);
    auto node = this->getChildByTag(BuildingNodeTag);
    if (node) {
        node->runAction(sequence);
    }
}

void BuildingCanon::shoot()
{
    if (targetUnit->status == Unit::Alive) {
        auto smokeNode = this->getChildByTag(SmokeNodeTag);
        auto luminousNode = this->getChildByTag(LuminousNodeTag);
        if (!smokeNode || !luminousNode) {
            CCLOG("NONE! (!smokeNode || !luminousNode!)");
            return;
        }
        auto bullet = CCSprite::createWithSpriteFrameName("stage/battle_effect/bullet.png");
        auto shootingPos = this->getPosition() + adjustedBulletPos;
        bullet->setPosition(shootingPos);
        
        auto smokeAction = dynamic_cast<timeline::ActionTimeline*>(smokeNode->getActionByTag(SmokeNodeTag));
        auto luminousAction = dynamic_cast<timeline::ActionTimeline*>(luminousNode->getActionByTag(LuminousNodeTag));
        FiniteTimeAction* fire = CallFunc::create([=]() {
            if (luminousNode && smokeNode && luminousAction && smokeAction && status == Alive) {
                // 発火・煙のエフェクト
                tmx->playSE("cannon_attack");
                smokeNode->setPosition(adjustedBulletPos * 0.4);
                luminousAction->gotoFrameAndPlay(0, false);
                smokeAction->gotoFrameAndPlay(0, false);
            }
        });
        auto shot = MoveTo::create(0.6, targetUnit->getPosition());
        auto parent = getParent();
        
        FiniteTimeAction* hit = CallFunc::create([=]() {
            // @todo ターゲットユニットがまだ射程距離にいるか
            if (targetUnit) {
                targetUnit->attacked(getDamagePerShot());
            }
        });
        auto disappear = FadeOut::create(0.1);
        auto sequence = Sequence::create(fire, shot, hit, disappear, NULL);
        bullet->runAction(sequence);
        
        if (parent) {
            parent->addChild(bullet,BulletZOrder,BulletNodeTag);
        }
        
    }
}