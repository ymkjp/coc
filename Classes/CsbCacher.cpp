#include "CsbCacher.h"
USING_NS_CC;

CsbCacher* CsbCacher::CsbCacherInstance = NULL;

CsbCacher::CsbCacher(){}

CsbCacher* CsbCacher::getInstance()
{
    if (NULL == CsbCacherInstance) {
        CsbCacherInstance = new CsbCacher();
        
    }
    return CsbCacherInstance;
}

//Node* CsbCacher::createNode(std::string fileName)
//{
//    auto key = csbNodeCache.find(fileName);
//    if (key == csbNodeCache.end()) {
//        // キャッシュが見つからなかったので新規作成してキャッシュに追加
//        // Vector<Node*>& getChildren
//        // _userObject	cocostudio::timeline::ActionTimelineData*
//        auto node = CSLoader::createNode(fileName);
////        auto data = dynamic_cast<timeline::ActionTimelineData*>(node->getUserObject());
//        csbNodeCache.insert(fileName, node);
//    }
//    auto newNode = Node::create();
//    auto oldNode = csbNodeCache.at(fileName);
//    newNode->setUserObject(oldNode->getUserObject());
//    for (auto child: oldNode->getChildren()) {
//        newNode->addChild(child);
//    }
//    CCLOG("newNode(%i)",!!newNode);
//    return newNode;
//}

timeline::ActionTimeline* CsbCacher::createActionTimeline(std::string fileName)
{
    auto key = csbActionCache.find(fileName);
    if (key == csbActionCache.end()) {
        // キャッシュが見つからなかったので新規作成してキャッシュに追加
        auto action = timeline::ActionTimelineCache::createAction(fileName);
        csbActionCache.insert(fileName, action);
    }
    // ActionTimeline は clonable なので clone を返す
    return csbActionCache.at(fileName)->clone();
}
