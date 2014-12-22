#include "ArcherOnTower.h"

USING_NS_CC;

bool ArcherOnTower::init(Tmx* _tmx, Vec2 _coord, float _damagePerShot)
{
    tmx = _tmx;
    coord = _coord;
    damagePerShot = _damagePerShot;
    
    action = Idle;
    direction = East;
    
    // 塔の上のアーチャー
    auto motionNode = CCSprite::createWithSpriteFrameName("unit/archer/idle/south.png");
    motionNode->setScale(0.92);
    this->addChild(motionNode,MotionOrder,MotionTag);
    
    // ユニットの影
    auto shadowSprite = CCSprite::createWithSpriteFrameName("unit/shadow/0.0.png");
    this->addChild(shadowSprite,ShadowOrder,ShadowTag);
    shadowSprite->setOpacity(120);
    shadowSprite->setPosition(3,-12);
    return true;
}

void ArcherOnTower::shoot()
{
    CCLOG("archer_tower_shoot");
    auto baseParent = getParent();
    if (!baseParent) {
        return;
    }
    if (targetUnit->status == Unit::Alive) {
        arrow = CCSprite::createWithSpriteFrameName("unit/archer/arrow/dark.png");
        arrow->setPosition(baseParent->getPosition() + getPosition());
        
        // 矢を施設方向に向けて回転
        float degree = tmx->calcDegree(coord, targetUnit->coord);
        arrow->setRotation(225 + degree);
        arrow->setScale(1.6);
        
        // シークエンス
        FiniteTimeAction* shootMotion = CallFunc::create([=]() {
            if (baseParent) {
                tmx->playSE("archer_tower_shoot");
                this->action = Attacking;
                this->updateMotionNode();
                this->motionAction->gotoFrameAndPlay(0, false);
            }
        });
        
        // 射程外に出たときはノーダメージ
        auto prevCoord = targetUnit->coord;
        
        auto shot = JumpTo::create(0.6, targetUnit->getPosition(),20,1);
        FiniteTimeAction* hit = CallFunc::create([=]() {
            if (targetUnit && targetUnit->status == Unit::Alive && targetUnit->coord == prevCoord)
            {
                targetUnit->attacked(damagePerShot);
            }
            if (arrow) {
                arrow->setVisible(false);
            }
        });
        auto sequence = Sequence::create(shootMotion, shot, hit, NULL);
        arrow->runAction(sequence);
        
        auto mapLayer = baseParent->getParent();
        if (mapLayer) {
            mapLayer->addChild(arrow,ArrowZOrder);
        }
    }
}


void ArcherOnTower::setDirection(Compass _direction)
{
    // 真南と真北は直前の方角に依存して決定する
    auto currentDirection = this->direction;
    
    switch (_direction) {
        case North:
            if (currentDirection == NorthWest
                || currentDirection == West
                || currentDirection == SouthWest) {
                direction = NorthWest;
            } else if (currentDirection == NorthEast
                       || currentDirection == East
                       || currentDirection == SouthEast) {
                direction = NorthEast;
            }
            break;
        case South:
            if (currentDirection == NorthWest
                || currentDirection == West
                || currentDirection == SouthWest) {
                direction = SouthWest;
            } else if (currentDirection == NorthEast
                       || currentDirection == East
                       || currentDirection == SouthEast) {
                direction = SouthEast;
            }
            break;
            
        default:
            direction = _direction;
            break;
    }
}


void ArcherOnTower::updateMotionNode()
{
    switch (action) {
        case Idle:
        {
            this->removeChild(getChildByTag(MotionTag));
            Sprite* motionNode;
            
            if (direction == East)
            {
                motionNode = CCSprite::createWithSpriteFrameName("unit/archer/walk/east/07.png");
                
            } else if (direction == West)
            {
                motionNode = CCSprite::createWithSpriteFrameName("unit/archer/walk/east/07.png");
                motionNode->setFlippedX(true);
                
            } else if (direction == NorthEast)
            {
                motionNode = CCSprite::createWithSpriteFrameName("unit/archer/walk/north/07.png");
                
            } else if (direction == NorthWest)
            {
                motionNode = CCSprite::createWithSpriteFrameName("unit/archer/walk/north/07.png");
                motionNode->setFlippedX(true);
                
            } else if (direction == SouthEast)
            {
                motionNode = CCSprite::createWithSpriteFrameName("unit/archer/idle/south.png");
                
            } else if (direction == SouthWest)
            {
                motionNode = CCSprite::createWithSpriteFrameName("unit/archer/idle/south.png");
                motionNode->setFlippedX(true);
                
            } else {
                CCLOG("unknown direction(%i)",direction);
            }
            motionNode->setScale(0.94);
            this->addChild(motionNode,MotionOrder,MotionTag);
            break;
        }
        case Attacking:
        {
            this->removeChild(getChildByTag(MotionTag));
            
            __String filename = "res/UnitArcherAttack";
            if (direction == East)
            {
                filename.append("East");
                
            } else if (direction == West)
            {
                filename.append("West");
                
            } else if (direction == NorthEast)
            {
                filename.append("NorthEast");
                
            } else if (direction == NorthWest)
            {
                filename.append("NorthWest");
                
            } else if (direction == SouthEast)
            {
                filename.append("SouthEast");
                
            } else if (direction == SouthWest)
            {
                filename.append("SouthWest");
            }
            
            filename.append(".csb");
            
            auto motionNode = tmx->csLoader->createNode(filename.getCString());
            motionNode->setScale(0.94);
            motionAction = tmx->actionTimelineCache->createAction(filename.getCString());
            motionNode->runAction(motionAction);
            
            this->addChild(motionNode,MotionOrder,MotionTag);
            break;
        }
        default:
        {
            break;
        }
    }
    
}
