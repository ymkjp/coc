#include "BattleScene.h"

Scene* BattleScene::createScene(Stages stage)
{
    auto scene = Scene::create();
    auto layer = BattleScene::create(stage);
    scene->addChild(layer);
    
    return scene;
}

bool BattleScene::init(Stages stage)
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    scrollView = ScrollView::create(visibleSize);
    backgroundLayer = Layer::create();
    tiledMapLayer = Layer::create();
    
    // ステージ関連のTmxクラス
    tmx = Tmx::create(stage);
    this->addChild(tmx,TmxOrder);
    
    // 画像関連のキャッシュ
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("assets.plist");
    
    // 各 Building, Unit をつくってくれるNodeFactoryクラス
    nodeFactory = NodeFactory::create(tmx);
    nodeFactory->retain();
    
    this->addBattleStage();
    this->initBuildings();
    nodeFactory->updateAttackRangeGrid();
    
    this->addEventDispacher(); // ディスパッチャー外すの忘れそう

    return true;
}

void  BattleScene::deployUnit()
{
    Vec2 tileCoord = tmx->convertToCoord(targetTouch->getLocation());
    if (tmx->isRemainedUnitSelected()
        && isInMapRange(tileCoord)
        && 0 == tmx->tiledMap->getLayer("Domain")->getTileGIDAt(tileCoord)) {
        
        // ユニット残数をデクリメントしてユニット生成
        tmx->decrementUnitCounter();
        auto unit = nodeFactory->createUnit(tmx->getSelectedUnit(), tileCoord);
        unit->setPosition(tmx->domainTMXLayer->convertToNodeSpace(targetTouch->getLocation()));
        tmx->units.pushBack(unit);
        tiledMapLayer->addChild(unit,UnitOrder);
    }
}

void BattleScene::deployUnitIfKeptTouching(float frame)
{
    deployUnit();
}


bool BattleScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    // 長押ししてたらデプロイし続ける
//    targetTouch = touch;
//    this->schedule(schedule_selector(BattleScene::deployUnitIfKeptTouching), 0.16);
    return true;
}

void BattleScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    this->unschedule(schedule_selector(BattleScene::deployUnitIfKeptTouching));
    backgroundLayer->setPosition(backgroundLayer->getPosition() + touch->getDelta());
}

void BattleScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    if (scrollStatus.scrollingDelay) {
        return;
    }
    this->unschedule(schedule_selector(BattleScene::deployUnitIfKeptTouching));
    targetTouch = touch;
    this->deployUnit();
    return;
}

void BattleScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    CCLOG("4. cancelled");
}

void BattleScene::addBattleStage()
{
    scrollView->setBounceable(false);
    auto spriteRight = Sprite::create("BackgroundStage1.png");
    auto spriteLeft = Sprite::create("BackgroundStage1.png");
    spriteLeft->setFlippedX(true);
    
    spriteLeft->setPosition(Vec2(origin.x + spriteLeft->getContentSize().width * 0.5 * 1, origin.y + spriteLeft->getContentSize().height * 0.5));
    spriteRight->setPosition(Vec2(origin.x + spriteRight->getContentSize().width * 0.5 * 3, origin.y + spriteRight->getContentSize().height * 0.5));
    backgroundLayer->addChild(spriteRight);
    backgroundLayer->addChild(spriteLeft);
    
    // スクロールビューの中身を真ん中に持っていく
    backgroundLayer->setPositionX(origin.x - spriteRight->getContentSize().width * 0.1);
    backgroundLayer->setPositionY(origin.y - spriteRight->getContentSize().height * 0.1);
    backgroundLayer->setScale(DEFAULT_ZOOM_RATE);
    
    // Adjust tiledMap to the background
    auto tiledMapPosition = Vec2(origin.x + spriteRight->getContentSize().width * 0.05, origin.y + spriteRight->getContentSize().height * 0.1);
    tiledMapLayer->addChild(tmx->tiledMap);
//    tiledMapLayer->addChild(spriteBatch);
    tiledMapLayer->setContentSize(tmx->domainTMXLayer->getContentSize());
    tiledMapLayer->setPosition(tiledMapPosition);
    backgroundLayer->addChild(tiledMapLayer);
    
    scrollView->setPosition(origin);
    scrollView->setContainer(backgroundLayer);
    scrollView->setContentSize(Size(spriteRight->getContentSize().width * 2, spriteRight->getContentSize().height));
    scrollView->setDelegate(this);
    scrollView->setMaxScale(MAX_ZOOM_RATE);
    scrollView->setMinScale(visibleSize.width / spriteRight->getContentSize().width * 0.5);
}


inline bool BattleScene::isTargetLayer(std::string name, Vec2 coord)
{
    return (isInMapRange(coord) && 0 != tmx->tiledMap->getLayer(name.c_str())->getTileGIDAt(coord));
}

/**
 {"TownHall", "ElixerTank", "GoldBank", "Canon", "TrenchMortar", "ArcherTower", "Wall"};
 */
