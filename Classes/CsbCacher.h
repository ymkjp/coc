#ifndef __CSB_CACHER_H__
#define __CSB_CACHER_H__

#include <map>

#include "cocos2d.h"
USING_NS_CC;

#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

#include "Definitions.h"

class CsbCacher
{
public:
    static CsbCacher* getInstance();

//    Node* createNode(std::string fileName);
    timeline::ActionTimeline* createActionTimeline(std::string fileName);
    
protected:
//    Map<std::string,Node*> csbNodeCache;
    Map<std::string,timeline::ActionTimeline*> csbActionCache;

    
private:
    CsbCacher();
    static CsbCacher* CsbCacherInstance;
};

#endif // __CSB_CACHER_H__
