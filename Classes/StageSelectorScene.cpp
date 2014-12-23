#include "StageSelectorScene.h"
#include "BattleScene.h"
#include "AudioManager.h"

USING_NS_CC;

Scene* StageSelectorScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = StageSelectorScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StageSelectorScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // バイナリ版 (.csb) だとフォントが反映されないため、XML版 (.csd) を使って描画している
    auto ui = CSLoader::getInstance()->createNodeFromXML("cocosstudio/StageSelectionLayer.csd");
    ui->setScale(visibleSize.width / ui->getContentSize().width);
    ui->setPosition(origin);
    this->addChild(ui);
    
    auto multiPlaySprite = Sprite::create("multi_play.png");
    multiPlaySprite->setPosition(Vec2(visibleSize.width * 0.25 - 23, visibleSize.height * 0.5));
    multiPlaySprite->setScale(visibleSize.width / multiPlaySprite->getContentSize().width * 0.45);
    ui->addChild(multiPlaySprite);
    
    // ScrollView_1 -> Button_Amigo
    auto scrollView = ui->getChildByName("ScrollView_1");
    
    for (auto stage: stageNameByStages) {
        __String buttonName = "Button_";
        buttonName.append(stage.second);
        auto btn = dynamic_cast<cocos2d::ui::Button*>(scrollView
                                                      ->getChildByName(buttonName.getCString()));
        btn->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
        {
            if (type == ui::Widget::TouchEventType::ENDED) {
                AudioManager::getInstance()->playSE("button_click");
                selectedStage = stage.first;
                
                auto delay = DelayTime::create(1);
                
                FiniteTimeAction* shootCloud = CallFunc::create([=]() {
                    // 画面切り替え時の雲を表示
                    auto cloudLayer = CSLoader::getInstance()->createNode("res/CloudFirstLayer.csb");
                    auto cloudAction = timeline::ActionTimelineCache::getInstance()->createAction("res/CloudFirstLayer.csb");
                    cloudLayer->runAction(cloudAction);
                    cloudAction->gotoFrameAndPlay(0, false);
                    this->addChild(cloudLayer);
                    
                });
                
                FiniteTimeAction* goToBattleScene = CallFunc::create([=]() {
                    auto scene = BattleScene::createScene(selectedStage);
                    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
                });

                auto sequence = Sequence::create(shootCloud, delay, goToBattleScene, NULL);
                this->runAction(sequence);
            }
        });
    }
    
    return true;
}
