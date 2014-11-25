#include "BattleScene.h"
#include "Definitions.h"
#include "Unit.h"
#include "Building.h"

Scene* BattleScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = BattleScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
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
    
    tiledMap = TMXTiledMap::create("map_01.tmx");
    domainTMXLayer = tiledMap->getLayer("Domain");
    wallTMXLayer = tiledMap->getLayer("Wall");
    
    // @todo Not yet used
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("assets.plist");
    spriteBatch = SpriteBatchNode::create("assets.png");
    
    this->addBattleStage();
    this->addBuildings();
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
    Vec2 tileCoord = this->convertToCoord(touch->getLocation());
    if (isInMapRange(tileCoord) && 0 == wallTMXLayer->getTileGIDAt(tileCoord) /** @fixme not only wall **/) {
        auto unit = Unit::create(Unit::__TYPE::Wallbreaker);
        auto goalCoord = this->findCoord(tileCoord);
        auto mapNavigator = MapNavigator::create(tiledMap);
        auto path = mapNavigator->navigate(tileCoord, goalCoord);
        
        if (path->empty()) {CCLOG("HMM... EMPTY PATH IS DETECTED");return;};
        Vector<FiniteTimeAction*> arrayOfactions;
        while (!path->empty()) {
            auto node = path->top();
            auto directionPoint = this->convertToTile(node);
            auto move = MoveTo::create(0.5, directionPoint);
            arrayOfactions.pushBack(move);
            path->pop();
        }
        auto seq = Sequence::create(arrayOfactions);
        unit->unitNode->setPosition(domainTMXLayer->convertToNodeSpace(touch->getLocation()));
        unit->unitNode->runAction(seq);
        
        tiledMapLayer->addChild(unit->unitNode);
    }
    return;
}

inline Vec2 BattleScene::findCoord(Vec2 pos)
{
    auto goalCoord = Vec2(20,22);
    return goalCoord;
}

inline Vec2 BattleScene::convertToCoord(Vec2 pos)
{
    Size tileSize = tiledMap->getTileSize();
    Point deployedPoint = domainTMXLayer->convertToNodeSpace(pos);
    Size domainSize = domainTMXLayer->getContentSize();
    
    float slopeFormula = tileSize.height / tileSize.width;
    float tileX = floor((slopeFormula * deployedPoint.x + domainSize.height * 0.5 - deployedPoint.y) / tileSize.height);
    float tileY = floor((-slopeFormula * deployedPoint.x + domainSize.height * 1.5 - deployedPoint.y) / tileSize.height);
    return Vec2(tileX, tileY);
}

inline Vec2 BattleScene::convertToTile(Vec2 pos)
{
    return tiledMap->getLayer("Domain")->getPositionAt(pos) + tiledMap->getTileSize() * 0.5;
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
//    tiledMap->setVisible(false);
    tiledMapLayer->addChild(tiledMap);
    tiledMapLayer->addChild(spriteBatch);
    tiledMapLayer->setContentSize(domainTMXLayer->getContentSize());
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
    
    domainTMXLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listner, domainTMXLayer);
}

/**
 @fixme ZORDER
  {"TownHall", "ElixerTank", "GoldBank", "Canon", "TrenchMortar", "ArcherTower", "Wall"};
 */
