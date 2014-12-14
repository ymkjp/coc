#include <iostream>
using namespace std;

#include "MapNavigator.h"

USING_NS_CC;

bool MapNavigator::init()
{
    return true;
}

/**
 @note 上限値を超えると Vec2(-1,-1) を返す
 @param Vec2 goalPoint should be __STATUS::NONE
 @link http://2dgames.jp/2012/05/22/a-starar/
 */
PathToGoal MapNavigator::navigate(const WorldGrid* _worldGrid, Vec2 startPoint, Vec2 goalPoint)
{
//    CCLOG("start(%f,%f),goal(%f,%f);openSet.size(%lu)",
//    startPoint.x,startPoint.y,goalPoint.x,goalPoint.y,openSet.size());
    
    // スタート地点とゴール地点が同じ場合は empty path を返す
    PathToGoal pathToGoal = {};
    if (startPoint == goalPoint) {
        return pathToGoal;
    }
    
    // キャッシュヒットすればそれを返す
    std::array<Vec2,2> cacheKey = {startPoint, goalPoint};
    auto key = pathCache.find(cacheKey);
    if (key != pathCache.end() && pathCache.at(cacheKey).size() > 1) {
        CCLOG("cache found");
        // キャッシュが見つかったのでそれを返す
        return pathCache.at(cacheKey);
    }
    
    auto result = std::async(std::launch::async, [=]
    {
        // 生成済みの
        WorldGrid worldGrid = *_worldGrid;
        Vec2Set openSet;
        AStar startNode = {};
        AStar* bestNode = nullptr;
        
        startNode.status = AStar::__STATUS::OPEN;
        startNode.cost = 0;
        startNode.distance = startPoint.getDistanceSq(goalPoint);
        startNode.parent = nullptr;
        startNode.pos = startPoint;
        worldGrid[static_cast<int>(startPoint.x)][static_cast<int>(startPoint.y)] = startNode;
        
        openSet.insert(startPoint);
        
        // Open first 8 nodes
        Vec2 targetPoint;
        for (Vec2Set::iterator i = surround.begin(); i != surround.end(); ++i)
        {
            targetPoint = startPoint + *i;
            if (isInMapRange(targetPoint) && worldGrid[static_cast<int>(targetPoint.x)][static_cast<int>(targetPoint.y)].status == AStar::__STATUS::NONE)
            {
//                CCLOG("[0]targetPoint(%f,%f)",targetPoint.x,targetPoint.y);
                AStar node = {};
                node.cost = 0;
                node.status = AStar::__STATUS::OPEN;
                node.distance = targetPoint.getDistanceSq(goalPoint);
                node.pos = targetPoint;
                node.parent = &startNode;
                worldGrid[static_cast<int>(targetPoint.x)][static_cast<int>(targetPoint.y)] = node;
                openSet.insert(targetPoint);
            }
        }
        worldGrid[static_cast<int>(startPoint.x)][static_cast<int>(startPoint.y)].status = AStar::__STATUS::CLOSE;
        openSet.erase(startPoint);
        
        // Find goal from openSet
        bool isFound = false;
        bool isOverSteps = false;
        int steps = 0;
        Vec2 bestPoint;
        Vec2 nextPoint;
        AStar* openedNode = nullptr;
        while (isFound == false) {
            ++steps;
            if (PATH_FINDING_MAX_THRESHOLD < steps) {
//                CCLOG("PATH_FINDING_MAX_THRESHOLD");
                isOverSteps = true;
                break;
            }
            for (Vec2Set::iterator openedPoint = openSet.begin(); openedPoint != openSet.end(); ++openedPoint)
            {
//                CCLOG("[%i]openedPoint(%f,%f)",steps,openedPoint->x,openedPoint->y);
                openedNode = &worldGrid[static_cast<int>(openedPoint->x)][static_cast<int>(openedPoint->y)];
                if (*openedPoint == goalPoint) {
                    isFound = true;
                    break;
                } else if (bestNode ==  nullptr || openedNode->GetScore() < bestNode->GetScore() ) {
                    bestPoint = *openedPoint;
                    bestNode = openedNode;
                } else if (openedNode->GetScore() == bestNode->GetScore() && arc4random_uniform(2) == 1) { /** @fixme **/
                    bestPoint = *openedPoint;
                    bestNode = openedNode;
                }
            }
            
            if (isFound) {
//                CCLOG("FOUND(%f,%f)",openedNode->pos.x,openedNode->pos.y);
                break;
            }
            
//            CCLOG("[%i]bestPoint(%f,%f)",steps,bestPoint.x,bestPoint.y);
            
            for (Vec2Set::iterator i = surround.begin(); i != surround.end(); ++i)
            {
                nextPoint = bestPoint + Vec2(i->x,i->y);
                if (isInMapRange(nextPoint)
                    && worldGrid[static_cast<int>(nextPoint.x)][static_cast<int>(nextPoint.y)].status == AStar::__STATUS::NONE
                    && openSet.find(nextPoint) == openSet.end()
                    ) {
//                    CCLOG("[%i]nextPoint(%f,%f)",steps,nextPoint.x,nextPoint.y);
                    AStar nextNode = {};
                    nextNode.status = AStar::__STATUS::OPEN;
                    nextNode.cost = steps;
                    nextNode.distance = nextPoint.getDistanceSq(goalPoint);
                    nextNode.pos = nextPoint;
                    nextNode.parent = bestNode;
                    worldGrid[static_cast<int>(nextPoint.x)][static_cast<int>(nextPoint.y)] = nextNode;
                    openSet.insert(nextPoint);
                }
            }
            worldGrid[static_cast<int>(bestPoint.x)][static_cast<int>(bestPoint.y)].status = AStar::__STATUS::CLOSE;
            openSet.erase(bestPoint);
            bestNode = {};
        }
        openSet.clear();
        
        // 探索終了
        // openedNode に path が入っている
        
        // 結果を用意する
        PathToGoal pathToGoal = {};
        
        if (isOverSteps) {
            pathToGoal.push(Vec2(-1,-1));
            return pathToGoal;
        }
        
        while (openedNode->parent != nullptr) {
            //        ++count; CCLOG("[%i]pathPos(%f,%f)",count,path->pos.x,path->pos.y);
            pathToGoal.push(openedNode->pos);
            openedNode = openedNode->parent;
        }
        
        return pathToGoal;
    });
    
    pathToGoal = result.get();
    
    
    // キャッシュに追加
    pathCache[cacheKey] = pathToGoal;
    
    return pathToGoal;
}
