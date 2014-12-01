#include "Unit.h"

USING_NS_CC;

const std::map<Vec2, Unit::__COMPASS> Unit::compassByCoords =
{
    {Vec2(-1,-1), North},
    {Vec2(-1,0),  NorthEast},
    {Vec2(1,-1),  East},
    {Vec2(1,0),   SouthEast},
    {Vec2(1,1),   South},
    {Vec2(0,1),   SouthWest},
    {Vec2(-1,1),  West},
    {Vec2(-1,0),  NorthWest},
};

bool Unit::init(Tmx* _tmx, Vec2 _coord)
{
    tmx = _tmx;
    type = this->getUnitType();
    coord = _coord;
    damagePerSec = 60;
    action = Walking;
    
    actionTimelineCache = timeline::ActionTimelineCache::getInstance();
    
    unitNode = Node::create();
    auto childNode = this->getActingNode();
    actionTimeline = this->getActionTimeline();
    
    childNode->runAction(actionTimeline);
    actionTimeline->gotoFrameAndPlay(0, true);
    unitNode->addChild(childNode,1 ,"childNode");
    this->addChild(unitNode);
    
    this->play(1);
    return true;
}

void Unit::update( float frame )
{
}

void Unit::startAttacking()
{
    auto childNode = unitNode->getChildByName("childNode");
    auto newChildNode = this->getActingNode();

    auto action = this->getActionTimeline();
    newChildNode->runAction(action);
    action->gotoFrameAndPlay(0, true);

    unitNode->removeChild(childNode);
    unitNode->addChild(newChildNode ,1,"childNode");
    actionTimeline = action;

    if (this->action == Walking) {
        this->action = Attacking;
        this->schedule(schedule_selector(Unit::attack), 1.0f);
    }
}


void Unit::attack(float frame)
{
//    CCLOG("targetBuilding->status %i",this->targetBuilding->status);
    this->targetBuilding->attacked(damagePerSec);
    if (this->targetBuilding->status == Building::Died) {
//        CCLOG("[%i]Unit::attack target died!",__LINE__);
        action = Walking;
        
        auto childNode = unitNode->getChildByName("childNode");
        auto newChildNode = this->getActingNode();
        auto action = this->getActionTimeline();
        newChildNode->runAction(action);
        action->gotoFrameAndPlay(0, true);
        unitNode->removeChild(childNode);
        unitNode->addChild(newChildNode ,1,"childNode");
        actionTimeline = action;
        
        this->scheduleOnce(schedule_selector(Unit::play), 0);
        this->unschedule(schedule_selector(Unit::attack));
    }
}

inline bool Unit::isNextCoord(float num)
{
    return (num == -1.0f || num == 0.0f || num == 1.0f);
}

void Unit::play(float frame)
{
    CCLOG("Unit::play frame[%f]",frame);
    if (this->tmx->noBuildings()) {
        CCLOG("@todo GAME DONE!");
        return;
    }
    auto goalPoint = this->findPointToGo();
    auto mapNavigator = MapNavigator::create(this->tmx);
    auto path = mapNavigator->navigate(this->coord, goalPoint);
    this->coord = goalPoint;
    if (path.empty()) {
        CCLOG("HMM... EMPTY PATH IS DETECTED");
        return;
    };
    
    Vec2 nextCoord;
    Vec2 prevCoord;
    Vec2 directionPoint;
    Vector<FiniteTimeAction*> arrayOfactions;
    MoveTo* moveAction;
    while (!path.empty()) {
        nextCoord = path.top();
        directionPoint = this->tmx->convertToTile(nextCoord);
        moveAction = MoveTo::create(0.5, directionPoint);
        auto posDiff =  Vec2((int)prevCoord.x - (int)nextCoord.x, (int)prevCoord.y - (int)nextCoord.y);
        // @todo 向き先に応じてアニメーションを切り替え
        //            Node* animatedNode = unit->createAnimatedNode(posDiff);
        //            FiniteTimeAction* func = CallFunc::create(CC_CALLBACK_0(Unit::animateNode, unit));
        //            FiniteTimeAction* func = CallFuncN::create([this,&animatedNode](Ref* target) {
        //                auto node = tiledMapLayer->getChildByName("unit");
        //                CCLOG("[node:%s]",node->getName().c_str());
        ////                unit->unitNode = animatedNode;
        //                tiledMapLayer->removeChildByName("unit");
        //                tiledMapLayer->addChild(animatedNode,1,"unit");
        //                // @todo
        //            });
        //            arrayOfactions.pushBack(func);
        
        arrayOfactions.pushBack(moveAction);
        path.pop();
        prevCoord = nextCoord;
    }
    //        FiniteTimeAction* attack = CallFunc::create(CC_CALLBACK_0(BattleScene::attack, this));
    FiniteTimeAction* attack = CallFunc::create(CC_CALLBACK_0(Unit::startAttacking, this));
    arrayOfactions.pushBack(attack);
    auto seq = Sequence::create(arrayOfactions);

    this->unitNode->runAction(seq);    
}


std::vector<Vec2> Unit::getTargetCoords(Building::__CATEGORY category)
{
    auto types = Building::getTypesByCategory(category);
    
    std::vector<Vec2> targetCoords = {};
    for (auto type: types) {
//        CCLOG("[Unit::getTargetCoords]type%i",type);
        targetCoords.insert(
                            targetCoords.end(),
                            this->tmx->buildingCoords[type].begin(),
                            this->tmx->buildingCoords[type].end());
    }
    return targetCoords;
}


