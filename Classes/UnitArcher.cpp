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
    CCLOG("fileName:%s,this->action:%i,,this->compass:%i",fileName.getCString(),this->action,this->compass);
    return fileName.getCString();
}

std::vector<Vec2> UnitArcher::getSurroundedCoords(Vec2 targetCoord)
{
    // 射程距離 3
    const std::vector<Vec2> surroundedCoords = {
        Vec2(-4,-4),Vec2(-4,-3),Vec2(-4,-2),Vec2(-4,-1),Vec2(-4,0),Vec2(-4,1),Vec2(-4,2),Vec2(-4,3),Vec2(-4,4),Vec2(-4,5), // 10
        Vec2(-3,-4),Vec2(-3,-3),Vec2(-3,-2),Vec2(-3,-1),Vec2(-3,0),Vec2(-3,1),Vec2(-3,2),Vec2(-3,3),Vec2(-3,4),Vec2(-3,5),
        Vec2(-2,-4),Vec2(-2,-3),Vec2(-2,-2),Vec2(-2,-1),Vec2(-2,0),Vec2(-2,1),Vec2(-2,2),Vec2(-2,3),Vec2(-2,4),Vec2(-2,5),
        Vec2(-1,-4),Vec2(-1,-3),Vec2(-1,-2),Vec2(-1,-1),Vec2(-1,0),Vec2(-1,1),Vec2(-1,2),Vec2(-1,3),Vec2(-1,4),Vec2(-1,5),
        Vec2(0,-4),Vec2(0,-3),Vec2(0,-2),Vec2(0,-1),Vec2(0,0),Vec2(0,2),Vec2(0,3),Vec2(0,4),Vec2(0,5),
        Vec2(1,-4),Vec2(1,-3),Vec2(1,-2),Vec2(1,-1),Vec2(1,0),Vec2(1,2),Vec2(1,3),Vec2(1,4),Vec2(1,5),
        Vec2(2,-4),Vec2(2,-3),Vec2(2,-2),Vec2(2,-1),Vec2(2,0),Vec2(2,2),Vec2(2,3),Vec2(2,4),Vec2(2,5),
        Vec2(3,-4),Vec2(3,-3),Vec2(3,-2),Vec2(3,-1),Vec2(3,0),Vec2(3,2),Vec2(3,3),Vec2(3,4),Vec2(3,5),
        Vec2(4,-4),Vec2(4,-3),Vec2(4,-2),Vec2(4,-1),Vec2(4,0),Vec2(4,2),Vec2(4,3),Vec2(4,4),Vec2(4,5),
        Vec2(5,-4),Vec2(5,-3),Vec2(5,-2),Vec2(5,-1),Vec2(5,0),Vec2(5,1),Vec2(5,2),Vec2(5,3),Vec2(5,4),Vec2(5,5), // 10
    };
    return surroundedCoords;
}
