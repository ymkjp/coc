#include "BuildingTrenchmortar.h"

USING_NS_CC;

void BuildingTrenchmortar::initOwn()
{
    auto smokeNode = CSLoader::createNode("res/MortarSmoke.csb");
    auto smokeAction = tmx->actionTimelineCache->createAction("res/MortarSmoke.csb");
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

void BuildingTrenchmortar::attack()
{
    auto buildingNode = this->getChildByTag(BuildingNodeTag);
    if (!buildingNode) {
        return;
    }
    auto motionAction = dynamic_cast<timeline::ActionTimeline*>(buildingNode->getActionByTag(BuildingActionTag));

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
    auto buildingNode = this->getChildByTag(BuildingNodeTag);
    if (!buildingNode) {
        return;
    }
    
    auto expandAction = ScaleTo::create(0.02, 1.02);
    auto shrinkAction = ScaleTo::create(0.02, 1);
    auto delay = DelayTime::create(0.1);
    FiniteTimeAction* shootBullet = CallFunc::create([=]() {
        this->shoot();
    });
    auto sequence = Sequence::create(expandAction, shrinkAction, delay, shootBullet, NULL);
    buildingNode->runAction(sequence);
}

void BuildingTrenchmortar::shoot()
{
    if (targetUnit->status == Unit::Alive) {
        auto smokeNode = this->getChildByTag(SmokeNodeTag);
        auto luminousNode = this->getChildByTag(LuminousNodeTag);
        if (!smokeNode || !luminousNode) {
            return;
        }
        auto smokeAction = dynamic_cast<timeline::ActionTimeline*>(smokeNode->getActionByTag(SmokeNodeTag));
        auto luminousAction = dynamic_cast<timeline::ActionTimeline*>(luminousNode->getActionByTag(LuminousNodeTag));
        
        tmx->playSE("mortar_shoot");
        auto bullet = CCSprite::createWithSpriteFrameName("stage/battle_effect/bullet.png");
        auto shootingPos = this->getPosition() + Vec2(0,60);
        bullet->setPosition(shootingPos);
        
        auto bulletShadow = CCSprite::createWithSpriteFrameName("stage/battle_effect/bullet_shadow.png");
        bulletShadow->setPosition(this->getPosition());
        
        auto spot = targetUnit->getPosition();
        auto parentNode = getParent();
        
        FiniteTimeAction* fire = CallFunc::create([=]() {
            if (status == Alive && luminousAction && smokeAction && luminousNode && smokeNode) {
                smokeNode->setPositionY(18);
                luminousAction->gotoFrameAndPlay(0, false);
                smokeAction->gotoFrameAndPlay(0, false);
            }
        });
        auto shot = JumpTo::create(2, targetUnit->getPosition(), 200, 1);

        FiniteTimeAction* hit = CallFunc::create([=]() {
            // @todo 射程距離にいるユニットにダメージ
            tmx->playSE("mortar_hit");
            if (parentNode) {
                auto impactNode = CSLoader::createNode("res/MortarImpact.csb");
                auto impactAction = tmx->actionTimelineCache->createAction("res/MortarImpact.csb");
                impactNode->runAction(impactAction);
                parentNode->addChild(impactNode,MortalImpactZOrder);
                impactNode->setPosition(spot);
                impactAction->gotoFrameAndPlay(0, false);

                targetUnit->attacked(getDamagePerShot());
//                parentNode->removeChildByTag(BulletNodeTag);
//                parentNode->removeChildByTag(BulletShadowNodeTag);
            }
            
            // 画面を揺らす
            auto backgroundLayer = parentNode->getParent();
            if (backgroundLayer) {
                Vector<FiniteTimeAction*> arrayOfactions;
                for (int count = 0; count < 5; ++count) {
                    auto shakeLeft = MoveBy::create(0.1, Vec2(- 6, 4));
                    auto shakeRight = MoveBy::create(0.1, Vec2(+ 6, -4));
                    arrayOfactions.pushBack(shakeLeft);
                    arrayOfactions.pushBack(shakeRight);
                }
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
            parentNode->addChild(bullet,BulletZOrder,BulletNodeTag);
            parentNode->addChild(bulletShadow,BulletShadowZOrder,BulletShadowNodeTag);
        }
    }
}