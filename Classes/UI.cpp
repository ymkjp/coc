#include "UI.h"
#include "StageSelectorScene.h"

USING_NS_CC;

bool UI::init(Tmx* _tmx)
{
    tmx = _tmx;
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
        auto panelName = "Panel_" + _unitNameByType.second;
        auto buttonName = "Button_" + _unitNameByType.second;
        
        auto btn = dynamic_cast<cocos2d::ui::Button*>(selector
                                                      ->getChildByName(panelName.c_str())
                                                      ->getChildByName(buttonName.c_str()));
        btn->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
                                   {
                                       if (type == ui::Widget::TouchEventType::ENDED) {
                                           selectedUnit = _unitNameByType.first;
                                           changeFrameVisibility();
                                       }
                                   });
    }
    
    // 赤いメッセージ
    auto warnPanel = ui->getChildByName("Panel_Warn");
    warnPanel->setVisible(false);
    
    // 「バトル終了」ボタン
    // EndBattleButtonPanel -> Button_EndButtle
    auto btn = dynamic_cast<cocos2d::ui::Button*>(ui
                                                  ->getChildByName("EndBattleButtonPanel")
                                                  ->getChildByName("Button_EndButtle"));
    btn->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
                               {
                                   if (type == ui::Widget::TouchEventType::ENDED) {
                                       CCLOG("Button_EndButtle");
                                       tmx->endBattle(0.01);
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

void UI::showStar(int starId)
{
    // OverOverallDamagePanel(-400,90) -> Panel_Star_{1|2|3} -> {stage_battle_result_silver_star|Text_StarMessage|Text_StarMessage_Shadow}

    // Star_1 (32,32.6) 20%
    // Star_2 (70,32.6) 20%
    // Star_3 (105,32.6) 20%
    
    // アニメーション順序
    // 1. 10 frame で star と message fadein
    // 2. 10 frame そのまま固定
    // 3. 30 frame で star は所定の位置へ縮小しながら移動, message も同じく追従する
    // 4. 移動完了後、message は消去

    auto parentPanel = ui->getChildByName("OverallDamagePanel");
    if (!parentPanel) {
        return;
    }
    
    auto panelName = StringUtils::format("Panel_Star_%d", starId);
    CCLOG("panelName(%s)",panelName.c_str());
    
    auto starPanel = parentPanel->getChildByName(panelName);
    auto fadeIn = FadeIn::create(0.5);
    auto stay = DelayTime::create(0.5);
    auto moving = MoveTo::create(1, starPos.at(starId));
    auto shrinking = ScaleTo::create(1, 0.2);
    FiniteTimeAction* deleteMessage = CallFunc::create([=]() {
        if (starPanel) {
            auto message = starPanel->getChildByName("Text_StarMessage");
            auto messageShadow = starPanel->getChildByName("Text_StarMessage_Shadow");
            if (message && messageShadow) {
                message->setVisible(false);
                messageShadow->setVisible(false);
            }
        }
    });
    auto seq = Sequence::create(fadeIn,stay,Spawn::create(moving,shrinking, NULL),deleteMessage, NULL);
    if (starPanel) {
        starPanel->setVisible(true);
        starPanel->runAction(seq);
    }
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
    percentage = (percentage > 100) ? 100 : percentage;
    auto text = StringUtils::format("%d", (int)percentage);
    text.append("%");
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
    auto panelName = "Panel_" + unitName;
    auto labelName = "Text_UnitCounter_" + unitName;
    auto labelText = StringUtils::format("x%d", count);
    auto label = dynamic_cast<cocos2d::ui::Text*>(selector
                                                  ->getChildByName(panelName.c_str())
                                                  ->getChildByName(labelName.c_str()));
    label->setString(labelText.c_str());
    
    // 0 になったらボタンを黒くする
    // UnitSelectorPanel -> Panel_Barbarian -> Button_Barbarian
    if (count <= 0) {
        auto blind = selector
        ->getChildByName(panelName.c_str())
        ->getChildByName("stage_ui_unit_black");
        
        CCLOG("unitName(%s),count(%i),blind?(%i)",unitName.c_str(),count,!!blind);
        blind->setVisible(true);
    }
}

inline void UI::changeFrameVisibility()
{
    auto selector = ui->getChildByName("UnitSelectorPanel");
    for (auto _unitNameByType: unitNameByType) {
        auto panelName = "Panel_" + _unitNameByType.second;
        auto frameName = "stage_ui_unit_frame_" + _unitNameByType.second;
        
        selector
        ->getChildByName(panelName.c_str())
        ->getChildByName(frameName.c_str())
        ->setVisible(_unitNameByType.first == selectedUnit);
    }
}

void UI::showWarning(std::string message)
{
    // Panel_Warn -> Text_Warn{_Shadow}
    auto panel = ui->getChildByName("Panel_Warn");
//    CCLOG("1.getOpacity(%i)",panel->getOpacity());;
    panel->setVisible(true);
    panel->setOpacity(255);
//    CCLOG("2.getOpacity(%i)",panel->getOpacity());
    auto text = dynamic_cast<cocos2d::ui::Text*>(panel->getChildByName("Text_Warn"));
    auto textShadow = dynamic_cast<cocos2d::ui::Text*>(panel->getChildByName("Text_Warn_Shadow"));
    text->setString(message);
    textShadow->setString(message);
    
    auto disappear = FadeOut::create(2);
    FiniteTimeAction* invisible = CallFunc::create([=]() {
        panel->setVisible(false);
//        CCLOG("3.getOpacity(%i)",panel->getOpacity());;
    });
    auto sequence = Sequence::create(disappear, invisible, NULL);
    
    panel->runAction(sequence);
}

void UI::hideMessage()
{
    // Panel_Message
    auto panel = ui->getChildByName("Panel_Message");
    auto disappear = FadeOut::create(0.2);
    FiniteTimeAction* invisible = CallFunc::create([=]() {
        panel->setVisible(false);
    });
    auto sequence = Sequence::create(disappear, invisible, NULL);
    
    panel->runAction(sequence);

}

void UI::showBattleResult(BattleScoreByType battleScoreByType)
{
    this->removeAllChildren();
    
    ui = CSLoader::getInstance()->createNodeFromXML("cocosstudio/BattleResult.csd");
    ui->setScale(visibleSize.width / ui->getContentSize().width);
    this->addChild(ui,1,BattleResultUI);
    
    // Panel_ReturnHome -> Button_ReturnHome
    auto btn = dynamic_cast<cocos2d::ui::Button*>(ui
                                                  ->getChildByName("Panel_ReturnHome")
                                                  ->getChildByName("Button_ReturnHome"));
    btn->setTouchEnabled(false);
    btn->setOpacity(0);
    FiniteTimeAction* btnVisible = CallFunc::create([=]() {
        btn->setTouchEnabled(true);
    });
    auto sequence = Sequence::create(DelayTime::create(1), btnVisible, FadeIn::create(0.1),NULL);
    btn->runAction(sequence);
    
    btn->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
                                   if (type == ui::Widget::TouchEventType::ENDED) {
                                       this->goToStageSelectorScene();
                                   }
                               });
    
    auto mainResultPanel = ui->getChildByName("Panel_MainResult");
    float percentage = battleScoreByType.at(DestructionRatioScore);
    percentage = (percentage > 100) ? 100 : percentage;

    int starCount = battleScoreByType[StarCountScore];
    bool isWon = (1 <= starCount);
    
    // 勝敗を表示
    // Panel_MainResult -> Panel_Ribbon -> Text_Victory{_Shadow}
    auto winningText = isWon ? "Victory" : "Defeated";
    auto ribbonPanel = mainResultPanel->getChildByName("Panel_Ribbon");
    auto winningLabel = dynamic_cast<cocos2d::ui::Text*>(ribbonPanel->getChildByName("Text_Victory"));
    auto winningLabelShadow = dynamic_cast<cocos2d::ui::Text*>(ribbonPanel->getChildByName("Text_Victory_Shadow"));
    winningLabel->setTextColor(isWon ? Color4B::YELLOW : Color4B::RED);
    
    winningLabel->setString(winningText);
    winningLabelShadow->setString(winningText);
    
    // 破壊率を表示
    // Panel_MainResult -> Panel_TotalDamage -> Text_Percentage{_Shadow}
    auto text = StringUtils::format("%d", (int)percentage);
    text.append("%");
    auto panel = mainResultPanel->getChildByName("Panel_TotalDamage");
    auto labelDamaged = dynamic_cast<cocos2d::ui::Text*>(panel->getChildByName("Text_Percentage"));
    auto labelDamagedShadow = dynamic_cast<cocos2d::ui::Text*>(panel->getChildByName("Text_Percentage_Shadow"));
    
    labelDamaged->setString(text.c_str());
    labelDamagedShadow->setString(text.c_str());
    
    // 獲得星数を表示
    // 処理としては獲得できなかった星の数を黒く塗りつぶす
    // Panel_MainResult -> Panel_Ribbon -> stage_battle_result_silver_star_{1|2|3}
    auto starFirst = ribbonPanel->getChildByName("stage_battle_result_silver_star_1");
    auto starSecond = ribbonPanel->getChildByName("stage_battle_result_silver_star_2");
    auto starThird = ribbonPanel->getChildByName("stage_battle_result_silver_star_3");
    if (starCount <= 0 && starFirst && starSecond && starThird) {
        starFirst->setColor(Color3B::BLACK);
        starFirst->setOpacity(200);
        starSecond->setColor(Color3B::BLACK);
        starSecond->setOpacity(200);
        starThird->setColor(Color3B::BLACK);
        starThird->setOpacity(200);
    } else if (starCount == 1 && starSecond && starThird) {
        starSecond->setColor(Color3B::BLACK);
        starSecond->setOpacity(200);
        starThird->setColor(Color3B::BLACK);
        starThird->setOpacity(200);
    } else if (starCount == 2 && starThird) {
        starThird->setColor(Color3B::BLACK);
        starThird->setOpacity(200);
    }
    
    // 獲得した Gold, Elixer の量を表示
    // Text_GoldScore, Text_ElixerScore
    auto lebelGold = dynamic_cast<cocos2d::ui::Text*>(ui->getChildByName("Text_GoldScore"));
    auto lebelElixer = dynamic_cast<cocos2d::ui::Text*>(ui->getChildByName("Text_ElixerScore"));
    auto goldText = StringUtils::format("%d", (int)battleScoreByType[CoinScore]);
    auto elixerText = StringUtils::format("%d", (int)battleScoreByType[ElixerScore]);
    lebelGold->setString(goldText);
    lebelElixer->setString(elixerText);
    
    // 後ろで回転している光
    // Panel_MainResult -> stage_battle_result_light
    auto backLight = mainResultPanel->getChildByName("stage_battle_result_light");
    auto rotation = RepeatForever::create(RotateBy::create(12, 360));
    backLight->runAction(rotation);
}

void UI::goToStageSelectorScene()
{
    auto scene = StageSelectorScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}
