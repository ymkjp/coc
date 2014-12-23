#include "BuildingTrenchmortar.h"

USING_NS_CC;

void BuildingTrenchmortar::initOwn()
{
    auto smokeNode = tmx->csLoader->createNode("res/MortarSmoke.csb");
    auto smokeAction = tmx->actionTimelineCache->createAction("res/MortarSmoke.csb");
    this->addChild(smokeNode,CanonSmokeOrder,SmokeNodeTag);
    smokeNode->setPositionY(32);
    
    smokeAction->setTag(SmokeActionTag);
    smokeNode->runAction(smokeAction);
    smokeAction->gotoFrameAndPause(0);
    
    auto luminousNode = tmx->csLoader->createNode("res/LuminousCircle.csb");
    auto luminousAction = tmx->actionTimelineCache->createAction("res/LuminousCircle.csb");
    this->addChild(luminousNode,LuminousCirclebOrder, LuminousNodeTag);
    luminousAction->setTag(LuminousActionTag);
    luminousNode->runAction(luminousAction);
    luminousAction->gotoFrameAndPause(0);
}

void BuildingTrenchmortar::attack()
{
    auto buildingNode = this->getChildByTag(BuildingNodeTag);
    if (!buildingNode) {
        return;
    }
    auto motionAction = dynamic_cast<timeline::ActionTimeline*>(buildingNode->getActionByTag(BuildingNodeTag));

    // 向き先に応じてアニメーションを切り替え
    FiniteTimeAction* turn = CallFunc::create([=]() {
        if (motionAction && status == Alive && targetUnit->status == Unit::Alive) {
            float comassDegreeGoal = tmx->calcCompassDegree(coord, targetUnit->coord);
            float flameGoal = ceil(comassDegreeGoal / 60);
            motionAction->gotoFrameAndPause(flameGoal);
        }
    });
    
    FiniteTimeAction* attack = CallFunc::create(CC_CALLBACK_0(BuildingTrenchmortar::expandAndShrink, this));
    auto seq = Sequence::create(turn, attack, nullptr);
    if (status == Alive) {
        this->runAction(seq);
    }
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
        if (status == Alive && buildingNode) {
            this->shoot();
        }
    });
    auto sequence = Sequence::create(expandAction, shrinkAction, delay, shootBullet, NULL);
    if (status == Alive && buildingNode) {
        buildingNode->runAction(sequence);
    }
}

void BuildingTrenchmortar::shoot()
{
    if (status == Alive && targetUnit->status == Unit::Alive) {
        auto smokeNode = this->getChildByTag(SmokeNodeTag);
        auto luminousNode = this->getChildByTag(LuminousNodeTag);
        if (!smokeNode || !luminousNode) {
            return;
        }
        auto smokeAction = dynamic_cast<timeline::ActionTimeline*>(smokeNode->getActionByTag(SmokeNodeTag));
        auto luminousAction = dynamic_cast<timeline::ActionTimeline*>(luminousNode->getActionByTag(LuminousNodeTag));
        
        tmx->playSE("mortar_shoot");

        auto bullet = tmx->csLoader->createNode("res/MortarBullet.csb");
        auto bulletAction = tmx->actionTimelineCache->createAction("res/MortarBullet.csb");
        auto shootingPos = this->getPosition() + Vec2(0,80);
        bullet->setColor(Color3B::GRAY);
        bullet->setPosition(shootingPos);
        bullet->runAction(bulletAction);
        bulletAction->gotoFrameAndPlay(0,true);
        
        auto bulletShadow = CCSprite::createWithSpriteFrameName("stage/battle_effect/bullet_shadow.png");
        bulletShadow->setPosition(this->getPosition());
        bulletShadow->setOpacity(120);
        bulletShadow->setScale(0.4);
        
        auto parentNode = getParent();
        
        FiniteTimeAction* fire = CallFunc::create([=]() {
            if (status == Alive && luminousAction && smokeAction && luminousNode && smokeNode) {
                luminousAction->gotoFrameAndPlay(0, false);
                smokeAction->gotoFrameAndPlay(0, false);
            }
        });
        
        float distance = targetUnit->coord.getDistanceSq(coord);
//        CCLOG("targetUnit->coord.getDistanceSq(%f)",distance);
        float sec = distance * 0.02;
        sec = (sec < 1.5) ? 1.5 : sec;
        auto jump = JumpTo::create(sec, aimedUnitPos, distance * 2, 1);
        
        // 弾からでている煙
        ParticleSystemQuad* m_emitter = ParticleSystemQuad::create("mortar_ball_smoke.plist");
        m_emitter->setTextureWithRect(SpriteFrameCache::getInstance()->getSpriteFrameByName("stage/trench_mortar/partical_smog.png")->getTexture(), SpriteFrameCache::getInstance()->getSpriteFrameByName("stage/trench_mortar/partical_smog.png")->getRect());
        m_emitter->setPosition(bullet->getContentSize() * 0.5);
        m_emitter->setAutoRemoveOnFinish(true);
        m_emitter->setDuration(distance * 0.5);
        m_emitter->setStartColor(Color4F(1, 1, 1, 0.3));
        bullet->addChild(m_emitter);
        
        auto prevCoord = targetUnit->coord;
        
        FiniteTimeAction* hit = CallFunc::create([=]() {
            
            tmx->playSE("mortar_hit");
            CCLOG("mortar_hit");
            if (parentNode) {
                auto impactNode = tmx->csLoader->createNode("res/MortarImpact.csb");
                auto impactAction = tmx->actionTimelineCache->createAction("res/MortarImpact.csb");
                impactNode->runAction(impactAction);
                parentNode->addChild(impactNode,MortalImpactZOrder);
                impactNode->setPosition(aimedUnitPos);
                impactAction->gotoFrameAndPlay(0, false);

                
                // 衝撃波の届くユニットにダメージ
                for (auto unit: tmx->units) {
                    if (unit && unit->status == Unit::Alive && isImpactRange(prevCoord, unit->coord)) {
                        unit->attacked(getDamagePerShot());
                    }
                }
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
            if (bullet) {
                bullet->setVisible(false);
            }
        });
        auto sequence = Sequence::create(fire, jump, hit, NULL);
        bullet->runAction(sequence);
        
        // 影も移動
        auto shadowMoving = MoveTo::create(sec, targetUnit->getPosition());
        FiniteTimeAction* invisible = CallFunc::create([=]() {
            if (bulletShadow) {
                bulletShadow->setVisible(false);
            }
        });
        auto shadowSequence = Sequence::create(shadowMoving,FadeOut::create(0.1),invisible, NULL);
        if (bulletShadow) {
            bulletShadow->runAction(shadowSequence);
        }
        
        if (status == Alive && parentNode) {
            parentNode->addChild(bullet,BulletZOrder,BulletNodeTag);
            parentNode->addChild(bulletShadow,BulletShadowZOrder,BulletShadowNodeTag);
        }
    }
}