#ifndef __UI_H__
#define __UI_H__

#include "cocos2d.h"
USING_NS_CC;

#include "cocostudio/CocoStudio.h"
using namespace cocostudio;
#include "cocosGUI.h"

#include "Definitions.h"
#include "Tmx.h"

class UI : public Node
{
public:
    virtual bool init(Tmx* tmx);
    static UI* create(Tmx* tmx) {
        auto p = new UI();
        if (p->init(tmx)) {
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return nullptr;
    }
    
    // UI画面はバイナリ版 (.csb) だとフォントが反映されないため、XML版 (.csd) を使って描画している
    void showBattleController();
    void showBattleResult(BattleScoreByType score);
    
    UnitType selectedUnit = NoUnitType;
    void updateUnitCounter(UnitType unitType, int count);
    void updateTimer(int currentBattleSecound);
    void updateDestructionRatio(float percentage);
    void updateBattleScore(ScoreType type, float earnedScore);
    void updateRemainingAssetScore(ScoreType type, float remainingScore);
    
    void showWarning(std::string message);
    void hideMessage();
    
protected:
    Tmx* tmx;
    Size visibleSize;
    Vec2 origin;
    Node* ui;
    
    void changeFrameVisibility();
    void goToStageSelectorScene();
};

#endif // __UI_H__
