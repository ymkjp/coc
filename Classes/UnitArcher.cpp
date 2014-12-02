#include "UnitArcher.h"

USING_NS_CC;

UnitType UnitArcher::getUnitType()
{
    return Archer;
}

__String UnitArcher::createFilename()
{
    __String fileName = "CocosProject_01/res/UnitArcher";
    switch (this->action) {
        case Unit::Attacking:
            fileName.append("Attack");
            break;
        case Unit::Walking:
            fileName.append("Walk");
            break;
        default:
            CCLOG("[default] Unit::getActionTimeline unexpected action");
            fileName.append("Walk");
            break;
    }
    
    // 上・横・下 (真北と真南は直前の compass に依存するためここ以前で設定されていない想定)
    if (this->compass == East) {
        fileName.append("East");
    } else if (this->compass == West) {
        fileName.append("West");
    } else if (this->compass == NorthEast) {
        fileName.append("NorthEast");
    } else if (this->compass == NorthWest) {
        fileName.append("NorthWest");
    } else if (this->compass == SouthEast) {
        fileName.append("SouthEast");
    } else if (this->compass == SouthWest) {
        fileName.append("SouthWest");
    } else {
        CCLOG("[default] Unit::getActionTimeline unexpected direction:%i",this->compass);
        fileName.append("SouthEast");
    }
    
    fileName.append(".csb");
    CCLOG("fileName%s",fileName.getCString());
    return fileName.getCString();
}

void UnitArcher::animateNode()
{
    posDiff = Vec2(1,1);
    if (!this->isNextCoord(posDiff.x) || !this->isNextCoord(posDiff.y)) {
        CCLOG("POS DIFF OUT OF RANGE (%f,%f)",posDiff.x,posDiff.y);
        return;
    }
    
    std::string fileName = "CocosProject/res/Unit";
    switch (type) {
        case Archer:
            fileName.append("Archer");
            break;
        case Barbarian:
            fileName.append("Barbarian");
            break;
        case Giant:
            fileName.append("Giant");
            break;
        case Goblin:
            fileName.append("Goblin");
            break;
        case Wallbreaker:
            fileName.append("Wallbreaker");
            break;
        default:
            CCLOG("type%i",type);
            return;
            break;
    }
    fileName.append("Walk");
    
    auto directionCompass = compassByCoords.at(posDiff);
    CCLOG("directionCompass:%i",directionCompass);
    bool isFlip = false;
    switch (directionCompass) {
        case North:
            fileName.append("South");
            break;
        case East:
            fileName.append("East");
            break;
        case South:
            fileName.append("North");
            break;
        case West:
            fileName.append("East");
            isFlip = true;
            break;
        default:
            // @todo
            return;// this->unitNode;
            break;
    }
    fileName.append(".csb");
    CCLOG("[FileName]%s",fileName.c_str());
    
    auto node = CSLoader::createNode(fileName);
    auto actionTimeline = timeline::ActionTimelineCache::createAction(fileName);
    node->runAction(actionTimeline);
    actionTimeline->gotoFrameAndPlay(0, true);
    unitNode = node;
    return;
}


