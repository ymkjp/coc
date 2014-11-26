#include "BattleScene.h"
#include "MapNavigator.h"

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
    
    tiledMap = TMXTiledMap::create("map_01.tmx");
    domainTMXLayer = tiledMap->getLayer("Domain");
    wallTMXLayer = tiledMap->getLayer("Wall");
    
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
    Vec2 tileCoord = this->convertToCoord(touch->getLocation());
    if (isInMapRange(tileCoord) && 0 == wallTMXLayer->getTileGIDAt(tileCoord) /** @fixme not only wall **/) {
        auto unit = Unit::create(Unit::__TYPE::Barbarian);
        unit->retain();
        auto goalCoord = this->findGoalCoord(tileCoord, Building::Resources);
        auto mapNavigator = MapNavigator::create(tiledMap);
        auto path = mapNavigator->navigate(tileCoord, goalCoord);
        
        if (path->empty()) {CCLOG("HMM... EMPTY PATH IS DETECTED");return;};
        
        Vec2 nextCoord;
        Vec2 prevCoord;
        Vec2 directionPoint;
        Vector<FiniteTimeAction*> arrayOfactions;
        MoveTo* moveAction;
        while (!path->empty()) {
            nextCoord = path->top();
            directionPoint = this->convertToTile(nextCoord);
            moveAction = MoveTo::create(0.5, directionPoint);
            auto posDiff =  Vec2((int)prevCoord.x - (int)nextCoord.x, (int)prevCoord.y - (int)nextCoord.y);
//            Node* animatedNode = unit->createAnimatedNode(posDiff);
            //            FiniteTimeAction* func = CallFunc::create(CC_CALLBACK_0(Unit::animateNode, unit));
//            FiniteTimeAction* func = CallFuncN::create([this,&animatedNode](Ref* target) {
//                auto node = tiledMapLayer->getChildByName("unit");
//                CCLOG("[node:%s]",node->getName().c_str());
////                unit->unitNode = animatedNode;
//                tiledMapLayer->removeChildByName("unit");
//                tiledMapLayer->addChild(animatedNode,1,"unit");
//                // @todo
//            });
//            arrayOfactions.pushBack(func);
            
            arrayOfactions.pushBack(moveAction);
            path->pop();
            prevCoord = nextCoord;
        }
//        FiniteTimeAction* attack = CallFunc::create(CC_CALLBACK_0(BattleScene::attack, this));
        FiniteTimeAction* attack = CallFunc::create(CC_CALLBACK_0(Unit::attack, unit));
        arrayOfactions.pushBack(attack);
        auto seq = Sequence::create(arrayOfactions);
        unit->unitNode->setPosition(domainTMXLayer->convertToNodeSpace(touch->getLocation()));
        unit->unitNode->runAction(seq);
        
        tiledMapLayer->addChild(unit->unitNode,1,"unit");
    }
    return;
}

void BattleScene::attack()
{
    auto node = tiledMapLayer->getChildByName("unit");
    CCLOG("node:%s",node->getName().c_str());
}

/**
 @todo Highlight the nearest coord
 */
inline Vec2 BattleScene::findGoalCoord(Vec2 startCoord, Building::__CATEGORY targetCategory)
{
    // 1. ターゲットのマスを経路探索で近いものから算出
    // 2.　ターゲット4マスの周囲12マスに経路探索をかけて最もコストの低かったマスを goalCoord とする
    // 2'. ターゲット9マスの周囲16マスに経路探索をかけて最もコストの低かったマスを goalCoord とする
    // 3. 経路探索でゴールにたどり着かなかった場合、単純移動距離の短い壁を goalCoord とする // todo

    auto types = Building::getTypesByCategory(targetCategory);
    
    std::vector<Vec2> targetCoords;
    for (auto type: types) {
        targetCoords.insert(targetCoords.end(), buildingCoords[type].begin(), buildingCoords[type].end());
    }
    
    auto navi = MapNavigator::create(tiledMap);
    
    Vec2 nearestCoord = Vec2(-1,-1);
    float nearestDistance;
    float distance;
    for (auto coord: targetCoords) {
        distance = startCoord.getDistanceSq(coord);
        if (nearestCoord == Vec2(-1,-1) ||  distance < nearestDistance) {
            nearestDistance = distance;
            nearestCoord = coord;
        }
    }
    auto space = buildingGrid.at(nearestCoord.x).at(nearestCoord.y)->getSpace();
    auto coordsSurround = Building::coordsSurround.at(space);
    int bestScore = -1;
    Vec2 bestCoord;
    float lastNodeScore;
    for (auto coord: coordsSurround) {
        lastNodeScore = navi->findLastNode(startCoord, nearestCoord + coord)->GetScore();
        if (bestScore == -1 || lastNodeScore < bestScore) {
            bestScore = lastNodeScore;
            bestCoord = nearestCoord + coord;
        }
    }
    return bestCoord;
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
//    tiledMap->setVisible(false); // XXX
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
 @fixme too fast animation
  {"TownHall", "ElixerTank", "GoldBank", "Canon", "TrenchMortar", "ArcherTower", "Wall"};
 */
void BattleScene::initBuildings()
{
    for (int x = 0; x < WORLD_MAP_WIDTH; ++x) {
        for (int y = 0; y < WORLD_MAP_HEIGHT; ++y) {
            
            auto coord = Vec2(x,y);
            auto coordPos = this->convertToTile(coord);
            
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
                mortar->setPosition(coordPos.x, coordPos.y - tiledMap->getTileSize().height * 0.5);
                tiledMapLayer->addChild(mortar);
            } else if (this->isTargetLayer("TownHall", coord)
                       && this->isTargetLayer("TownHall", eeCoord)
                       && this->isTargetLayer("TownHall", eessCoord)
                       && this->isTargetLayer("TownHall", ssCoord)) {
                this->addToBuildingCache(Building::TownHall, coord);
                auto hall = CSLoader::createNode("CocosProject/res/TownHall.csb");
                hall->setPosition(coordPos.x, coordPos.y - tiledMap->getTileSize().height * 1.5);
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
                canon->setPosition(coordPos.x, coordPos.y - tiledMap->getTileSize().height * 0.5);
                tiledMapLayer->addChild(canon);
            } else if (this->isTargetLayer("GoldBank", coord)
                       && this->isTargetLayer("GoldBank", eastCoord)
                       && this->isTargetLayer("GoldBank", southCoord)
                       && this->isTargetLayer("GoldBank", esCoord)) {
                this->addToBuildingCache(Building::GoldBank, coord);
                auto bank = CSLoader::createNode("CocosProject/res/GoldBank.csb");
                bank->setScale(0.75);
                bank->setPosition(coordPos.x, coordPos.y - tiledMap->getTileSize().height * 0.5);
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
    buildingGrid[coord.x][coord.y] = building;
    buildingCoords[type].push_back(coord);
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
