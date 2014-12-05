#ifndef __UI_H__
#define __UI_H__

#include "cocos2d.h"
USING_NS_CC;

#include "cocostudio/CocoStudio.h"
using namespace cocostudio;
#include "cocosGUI.h"

#include "Definitions.h"

class UI : public Node
{
public:
    virtual bool init();
    CREATE_FUNC(UI);
    
    // UI画面はバイナリ版 (.csb) だとフォントが反映されないため、XML版 (.csd) を使って描画している
    void showBattleController();
    void showBattleResult();

protected:
    Size visibleSize;
    Vec2 origin;
};

#endif // __UI_H__
