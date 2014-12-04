#include "UI.h"

USING_NS_CC;

bool UI::init()
{
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    // バイナリ版 (.csb) だとフォントが反映されないため、XML版 (.csd) を使って描画している
    Node* ui = CSLoader::getInstance()->createNodeFromXML("cocosstudio/UILayer_01.csd");
    ui->setScale(visibleSize.width / ui->getContentSize().width);
    this->addChild(ui);

//    CSLoader::createNode("res/UILayer_01.csb");
//    CSLoader::createNode("CocosProject/res/TownHall.csb");


    /**
     @example "UnitArcherWalkEast.csb"
     */

    
    return true;
}
