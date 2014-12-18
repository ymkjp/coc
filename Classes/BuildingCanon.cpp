#include "BuildingCanon.h"

USING_NS_CC;

void BuildingCanon::initOwn()
{
    auto smokeNode = tmx->csLoader->createNode("res/CanonSmoke.csb");
    auto smokeAction = tmx->actionTimelineCache->createAction("res/CanonSmoke.csb");
    this->addChild(smokeNode,CanonSmokeOrder,SmokeNodeTag);
    smokeAction->setTag(SmokeActionTag);
    smokeNode->runAction(smokeAction);
    smokeAction->gotoFrameAndPause(60);
    
    auto luminousNode = tmx->csLoader->createNode("res/LuminousCircle.csb");
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
            
            // 発火位置・リコイル深度の調整値を算出
            if (0 < comassDegreeGoal && comassDegreeGoal <= 45) {
                adjustedBulletPos = Vec2(ADJUST_SMALL,ADJUST_HUGE);
            } else if (45 < comassDegreeGoal && comassDegreeGoal <= 90) {
                adjustedBulletPos = Vec2(ADJUST_LARGE,ADJUST_REGULAR);
            } else if (90 < comassDegreeGoal && comassDegreeGoal <= 135) {
                adjustedBulletPos = Vec2(ADJUST_LARGE,-ADJUST_SMALL);
            } else if (135 < comassDegreeGoal && comassDegreeGoal <= 180) {
                adjustedBulletPos = Vec2(ADJUST_REGULAR,-ADJUST_REGULAR);
            } else if (180 < comassDegreeGoal && comassDegreeGoal <= 225) {
                adjustedBulletPos = Vec2(-ADJUST_REGULAR,-ADJUST_REGULAR);
            } else if (225 < comassDegreeGoal && comassDegreeGoal <= 270) {
                adjustedBulletPos = Vec2(-ADJUST_LARGE,-ADJUST_REGULAR);
            } else if (270 < comassDegreeGoal && comassDegreeGoal <= 315) {
                adjustedBulletPos = Vec2(-ADJUST_LARGE,ADJUST_SMALL);
            } else if (315 < comassDegreeGoal && comassDegreeGoal <= 360) {
                adjustedBulletPos = Vec2(-ADJUST_SMALL,ADJUST_HUGE);
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
    if (!targetUnit || targetUnit->status == Unit::Died) {
        return;
    }
    auto recoilPos = adjustedBulletPos * -0.1;
    auto expandAction = Spawn::create(ScaleTo::create(0.2, 1.1), MoveBy::create(0.2,recoilPos) ,NULL);
    auto shrinkAction = Spawn::create(ScaleTo::create(0.2, 1), MoveBy::create(0.2,recoilPos * -1) ,NULL);
    FiniteTimeAction* shootBullet = CallFunc::create([=]() {
        this->shoot();
    });
    auto sequence = Sequence::create(shootBullet,expandAction,shrinkAction, NULL);
    auto node = this->getChildByTag(BuildingNodeTag);
    if (node) {
        Node* barrel = node->getChildByName("Barrel");
        if (barrel) {
            barrel->runAction(sequence);
        }
    }
}

void BuildingCanon::shoot()
{
    if (targetUnit->status == Unit::Alive) {
        auto smokeNode = this->getChildByTag(SmokeNodeTag);
        auto luminousNode = this->getChildByTag(LuminousNodeTag);
        if (!smokeNode || !luminousNode) {
            return;
        }
        auto bullet = CCSprite::createWithSpriteFrameName("stage/battle_effect/bullet.png");
        bullet->setScale(0.6);
        bullet->setRotation(-90);
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
        
        auto prevCoord = targetUnit->coord;
        float distance = targetUnit->coord.getDistanceSq(coord);
        float sec = distance * 0.006;
        CCLOG("targetUnit->coord.getDistanceSq(%f),sec(%f)",distance,sec);
        sec = (sec < 0.26) ? 0.26 : sec;
        
        auto shot = MoveTo::create(sec, targetUnit->getPosition());
        auto parent = getParent();
        
        auto disappear = FadeOut::create(0.01);
        FiniteTimeAction* hit = CallFunc::create([=]() {
            if (targetUnit && targetUnit->coord == prevCoord) {
                targetUnit->attacked(getDamagePerShot());
            }
        });
        FiniteTimeAction* invisible = CallFunc::create([=]() {
            if (bullet) {
                bullet->setVisible(false);
            }
        });
        auto sequence = Sequence::create(fire, shot, disappear, hit, invisible, NULL);
        bullet->runAction(sequence);
        
        if (parent) {
            parent->addChild(bullet,BulletZOrder,BulletNodeTag);
        }
        
    }
}