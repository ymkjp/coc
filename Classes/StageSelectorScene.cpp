#include "StageSelectorScene.h"
#include "BattleScene.h"

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
    auto ui = CSLoader::getInstance()->createNodeFromXML("cocosstudio/StageSelectorLayer.csd");
    ui->setScale(visibleSize.width / ui->getContentSize().width);
    this->addChild(ui);
    
    // Panel_RightPart -> ScrollView_1 -> Panel_Amigo -> Button_Amigo
    auto scrollView = ui->getChildByName("Panel_RightPart")->getChildByName("ScrollView_1");
    
    for (auto stage: stageNameByStages) {
        __String panelName = "Panel_";
        __String buttonName = "Button_";
        panelName.append(stage.second);
        buttonName.append(stage.second);
        auto btn = dynamic_cast<cocos2d::ui::Button*>(scrollView
                                                      ->getChildByName(panelName.getCString())
                                                      ->getChildByName(buttonName.getCString()));
        btn->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
        {
            if (type == ui::Widget::TouchEventType::ENDED) {
                selectedStage = stage.first;
                this->scheduleOnce(schedule_selector(StageSelectorScene::goToBattleScene), DISPLAY_TIME_SPLASH_SCENE);
            }
        });
    }
    
    return true;
}

void StageSelectorScene::goToBattleScene(float dt)
{
    auto scene = BattleScene::createScene(selectedStage);
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}
