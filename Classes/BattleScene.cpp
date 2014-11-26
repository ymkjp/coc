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
    tmx->retain();
//
//    tiledMap = TMXTiledMap::create("map_01.tmx");
//    domainTMXLayer = tiledMap->getLayer("Domain");
//    wallTMXLayer = tiledMap->getLayer("Wall");
    
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
        auto unit = Unit::create(tmx, Unit::__TYPE::Barbarian, tileCoord);
        unit->unitNode->setPosition(tmx->domainTMXLayer->convertToNodeSpace(touch->getLocation()));
        tiledMapLayer->addChild(unit->unitNode,1,"unit");
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
//    tiledMap->setVisible(false); // XXX
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
                this->addToBuildingCache(Building::Wall, coord);
                auto* filename = String::create("stage/wall/");
                if (this->isTargetLayer("Wall", westCoord) && this->isTargetLayer("Wall", northCoord)) {
                    filename->append("1027.0.png");
                } else if (this->isTargetLayer("Wall", westCoord)) {
                    filename->append("1029.0.png");
                } else if (this->isTargetLayer("Wall", northCoord)) {
                    filename->append("1028.0.png");
                } else {
                    filename->append("1030.0.png");
                }
                Sprite* sprite = CCSprite::createWithSpriteFrameName(filename->getCString());
                sprite->setPosition(coordPos);
                spriteBatch->addChild(sprite);
            } else if (this->isTargetLayer("ArcherTower", coord)
                       && this->isTargetLayer("ArcherTower", eastCoord)
                       && this->isTargetLayer("ArcherTower", southCoord)
                       && this->isTargetLayer("ArcherTower", esCoord)) {
                this->addToBuildingCache(Building::ArcherTower, coord);
                Sprite* sprite = CCSprite::createWithSpriteFrameName("stage/archer_tower/1036.0.png");
                sprite->setPosition(coordPos);
                spriteBatch->addChild(sprite);
            } else if (this->isTargetLayer("TrenchMortar", coord)
                       && this->isTargetLayer("TrenchMortar", eastCoord)
                       && this->isTargetLayer("TrenchMortar", southCoord)
                       && this->isTargetLayer("TrenchMortar", esCoord)) {
                this->addToBuildingCache(Building::TrenchMortar, coord);
                auto mortar = CSLoader::createNode("CocosProject/res/TrenchMortar.csb");
                auto action = timeline::ActionTimelineCache::createAction("CocosProject/res/TrenchMortar.csb");
                mortar->runAction(action);
                action->gotoFrameAndPlay(0, true);
                mortar->setPosition(coordPos.x, coordPos.y - tmx->tiledMap->getTileSize().height * 0.5);
                tiledMapLayer->addChild(mortar);
            } else if (this->isTargetLayer("TownHall", coord)
                       && this->isTargetLayer("TownHall", eeCoord)
                       && this->isTargetLayer("TownHall", eessCoord)
                       && this->isTargetLayer("TownHall", ssCoord)) {
                this->addToBuildingCache(Building::TownHall, coord);
                auto hall = CSLoader::createNode("CocosProject/res/TownHall.csb");
                hall->setPosition(coordPos.x, coordPos.y - tmx->tiledMap->getTileSize().height * 1.5);
                tiledMapLayer->addChild(hall);
            } else if (this->isTargetLayer("Canon", coord)
                       && this->isTargetLayer("Canon", eastCoord)
                       && this->isTargetLayer("Canon", southCoord)
                       && this->isTargetLayer("Canon", esCoord)) {
                this->addToBuildingCache(Building::Canon, coord);
                auto canon = CSLoader::createNode("CocosProject/res/Canon.csb");
                auto action = timeline::ActionTimelineCache::createAction("CocosProject/res/Canon.csb");
                canon->runAction(action);
                action->gotoFrameAndPlay(0, true);
                canon->setPosition(coordPos.x, coordPos.y - tmx->tiledMap->getTileSize().height * 0.5);
                tiledMapLayer->addChild(canon);
            } else if (this->isTargetLayer("GoldBank", coord)
                       && this->isTargetLayer("GoldBank", eastCoord)
                       && this->isTargetLayer("GoldBank", southCoord)
                       && this->isTargetLayer("GoldBank", esCoord)) {
                this->addToBuildingCache(Building::GoldBank, coord);
                auto bank = CSLoader::createNode("CocosProject/res/GoldBank.csb");
                bank->setScale(0.75);
                bank->setPosition(coordPos.x, coordPos.y - tmx->tiledMap->getTileSize().height * 0.5);
                tiledMapLayer->addChild(bank);
            } else if (this->isTargetLayer("ElixerTank", coord)
                       && this->isTargetLayer("ElixerTank", eastCoord)
                       && this->isTargetLayer("ElixerTank", southCoord)
                       && this->isTargetLayer("ElixerTank", esCoord)) {
                this->addToBuildingCache(Building::ElixerTank, coord);
                auto tank = CSLoader::createNode("CocosProject/res/ElixerTank.csb");
                tank->setPosition(coordPos);
                tank->setScale(0.75);
                tiledMapLayer->addChild(tank);
            }
        }
    }
}

inline void BattleScene::addToBuildingCache(Building::__TYPE type, Vec2 coord)
{
    auto building = Building::create(type, coord);
    building->retain();
    tmx->buildingGrid[coord.x][coord.y] = building;
    tmx->buildingCoords[type].push_back(coord);
}

inline bool BattleScene::isTargetLayer(std::string name, Vec2 coord)
{
    return (isInMapRange(coord) && 0 != tmx->tiledMap->getLayer(name.c_str())->getTileGIDAt(coord));
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
