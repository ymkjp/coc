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
    this->removeAllChildren();
    ui = CSLoader::getInstance()->createNodeFromXML("cocosstudio/BattleController.csd");
    ui->setScale(visibleSize.width / ui->getContentSize().width);
    this->addChild(ui,1,BattleControllerUI);
    
    
    // UnitSelectorPanel -> Panel_Barbarian -> Button_Barbarian
    auto selector = ui->getChildByName("UnitSelectorPanel");
    
    for (auto _unitNameByType: unitNameByType) {
        __String panelName = "Panel_";
        __String buttonName = "Button_";
        
        panelName.append(_unitNameByType.second);
        buttonName.append(_unitNameByType.second);
        auto btn = dynamic_cast<cocos2d::ui::Button*>(selector
                                                      ->getChildByName(panelName.getCString())
                                                      ->getChildByName(buttonName.getCString()));
        btn->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
                                   {
                                       if (type == ui::Widget::TouchEventType::ENDED) {
                                           selectedUnit = _unitNameByType.first;
                                           changeFrameVisibility();
                                       }
                                   });
    }
    
    // EndBattleButtonPanel -> Button_EndButtle
    auto btn = dynamic_cast<cocos2d::ui::Button*>(ui
                                                  ->getChildByName("EndBattleButtonPanel")
                                                  ->getChildByName("Button_EndButtle"));
    btn->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
                               {
                                   if (type == ui::Widget::TouchEventType::ENDED) {
                                       this->showBattleResult();
                                   }
                               });
}

inline void UI::changeFrameVisibility()
{
    auto selector = ui->getChildByName("UnitSelectorPanel");
    for (auto _unitNameByType: unitNameByType) {
        __String panelName = "Panel_";
        __String frameName = "stage_ui_unit_frame_";
        frameName.append(_unitNameByType.second);
        panelName.append(_unitNameByType.second);
        
        selector
        ->getChildByName(panelName.getCString())
        ->getChildByName(frameName.getCString())
        ->setVisible(_unitNameByType.first == selectedUnit);
    }
}

void UI::showBattleResult()
{
    this->removeAllChildren();
    
    ui = CSLoader::getInstance()->createNodeFromXML("cocosstudio/BattleResult.csd");
    ui->setScale(visibleSize.width / ui->getContentSize().width);
    this->addChild(ui,1,BattleResultUI);
    
    // Panel_ReturnHome -> Button_ReturnHome
    auto btn = dynamic_cast<cocos2d::ui::Button*>(ui
                                                  ->getChildByName("Panel_ReturnHome")
                                                  ->getChildByName("Button_ReturnHome"));
    btn->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
                                   if (type == ui::Widget::TouchEventType::ENDED) {
                                       this->goToStageSelectorScene();
                                   }
                               });
}

void UI::goToStageSelectorScene()
{
    auto scene = StageSelectorScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}
