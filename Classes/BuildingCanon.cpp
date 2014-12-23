#include "BuildingCanon.h"

USING_NS_CC;

void BuildingCanon::initOwn()
{
    idNum = rand() % 100;
    
    auto smokeNode = tmx->csLoader->createNode("res/CanonSmoke.csb");
    auto smokeAction = tmx->actionTimelineCache->createAction("res/CanonSmoke.csb");
    this->addChild(smokeNode,CanonSmokeOrder,SmokeNodeTag);
    smokeAction->setTag(SmokeActionTag);
    smokeNode->runAction(smokeAction);
    smokeAction->gotoFrameAndPause(0);
    
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
    
    if (!node || !action || targetUnit->status == Unit::Died) {
        return;
    }
    
    // 180度未満の場合は10度づつ順回転、180度以上の場合は逆回転
    Vector<FiniteTimeAction*> arrayOfactions = {};
    float comassDegreeGoal = tmx->calcCompassDegree(coord, targetUnit->coord) + 45;
    if (360 < comassDegreeGoal) {
        comassDegreeGoal -= 360;
    }
    int goalFrame = ceil(comassDegreeGoal * 0.1);
    goalFrame = (goalFrame == 36) ? 0 : goalFrame;
    
    int currentFrame = action->getCurrentFrame();
    int frameDiff = goalFrame - currentFrame;
    bool isClockwiseRotation = frameDiff < 18.0;
    if (frameDiff < 0) {
        isClockwiseRotation = 18.0 < abs(frameDiff);
    }
    
//    CCLOG("\n\n\n[id:%i]currentFrame(%i),goalFrame(%i),isCWR(%i,%i)",idNum,currentFrame,goalFrame,isClockwiseRotation,goalFrame - currentFrame);
    ++times;
    int count = 0;
    int breaker = 37;
    while (currentFrame != goalFrame) {
        ++count;
        if (breaker < count) {
            break;
        }
        
//        CCLOG("[%i.01]currentFrame(%i),goalFrame(%i),isCWR(%i)",times,currentFrame,goalFrame,isClockwiseRotation);
        
        if (isClockwiseRotation) {
            currentFrame = abs(currentFrame + 1) % 36;
        } else if (!isClockwiseRotation && currentFrame == 0) {
            currentFrame = 35;
        } else if (!isClockwiseRotation) {
            currentFrame = abs(currentFrame - 1) % 36;
        }
//        CCLOG("[%i.02]currentFrame(%i),goalFrame(%i),isCWR(%i)",times,currentFrame,goalFrame,isClockwiseRotation);
        
        FiniteTimeAction* tick = CallFunc::create([=]() {
            if (action && targetUnit->status == Unit::Alive) {
                action->gotoFrameAndPause(currentFrame % 36);
            }
        });
        arrayOfactions.pushBack(tick);
        arrayOfactions.pushBack(DelayTime::create(0.01));
    }
    
    // 最終地点の角度に到達させる
    FiniteTimeAction* lastTick = CallFunc::create([=]() {
        if (action && targetUnit->status == Unit::Alive) {
            action->gotoFrameAndPause(goalFrame);
        }
    });
    arrayOfactions.pushBack(lastTick);
    
    // 発火位置・リコイル深度の調整値を算出
    if (0 <= goalFrame && goalFrame <= 18) {
        adjustedBulletPos = adjustingFirePos.at(goalFrame);
    } else if (19 <= goalFrame && goalFrame <= 36) {
        auto flippedPos = adjustingFirePos.at(36 - goalFrame);
        adjustedBulletPos = Vec2(-flippedPos.x, flippedPos.y);
    }
    
    FiniteTimeAction* attack = CallFunc::create(CC_CALLBACK_0(BuildingCanon::expandAndShrink, this));
    arrayOfactions.pushBack(attack);
    
    auto seq = Sequence::create(arrayOfactions);
    this->runAction(seq);
}

void BuildingCanon::expandAndShrink()
{
    if (!targetUnit || targetUnit->status == Unit::Died) {
        return;
    }
    auto recoilPos = adjustedBulletPos * -0.26;
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
        bullet->setScale(0.8);
        bullet->setRotation(-90);
        auto shootingPos = this->getPosition() + adjustedBulletPos;
        bullet->setPosition(shootingPos);
        
        auto smokeAction = dynamic_cast<timeline::ActionTimeline*>(smokeNode->getActionByTag(SmokeNodeTag));
        auto luminousAction = dynamic_cast<timeline::ActionTimeline*>(luminousNode->getActionByTag(LuminousNodeTag));
        FiniteTimeAction* fire = CallFunc::create([=]() {
            if (luminousNode && smokeNode && luminousAction && smokeAction && status == Alive) {
                // 発火・煙のエフェクト
                tmx->playSE("cannon_attack");
                smokeNode->setPosition(adjustedBulletPos * 0.96);
                luminousAction->gotoFrameAndPlay(0, false);
                smokeAction->gotoFrameAndPlay(0, false);
            }
        });
        
        auto prevCoord = targetUnit->coord;
        float distance = targetUnit->coord.getDistanceSq(coord);
        if (distance <= 2) {
            bullet->setVisible(false);
        }
        float sec = distance * 0.005;
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