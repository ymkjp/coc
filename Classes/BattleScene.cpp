#include "BattleScene.h"

Scene* BattleScene::createScene()
{
    auto scene = Scene::create();
    auto layer = BattleScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool BattleScene::init()
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
    
    tmx = Tmx::create();
    tmx->retain(); // tmx inherits Ref* not Node*
    
    // @todo Not yet used
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("assets.plist");
    spriteBatch = SpriteBatchNode::create("assets.png");
    
    this->addBattleStage();
    this->initBuildings();
    this->addEventDispacher();
//    this->addUILayer();
    
    return true;
}

void BattleScene::addUILayer()
{
    Node *uiHeaderNode = CSLoader::getInstance()->createNodeFromXML("CocosProject/cocosstudio/UILayer_02.csd");
    uiHeaderNode->setScale(visibleSize.width / uiHeaderNode->getContentSize().width);
    this->addChild(uiHeaderNode);
    Node *uiButtomNode = CSLoader::getInstance()->createNodeFromXML("CocosProject/cocosstudio/UILayer_01.csd");
    uiButtomNode->setScale(visibleSize.width / uiButtomNode->getContentSize().width);
    this->addChild(uiButtomNode);
}

bool BattleScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    return true;
}

void BattleScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    backgroundLayer->setPosition(backgroundLayer->getPosition() + touch->getDelta());
}

void BattleScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    if (scrollStatus.scrollingDelay) {
        return;
    }
    Vec2 tileCoord = tmx->convertToCoord(touch->getLocation());
    if (isInMapRange(tileCoord) && 0 == tmx->wallTMXLayer->getTileGIDAt(tileCoord) /** @fixme not only wall **/) {
//        auto unit = Unit::create(tmx, Barbarian, tileCoord);
        auto unit = Unit::create(tmx, Wallbreaker, tileCoord);

        unit->unitNode->setPosition(tmx->domainTMXLayer->convertToNodeSpace(touch->getLocation()));
        units.pushBack(unit);
        tiledMapLayer->addChild(unit);
    }
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
    backgroundLayer->setPosition(origin);
    backgroundLayer->setScale(DEFAULT_ZOOM_RATE);
    
    // Adjust tiledMap to the background
    auto tiledMapPosition = Vec2(origin.x + spriteRight->getContentSize().width * 0.05, origin.y + spriteRight->getContentSize().height * 0.1);
    tmx->tiledMap->setVisible(TILEDMAP_VISIBLE_ON);
    tiledMapLayer->addChild(tmx->tiledMap);
    tiledMapLayer->addChild(spriteBatch);
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
 @fixme ZORDER
 @fixme too fast animation
 {"TownHall", "ElixerTank", "GoldBank", "Canon", "TrenchMortar", "ArcherTower", "Wall"};
 */
void BattleScene::initBuildings()
{
    for (int x = 0; x < WORLD_MAP_WIDTH; ++x) {
        for (int y = 0; y < WORLD_MAP_HEIGHT; ++y) {
            
            auto coord = Vec2(x,y);
            auto coordPos = tmx->convertToTile(coord);
            
            auto eastCoord = coord + Vec2(1,0);
            auto westCoord = coord + Vec2(-1,0);
            auto southCoord = coord + Vec2(0,1);
            auto northCoord = coord + Vec2(0,-1);
            auto esCoord = coord + Vec2(1,1);
            
            auto eeCoord = coord + Vec2(2, 0);
            auto eessCoord = coord + Vec2(2, 2);
            auto ssCoord = coord + Vec2(0, 2);
            
            if (this->isTargetLayer("Wall", coord)) {
                this->addBuilding(Wall, coord, coordPos);
            } else if (this->isTargetLayer("ArcherTower", coord)
                       && this->isTargetLayer("ArcherTower", eastCoord)
                       && this->isTargetLayer("ArcherTower", southCoord)
                       && this->isTargetLayer("ArcherTower", esCoord)) {
                this->addBuilding(ArcherTower, coord, coordPos);
            } else if (this->isTargetLayer("TrenchMortar", coord)
                       && this->isTargetLayer("TrenchMortar", eastCoord)
                       && this->isTargetLayer("TrenchMortar", southCoord)
                       && this->isTargetLayer("TrenchMortar", esCoord)) {
                this->addBuilding(TrenchMortar, coord, Vec2(coordPos.x, coordPos.y - tmx->tiledMap->getTileSize().height * 0.5));
            } else if (this->isTargetLayer("TownHall", coord)
                       && this->isTargetLayer("TownHall", eeCoord)
                       && this->isTargetLayer("TownHall", eessCoord)
                       && this->isTargetLayer("TownHall", ssCoord)) {
                this->addBuilding(TownHall, coord, Vec2(coordPos.x, coordPos.y - tmx->tiledMap->getTileSize().height * 1.5));
            } else if (this->isTargetLayer("Canon", coord)
                       && this->isTargetLayer("Canon", eastCoord)
                       && this->isTargetLayer("Canon", southCoord)
                       && this->isTargetLayer("Canon", esCoord)) {
                this->addBuilding(Canon, coord, Vec2(coordPos.x, coordPos.y - tmx->tiledMap->getTileSize().height * 0.5));
            } else if (this->isTargetLayer("GoldBank", coord)
                       && this->isTargetLayer("GoldBank", eastCoord)
                       && this->isTargetLayer("GoldBank", southCoord)
                       && this->isTargetLayer("GoldBank", esCoord)) {
                this->addBuilding(GoldBank, coord, Vec2(coordPos.x, coordPos.y - tmx->tiledMap->getTileSize().height * 0.5));
            } else if (this->isTargetLayer("ElixerTank", coord)
                       && this->isTargetLayer("ElixerTank", eastCoord)
                       && this->isTargetLayer("ElixerTank", southCoord)
                       && this->isTargetLayer("ElixerTank", esCoord)) {
                this->addBuilding(ElixerTank, coord, coordPos);
            }
        }
    }
}

inline void BattleScene::addBuilding(BuildingType type, Vec2 coord, Vec2 pos)
{
    auto building = Building::create(tmx, type, coord);
    buildings.pushBack(building);
    tmx->buildingGrid[coord.x][coord.y] = building;
    tmx->buildingCoords[type].push_back(coord);
    building->buildingNode->setPosition(pos);
    tiledMapLayer->addChild(building);
}

void BattleScene::addEventDispacher()
{
    auto scrollViewListner = EventListenerTouchOneByOne::create();
    scrollViewListner->setSwallowTouches(true);
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
