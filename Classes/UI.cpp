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
    this->changeFrameVisibility();
    this->addChild(ui,1,BattleControllerUI);
    
    
    // ユニット選択ボタン
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
    
    // 「バトル終了」ボタン
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


// @todo どの画面を表示しているか管理できたほうがよい
void UI::updateTimer(int currentBattleSecound)
{
    // BattleRemainLabelPanel->Text_Timer{_Shadow}
    int minutes = currentBattleSecound / 60;
    int secounds = currentBattleSecound % 60;
    auto text = StringUtils::format("%d m %d s", minutes, secounds);
    auto panel = ui->getChildByName("BattleRemainLabelPanel");
    auto label = dynamic_cast<cocos2d::ui::Text*>(panel->getChildByName("Text_Timer"));
    auto labelShadow = dynamic_cast<cocos2d::ui::Text*>(panel->getChildByName("Text_Timer_Shadow"));
    
    label->setString(text.c_str());
    labelShadow->setString(text.c_str());
}

void UI::updateBattleScore(ScoreType type, float earnedScore)
{
    // PlayersAssetPanel->Text_Players{Coin|Elixer}
    auto name = scoreNameByType.at(type);
    
    auto labelText = StringUtils::format("%d", (int) earnedScore);
    auto label = dynamic_cast<cocos2d::ui::Text*>(ui
                                                  ->getChildByName("PlayersAssetPanel")
                                                  ->getChildByName("Text_Players" + name));
    label->setString(labelText.c_str());
}

void UI::updateRemainingAssetScore(ScoreType type, float remainingScore)
{
    // AvailableLootPanel -> Text_{Coin|Elixer}_Shadow
    auto name = scoreNameByType.at(type);
    
    auto labelText = StringUtils::format("%d", (int) remainingScore);
    auto panel = ui->getChildByName("AvailableLootPanel");
    auto label = dynamic_cast<cocos2d::ui::Text*>(panel->getChildByName("Text_" + name));
    auto labelShadow = dynamic_cast<cocos2d::ui::Text*>(panel->getChildByName("Text_" + name + "_Shadow"));
    
    label->setString(labelText.c_str());
    labelShadow->setString(labelText.c_str());
}

void UI::updateDestructionRatio(float percentage)
{
    // OverallDamagePanel -> Text_DamagePercent{_Shadow}
    auto text = StringUtils::format("%d", (int)percentage);
    text.append(" %");
    auto panel = ui->getChildByName("OverallDamagePanel");
    auto label = dynamic_cast<cocos2d::ui::Text*>(panel->getChildByName("Text_DamagePercent"));
    auto labelShadow = dynamic_cast<cocos2d::ui::Text*>(panel->getChildByName("Text_DamagePercent_Shadow"));
    
    label->setString(text.c_str());
    labelShadow->setString(text.c_str());
}

void UI::updateUnitCounter(UnitType unitType, int count)
{
    // UnitSelectorPanel -> Panel_Barbarian -> Text_UnitCounter_Barbarian
    auto unitName = unitNameByType.at(unitType);
    auto selector = ui->getChildByName("UnitSelectorPanel");
    __String panelName = "Panel_";
    __String labelName = "Text_UnitCounter_";
    panelName.append(unitName);
    labelName.append(unitName);
    auto labelText = StringUtils::format("x%d", count);
    auto label = dynamic_cast<cocos2d::ui::Text*>(selector
                                                  ->getChildByName(panelName.getCString())
                                                  ->getChildByName(labelName.getCString()));
    label->setString(labelText.c_str());
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
