#ifndef __UI_H__
#define __UI_H__

#include "cocos2d.h"
USING_NS_CC;

#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

#include "Definitions.h"

class UI : public Node
{
public:
    virtual bool init();
    CREATE_FUNC(UI);

protected:
    Size visibleSize;
    Vec2 origin;
};

#endif // __UI_H__
