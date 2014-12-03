#include "NodeFactory.h"

USING_NS_CC;

#include "BuildingArchertower.h"
#include "BuildingCanon.h"
#include "BuildingElixertank.h"
#include "BuildingGoldbank.h"
#include "BuildingTownhall.h"
#include "BuildingTrenchmortar.h"
#include "BuildingWall.h"

#include "UnitArcher.h"
#include "UnitBarbarian.h"
#include "UnitGiant.h"
#include "UnitGoblin.h"
#include "UnitWallbreaker.h"


bool NodeFactory::init(Tmx* _tmx)
{
    tmx = _tmx;
    return true;
}

Unit* NodeFactory::createUnit(UnitType type, Vec2 coord)
{
    switch (type) {
        case Archer:
            return UnitArcher::create(tmx, coord);
            break;
        case Barbarian:
            return UnitBarbarian::create(tmx, coord);
            break;
        case Giant:
            return UnitGiant::create(tmx, coord);
            break;
        case Goblin:
            return UnitGoblin::create(tmx, coord);
            break;
        case Wallbreaker:
            return UnitWallbreaker::create(tmx, coord);
            break;
            
        default:
            // Unknown UnitType
            return UnitArcher::create(tmx, coord);
            break;
    }
}

Building* NodeFactory::createBuilding(BuildingType type, Vec2 coord)
{
    switch (type) {
        case TownHall:
            return BuildingTownhall::create(tmx, coord);
            break;
        case ElixerTank:
            return BuildingElixertank::create(tmx, coord);
            break;
        case GoldBank:
            return BuildingGoldbank::create(tmx, coord);
            break;
        case Canon:
            return BuildingCanon::create(tmx, coord);
            break;
        case TrenchMortar:
            return BuildingTrenchmortar::create(tmx, coord);
            break;
        case ArcherTower:
            return BuildingArchertower::create(tmx, coord);
            break;
        case Wall:
            return BuildingWall::create(tmx, coord);
            break;
        default:
            // Unknown BuildingType
            return BuildingWall::create(tmx, coord);
            break;
    }
}


void NodeFactory::updateAttackRangeGrid()
{
    BuildingDefense::updateAttackRangeGrid(this->tmx);
}

