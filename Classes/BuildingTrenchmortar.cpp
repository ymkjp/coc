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
    auto expandAction = ScaleTo::create(0.02, 1.02);
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
        
        bulletShadow = CCSprite::createWithSpriteFrameName("stage/battle_effect/bullet_shadow.png");
        bulletShadow->setPosition(this->getPosition());
        
        auto spot = targetUnit->getPosition();
        auto parentNode = getParent();
        
        FiniteTimeAction* fire = CallFunc::create([=]() {
            if (status == Alive && luminousAction && smokeAction) {
                luminousAction->gotoFrameAndPlay(0, false);
                smokeAction->gotoFrameAndPlay(0, false);
            }
        });
        auto shot = JumpTo::create(2, targetUnit->getPosition(), 200, 1);

        FiniteTimeAction* hit = CallFunc::create([=]() {
            // @todo 射程距離にいるユニットにダメージ
            audioManager->playSE("mortar_hit");
            if (parentNode) {
                impactNode = CSLoader::createNode("res/MortarImpact.csb");
                impactAction = timeline::ActionTimelineCache::createAction("res/MortarImpact.csb");
                impactNode->runAction(impactAction);
                parentNode->addChild(impactNode,MortalImpactZOrder);
                impactNode->setPosition(spot);
                impactAction->gotoFrameAndPlay(0, false);

                targetUnit->attacked(getDamagePerShot());
                parentNode->removeChild(bullet);
                parentNode->removeChild(bulletShadow);
            }
            
            // 画面を揺らす
            auto backgroundLayer = parentNode->getParent();
            if (backgroundLayer) {
                auto prevPos = backgroundLayer->getPosition();
                Vector<FiniteTimeAction*> arrayOfactions;
                for (int count = 0; count < 5; ++count) {
                    auto shakeLeft = MoveBy::create(0.1, Vec2(- 2, 0));
                    auto shakeRight = MoveBy::create(0.1, Vec2(+ 2, 0));
                    arrayOfactions.pushBack(shakeLeft);
                    arrayOfactions.pushBack(shakeRight);
                }
                arrayOfactions.pushBack(MoveTo::create(0.1, prevPos));
//                CCLOG("prevPos(%f,%f)",prevPos.x,prevPos.y);
                backgroundLayer->runAction(Sequence::create(arrayOfactions));
            }
        });
        auto disappear = FadeOut::create(0.1);
        auto sequence = Sequence::create(fire, shot, hit, disappear, NULL);
        bullet->runAction(sequence);
        
        // 影も移動
        auto shadowMoving = MoveTo::create(1, targetUnit->getPosition());
        auto shadowSequence = Sequence::create(shadowMoving,disappear, NULL);
        bulletShadow->runAction(shadowSequence);
        
        if (parentNode) {
            parentNode->addChild(bullet,BulletZOrder);;
            parentNode->addChild(bulletShadow,BulletShadowZOrder);
        }
    }
}