void BattleScene::initBuildings()
{
    for (int x = 0; x < WORLD_MAP_WIDTH; ++x) {
        for (int y = 0; y < WORLD_MAP_HEIGHT; ++y) {
            
            auto coord = Vec2(x,y);
            auto coordPos = tmx->convertToRealPos(coord);
            
            auto eastCoord = coord + Vec2(1,0);
            auto westCoord = coord + Vec2(-1,0);
            auto southCoord = coord + Vec2(0,1);
            auto northCoord = coord + Vec2(0,-1);
            auto esCoord = coord + Vec2(1,1);
            
            auto eeCoord = coord + Vec2(2, 0);
            auto eessCoord = coord + Vec2(2, 2);
            auto ssCoord = coord + Vec2(0, 2);
            
            // ユニットをデプロイできない赤いエリア
            if (this->isTargetLayer("Domain", coord)) {
                auto domainSprite = CCSprite::createWithSpriteFrameName("stage/field/14.png");
                tmx->domainCells.pushBack(domainSprite);
                domainSprite->setPosition(coordPos);
                domainSprite->setOpacity(20);
                domainSprite->setScale(0.96);
                tiledMapLayer->addChild(domainSprite,DomainOrder);
            }
            
            if (isTargetLayer("Wall", coord)) {
                addBuilding(Wall, coord, coordPos);
            } else if (isTargetLayer("ArcherTower", coord)
                       && isTargetLayer("ArcherTower", eastCoord)
                       && isTargetLayer("ArcherTower", southCoord)
                       && isTargetLayer("ArcherTower", esCoord)) {
                addGrass(coord);
                addBuilding(ArcherTower, coord, coordPos);
            } else if (isTargetLayer("TrenchMortar", coord)
                       && isTargetLayer("TrenchMortar", eastCoord)
                       && isTargetLayer("TrenchMortar", southCoord)
                       && isTargetLayer("TrenchMortar", esCoord)) {
                addGrass(coord);
                addBuilding(TrenchMortar, coord, Vec2(coordPos.x, coordPos.y - tmx->tiledMap->getTileSize().height * 0.5));
            } else if (isTargetLayer("TownHall", coord)
                       && isTargetLayer("TownHall", eeCoord)
                       && isTargetLayer("TownHall", eessCoord)
                       && isTargetLayer("TownHall", ssCoord)) {
                addGrass(coord);
                addBuilding(TownHall, coord, Vec2(coordPos.x, coordPos.y - tmx->tiledMap->getTileSize().height * 1.5));
            } else if (isTargetLayer("Canon", coord)
                       && isTargetLayer("Canon", eastCoord)
                       && isTargetLayer("Canon", southCoord)
                       && isTargetLayer("Canon", esCoord)) {
                addGrass(coord);
                addBuilding(Canon, coord, Vec2(coordPos.x, coordPos.y - tmx->tiledMap->getTileSize().height * 0.5));
            } else if (isTargetLayer("GoldBank", coord)
                       && isTargetLayer("GoldBank", eastCoord)
                       && isTargetLayer("GoldBank", southCoord)
                       && isTargetLayer("GoldBank", esCoord)) {
                addGrass(coord);
                addBuilding(GoldBank, coord, Vec2(coordPos.x, coordPos.y - tmx->tiledMap->getTileSize().height * 0.5));
            } else if (isTargetLayer("ElixerTank", coord)
                       && isTargetLayer("ElixerTank", eastCoord)
                       && isTargetLayer("ElixerTank", southCoord)
                       && isTargetLayer("ElixerTank", esCoord)) {
                addGrass(coord);
                addBuilding(ElixerTank, coord, coordPos);
            }
        }
    }
}

inline void BattleScene::addBuilding(BuildingType type, Vec2 coord, Vec2 pos)
{
    auto building = nodeFactory->createBuilding(type, coord);
    tmx->buildings.pushBack(building);
    tmx->buildingGrid[coord.x][coord.y] = building;
    tmx->buildingCoords[type].push_back(coord);
    building->setPosition(pos);
    tiledMapLayer->addChild(building,BuildingOrder);
}

inline void BattleScene::addGrass(Vec2 coord)
{
    auto grassSprite = CCSprite::createWithSpriteFrameName("stage/field/256.0.png");
    tmx->grassCells.pushBack(grassSprite);
    grassSprite->setPosition(tmx->convertToRealPos(coord));
    grassSprite->setScale(1.6);
    tiledMapLayer->addChild(grassSprite,GrassOrder);
}

void BattleScene::addEventDispacher()
{
    auto scrollViewListner = EventListenerTouchOneByOne::create();
    scrollViewListner->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
        return true;
    };
    scrollViewListner->onTouchEnded = [this](Touch* touch, Event* event) -> void {
        this->runAction(Sequence::create(DelayTime::create(0.001),CallFunc::create([this](){
            this->scrollStatus.scrollingDelay = false;
        }), nullptr));
        return;
    };
    scrollView->getEventDispatcher()->addEventListenerWithSceneGraphPriority(scrollViewListner, scrollView);
    this->addChild(scrollView);
    
    auto listner = EventListenerTouchOneByOne::create();
//    listner->setSwallowTouches(true);
    listner->onTouchBegan = CC_CALLBACK_2(BattleScene::onTouchBegan, this);
    listner->onTouchMoved = CC_CALLBACK_2(BattleScene::onTouchMoved, this);
    listner->onTouchEnded = CC_CALLBACK_2(BattleScene::onTouchEnded, this);
    listner->onTouchCancelled = CC_CALLBACK_2(BattleScene::onTouchCancelled, this);
    
    tmx->domainTMXLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listner, tmx->domainTMXLayer);
}

void BattleScene::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent)
{
    CCLOG("11. Touches Began");
}

void BattleScene::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches,cocos2d::Event *pEvent)
{
    CCLOG("11. Touches Moved");
}

void BattleScene::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent)
{
    CCLOG("11. Touches Ended");
}

void BattleScene::onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent)
{
    CCLOG("11. Touches Cancelled");
}

void BattleScene::scrollViewDidScroll(ScrollView *view)
{
    scrollStatus.scrollingDelay = true;
}

void BattleScene::scrollViewDidZoom(ScrollView *view)
{
}
