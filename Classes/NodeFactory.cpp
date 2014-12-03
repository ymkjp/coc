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
