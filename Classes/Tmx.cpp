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
    
    // 経路探索マップナビゲータの初期化
    mapNavigator = MapNavigator::create();
    mapNavigator->retain();
    
    actionTimelineCache = timeline::ActionTimelineCache::getInstance();

//    SimpleAudioEngine::getInstance()->playEffect("planning.mp3", false, 1.0f, 0.0f, 1.0f);
    
    SimpleAudioEngine::getInstance()->setEffectsVolume(0.8);
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.8);
//    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("planning.mp3");
//    SimpleAudioEngine::getInstance()->playBackgroundMusic("planning.mp3",true);
//
    audioManager = AudioManager::getInstance();
    audioManager->playBgm("planning",true);

    return true;
}

void Tmx::startBattle()
{
    battleStatus = Playing;
    audioManager->playBgm("combat_music", true);

    scheduleOnce(schedule_selector(Tmx::hideMessage), 1.0);
    schedule(schedule_selector(Tmx::decreaseTimerCount), 1.0f);
    schedule(schedule_selector(Tmx::detectUnitAnnihilation), 2.0f);
}

// "You cannot deploy troops on the red area!" とか
void Tmx::showWarning(std::string warningMessage)
{
    ui->showWarning(warningMessage);
}

void Tmx::hideMessage(float frame)
{
    ui->hideMessage();
}

void Tmx::decreaseTimerCount(float frame)
{
    if (battleStatus == Playing) {
        --currentBattleSecound;
        ui->updateTimer(currentBattleSecound);
    }
    
    if (currentBattleSecound < 0) {
        showBattleResult();
    }
}


void Tmx::detectUnitAnnihilation(float frame)
{
    int remainTypeCount = 0;
    for (auto remained: unitRemainedCounterByType) {
        if (remained.second != 0) {
            ++remainTypeCount;
        }
    }
    if (0 < remainTypeCount) {
        return;
    }
    
    int aliveCount = 0;
    for (auto unit: units) {
        if (unit->status == Unit::Alive)
        {
            ++aliveCount;
        }
    }
    if (0 < aliveCount) {
        return;
    }

    if (remainTypeCount <= 0 && aliveCount <= 0) {
        // CCLOG("[units]size(%lu),alive(%i),remainTypeCount(%i)",units.size(),aliveCount,remainTypeCount);
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
    // 施設タイプごとにキャッシュしているものを破棄
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
    
    // 施設が建っているcoordを管理しているキャッシュを破棄
    buildingGrid[building->coord.x][building->coord.y] = nullptr;
    
    // 経路のグラフに入っている建物を抜く
    worldGrid.at(static_cast<int>(building->coord.x)).at(static_cast<int>(building->coord.y)) = {};
    
    // 経路のキャッシュをすべて破棄
    mapNavigator->pathCache.clear();
    
    // 施設のメモリ確保している Vector から削除
    buildings.eraseObject(building);
//    CCLOG("Tmx.buildings.size(%lu)",buildings.size());
}

bool Tmx::isTravelable(float posX, float posY)
{
    Building* building = buildingGrid.at(posX).at(posY);
    return building == nullptr || building->type != Wall;
}

PathToGoal Tmx::navigate(Vec2 startCoord, Vec2 goalCoord)
{
    if (!worldGridInitialized)
    {
        // 建物の初期化後にグリッドを生成する必要があるためここでイニシャライズしている
        initWorldGrid();
        worldGridInitialized = true;
    }
    return mapNavigator->navigate(&worldGrid, startCoord, goalCoord);
}

void Tmx::initWorldGrid()
{
    for (int x = 0; x < worldGrid.size(); ++x) {
        for (int y = 0; y < worldGrid[x].size(); ++y) {
            AStar plainNode = {};
            plainNode.status = (this->isTravelable(x,y)) ? AStar::__STATUS::NONE : AStar::__STATUS::UNABLE;
            plainNode.pos = Vec2(x,y);
            worldGrid[x][y] = plainNode;
        }
    }
}

void Tmx::showBattleResult()
{
    if (battleStatus == Finished) {
        return;
    }
    battleStatus = Finished;
    scheduleOnce(schedule_selector(Tmx::killAllUnits), 1.0);
    
    float percentage = earnedBattleScoreByType.at(DestructionRatioScore);
    bool isWon = (50 <= percentage);
    audioManager->playBgm(isWon ? "winwinwin" : "battle_lost_02", false);
    ui->showBattleResult(earnedBattleScoreByType);
}

void Tmx::killAllUnits(float frame)
{
    for (auto unit: units) {
        if (unit) {
            unit->finished();
        }
    }
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

    float ratio = 100 - ((float)currentBuildingCount / (float)fullBuildingCount * 100);
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