void BattleScene::addBuildings()
{
    for (int x = 0; x < WORLD_MAP_WIDTH; ++x) {
        for (int y = 0; y < WORLD_MAP_HEIGHT; ++y) {
            
            auto coord = Vec2(x,y);
            auto coordPos = this->convertToTile(coord);
            
            auto westBrock = coord + Vec2(-1,0);
            auto northBrock = coord + Vec2(0,1);
            auto wnBrock = coord + Vec2(-1,1);
            
            auto wwBrock = coord + Vec2(-2, 0);
            auto wwnnBrock = coord + Vec2(-2, 2);
            auto nnBrock = coord + Vec2(0, 2);
            
            if (this->isTargetLayer("Wall", coord)) {
                auto* filename = String::create("stage/wall/");
                if (this->isTargetLayer("Wall", westBrock) && this->isTargetLayer("Wall", northBrock)) {
                    filename->append("1027.0.png");
                } else if (this->isTargetLayer("Wall", westBrock)) {
                    filename->append("1029.0.png");
                } else if (this->isTargetLayer("Wall", northBrock)) {
                    filename->append("1028.0.png");
                } else {
                    filename->append("1030.0.png");
                }
                Sprite* sprite = CCSprite::createWithSpriteFrameName(filename->getCString());
                sprite->setPosition(coordPos);
                spriteBatch->addChild(sprite);
            } else if (this->isTargetLayer("ArcherTower", coord)
                       && this->isTargetLayer("ArcherTower", westBrock)
                       && this->isTargetLayer("ArcherTower", northBrock)
                       && this->isTargetLayer("ArcherTower", wnBrock)) {
                Sprite* sprite = CCSprite::createWithSpriteFrameName("stage/archer_tower/1036.0.png");
                sprite->setPosition(coordPos.x - tiledMap->getTileSize().width * 0.5, coordPos.y + tiledMap->getTileSize().height * 0.5);
                spriteBatch->addChild(sprite);
            } else if (this->isTargetLayer("TrenchMortar", coord)
                       && this->isTargetLayer("TrenchMortar", westBrock)
                       && this->isTargetLayer("TrenchMortar", northBrock)
                       && this->isTargetLayer("TrenchMortar", wnBrock)) {
                auto mortar = CSLoader::createNode("CocosProject/res/TrenchMortar.csb");
                auto action = timeline::ActionTimelineCache::createAction("CocosProject/res/TrenchMortar.csb");
                mortar->runAction(action);
                action->gotoFrameAndPlay(0, true);
                mortar->setPosition(coordPos.x - tiledMap->getTileSize().width * 0.5, coordPos.y);
                tiledMapLayer->addChild(mortar); // @fixme ZORDER
            } else if (this->isTargetLayer("TownHall", coord)
                       && this->isTargetLayer("TownHall", wwBrock)
                       && this->isTargetLayer("TownHall", wwnnBrock)
                       && this->isTargetLayer("TownHall", nnBrock)) {
                auto hall = CSLoader::createNode("CocosProject/res/TownHall.csb");
                hall->setPosition(coordPos.x - tiledMap->getTileSize().width, coordPos.y  - tiledMap->getTileSize().height * 0.5);
                tiledMapLayer->addChild(hall); // @fixme ZORDER
            } else if (this->isTargetLayer("Canon", coord)
                       && this->isTargetLayer("Canon", westBrock)
                       && this->isTargetLayer("Canon", northBrock)
                       && this->isTargetLayer("Canon", wnBrock)) {
                auto canon = CSLoader::createNode("CocosProject/res/Canon.csb");
                auto action = timeline::ActionTimelineCache::createAction("CocosProject/res/Canon.csb");
                canon->runAction(action);
                action->gotoFrameAndPlay(0, true);
                canon->setRotation(90);
                canon->setPosition(coordPos.x - tiledMap->getTileSize().width * 0.5, coordPos.y);
                tiledMapLayer->addChild(canon); // @fixme ZORDER
            } else if (this->isTargetLayer("GoldBank", coord)
                       && this->isTargetLayer("GoldBank", westBrock)
                       && this->isTargetLayer("GoldBank", northBrock)
                       && this->isTargetLayer("GoldBank", wnBrock)) {
                auto bank = CSLoader::createNode("CocosProject/res/GoldBank.csb");
                bank->setRotation(90);
                bank->setScale(0.75);
                bank->setPosition(coordPos.x - tiledMap->getTileSize().width * 0.5, coordPos.y);
                tiledMapLayer->addChild(bank); // @fixme ZORDER
            } else if (this->isTargetLayer("ElixerTank", coord)
                       && this->isTargetLayer("ElixerTank", westBrock)
                       && this->isTargetLayer("ElixerTank", northBrock)
                       && this->isTargetLayer("ElixerTank", wnBrock)) {
                auto tank = CSLoader::createNode("CocosProject/res/ElixerTank.csb");
                tank->setPosition(coordPos.x - tiledMap->getTileSize().width * 0.5, coordPos.y);
                tank->setScale(0.75);
                tiledMapLayer->addChild(tank); // @fixme ZORDER
            }
        }
    }
}

inline bool BattleScene::isTargetLayer(std::string name, Vec2 coord)
{
    return (isInMapRange(coord) && 0 != tiledMap->getLayer(name.c_str())->getTileGIDAt(coord));
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
