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
    this->addUILayer();
    
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
    if (isInMapRange(tileCoord) && 0 == wallTMXLayer->getTileGIDAt(tileCoord)) {
        auto unit = Unit::create(tiledMap, Unit::__TYPE::Wallbreaker);
        unit->run(domainTMXLayer->convertToNodeSpace(touch->getLocation()), tileCoord);
        tiledMapLayer->addChild(unit->unitNode);
    }
    return;
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
    
    tiledMapLayer->addChild(tiledMap);
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
        this->runAction(Sequence::create(DelayTime::create(0.01),CallFunc::create([this](){
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

void BattleScene::addBuildings()
{
//    auto objectGroup = tiledMap->getObjectGroup("StageObject");
//    auto objects = objectGroup->getObjects();
//    if (objects.empty()) {
//        CCLOG("No Objects");
//        return;
//    }
//    
//    for (auto object : objects) {
//        ValueMap objectInfo = object.asValueMap();
//        if (objectInfo.find("name") == objectInfo.end()
//            || objectInfo.find("type") == objectInfo.end()
//            || objectInfo.find("gid") == objectInfo.end()
//            || objectInfo.find("x") == objectInfo.end()
//            || objectInfo.find("y") == objectInfo.end()
//            ) {
//            CCLOG("can not find necessary info");
//            continue;
//        }
//        auto name = objectInfo.at("name").asString();
//        auto type = objectInfo.at("type").asString();
//        
//        auto building = Building::create(tiledMap, objectInfo);
//        tiledMapLayer->addChild(building->sprite);
//        
//        for (auto info: objectInfo)
//        {
//            //                CCLOG("{%s:%s}",info.first.c_str(),info.second.asString().c_str());
//        }
//    }
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
