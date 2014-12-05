#include "UI.h"
#include "StageSelectorScene.h"

USING_NS_CC;


bool UI::init()
{
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    return true;
}

void UI::showBattleController()
{
    CCLOG("UI::showBattleController()!");
    
    this->removeAllChildren();
    auto ui = CSLoader::getInstance()->createNodeFromXML("cocosstudio/UILayer_01.csd");
    ui->setScale(visibleSize.width / ui->getContentSize().width);
    this->addChild(ui,1,BattleControllerUI);
}

void UI::showBattleResult()
{
    CCLOG("UI::showBattleResult()!");
    
    this->removeAllChildren();
    
    auto ui = CSLoader::getInstance()->createNodeFromXML("cocosstudio/BattleResult.csd");
    ui->setScale(visibleSize.width / ui->getContentSize().width);
    this->addChild(ui,1,BattleResultUI);
    
    // Panel_ReturnHome -> Button_ReturnHome
    auto btn = dynamic_cast<cocos2d::ui::Button*>(ui
                                                  ->getChildByName("Panel_ReturnHome")
                                                  ->getChildByName("Button_ReturnHome"));
    btn->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
                                   if (type == ui::Widget::TouchEventType::ENDED) {
                                       auto scene = StageSelectorScene::createScene();
                                       Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
                                   }
                               });
}
