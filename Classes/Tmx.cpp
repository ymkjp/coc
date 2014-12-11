#include "Tmx.h"
USING_NS_CC;

bool Tmx::init(Stages stage)
{
    tiledMap = TMXTiledMap::create(tmxFileNameByStage.at(stage).c_str());
    domainTMXLayer = tiledMap->getLayer("Domain");
    tiledMap->setVisible(TILEDMAP_VISIBLE_ON);

    // UI表示
    ui = UI::create(this);
    this->addChild(ui);
    
    // デフォルト選択はバーバリアン
    ui->selectedUnit = Barbarian;
    
    // バトルコントローラー表示
    ui->showBattleController();

    for (auto initUnitNumber: unitInitNumberByType) {
        unitRemainedCounterByType[initUnitNumber.first] = initUnitNumber.second;
        ui->updateUnitCounter(initUnitNumber.first, initUnitNumber.second);
    }
    
    // タイマー初期化
    battleStatus = WaitingForStart;
    currentBattleSecound = fullBattleSecond;
    ui->updateTimer(currentBattleSecound);
    
    // 所持コイン・エリクサ
    ui->updateBattleScore(ElixerScore, earnedBattleScoreByType.at(ElixerScore));
    ui->updateBattleScore(CoinScore, earnedBattleScoreByType.at(CoinScore));
    
    // 施設破壊率
    ui->updateDestructionRatio(0);
    
    return true;
}

void Tmx::startBattle()
{
    battleStatus = Playing;
    schedule(schedule_selector(Tmx::decreaseTimerCount), 1.0f);
}

void Tmx::decreaseTimerCount(float frame)
{
    if (battleStatus == Playing) {
        --currentBattleSecound;
        ui->updateTimer(currentBattleSecound);
    }
    
    if (currentBattleSecound < 0) {
        battleStatus = Finished;
        showBattleResult();
    }
}

Vec2 Tmx::convertToCoord(Vec2 pos)
{
    Size tileSize = tiledMap->getTileSize();
    Point deployedPoint = domainTMXLayer->convertToNodeSpace(pos);
    Size domainSize = domainTMXLayer->getContentSize();
    
    float slopeFormula = tileSize.height / tileSize.width;
    float tileX = floor((slopeFormula * deployedPoint.x + domainSize.height * 0.5 - deployedPoint.y) / tileSize.height);
    float tileY = floor((-slopeFormula * deployedPoint.x + domainSize.height * 1.5 - deployedPoint.y) / tileSize.height);
    return Vec2(tileX, tileY);
}

Vec2 Tmx::convertToRealPos(Vec2 pos)
{
    return tiledMap->getLayer("Domain")->getPositionAt(pos) + tiledMap->getTileSize() * 0.5;
}

Vec2 Tmx::convertToIso(Vec2 rawPos)
{
    Size tileSize = tiledMap->getTileSize();
    Size mapSize = tiledMap->getMapSize();
    auto pos = Vec2(rawPos.x, mapSize.height - rawPos.y);
    
    float ratio = tileSize.width / tileSize.height;
    float x = tileSize.width * 0.5 * (pos.x/(tileSize.width / ratio) - pos.y/tileSize.height);
    float y = tileSize.height * 0.5 * (mapSize.height - pos.x/(tileSize.width / ratio) - pos.y/tileSize.height);
    return Vec2(x, y) + Vec2(mapSize.width * 0.65, mapSize.height);
}

bool Tmx::noBuildings()
{
    for (auto buildingArray: buildingGrid) {
        for (auto building: buildingArray) {
            if (building != nullptr && building->type != Wall) {
                return false;
            }
        }
    }
    return true;
}

void Tmx::eraseBuilding(Building* building)
{
    auto coords = &buildingCoords.at(building->type);
//    CCLOG("[Tmx::eraseBuilding] type:%i, building->coord(%f,%f)",building->type,building->coord.x,building->coord.y);
    
//    for(int i =0; i <coords->size(); ++i){
//        CCLOG("[Tmx::eraseBuilding] cached coord(%f,%f)",coords->at(i).x,coords->at(i).y);
//    }
    
    coords->erase(remove(coords->begin(), coords->end(), building->coord), coords->end());
    
//    CCLOG("[Tmx::eraseBuilding] coords.size%lu",coords->size());
//    for(int i =0; i <coords->size(); ++i){
//        CCLOG("[Tmx::eraseBuilding] remain coord(%f,%f)",coords->at(i).x,coords->at(i).y);
//    }
    
    buildingGrid[building->coord.x][building->coord.y] = nullptr;
    
    buildings.eraseObject(building);
    CCLOG("Tmx.buildings.size(%lu)",buildings.size());
}

void Tmx::showBattleResult()
{
    battleStatus = Finished;
    ui->showBattleResult(earnedBattleScoreByType);
}


UnitType Tmx::getSelectedUnit()
{
    return ui->selectedUnit;
}

void Tmx::decrementUnitCounter()
{
    auto unitType = getSelectedUnit();
    int remainCount = unitRemainedCounterByType.at(unitType) - 1;

    if (0 <= remainCount && battleStatus == Playing) {
        unitRemainedCounterByType[unitType] = remainCount;
        
        // @todo 長押しでつまることがあるので1秒に1回までに留める
        ui->updateUnitCounter(unitType, remainCount);
    }
}

bool Tmx::isRemainedUnitSelected()
{
    return 0 < unitRemainedCounterByType.at(getSelectedUnit());
}

void Tmx::increaseBattleScore(ScoreType type, float amount)
{
    if (0 < amount && battleStatus == Playing) {
        earnedBattleScoreByType[type] += amount;
        ui->updateBattleScore(type, earnedBattleScoreByType.at(type));
        
        remainingScoreByType[type] -= amount;
        ui->updateRemainingAssetScore(type, remainingScoreByType.at(type));
    }
}

BattleScoreByType* Tmx::getBattleScoreResult()
{
    return &earnedBattleScoreByType;
}


void Tmx::countUpRemainingScore(ScoreType type, float amount)
{
    if (0 < amount) {
        remainingScoreByType[type] += amount;
    }
}

void Tmx::incrementBuildingCount()
{
    ++fullBuildingCount;
}

void Tmx::decrementBuildingCount()
{
    --currentBuildingCount;

    float ratio = 100 - (float)currentBuildingCount / (float)fullBuildingCount * 100;
    earnedBattleScoreByType[DestructionRatioScore] = ratio;
//    CCLOG("currentBuildingCount(%i),fullBuildingCount(%i),ratio(%f)",currentBuildingCount,fullBuildingCount,ratio);
    if (battleStatus == Playing) {
        ui->updateDestructionRatio(ratio);
    }
}

void Tmx::initBattleControllerCounters()
{
    ui->updateRemainingAssetScore(ElixerScore, remainingScoreByType.at(ElixerScore));
    ui->updateRemainingAssetScore(CoinScore, remainingScoreByType.at(CoinScore));
    
    currentBuildingCount = fullBuildingCount;
}

